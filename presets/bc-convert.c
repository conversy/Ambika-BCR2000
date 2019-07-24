/*****************************************************************************\
| BC-Convert - a conversion utility for B-Control SysEx data                  |
| (C) 2006 Michael Kukat                                                      |
|-----------------------------------------------------------------------------|
| This program is able to convert files between B-Control system exclusive    |
| data and plain text files which are suitable to create complex controller   |
| configurations for those MIDI controllers.                                  |
|-----------------------------------------------------------------------------|
| WARNING!                                                                    |
| This program is provided as-is, without any warranties. If you don't know   |
| what you are doing, you can cause data loss or in worst case even damage to |
| your equipment. Use on your own risk!                                       |
\*****************************************************************************/

/* $Id: bc-convert.c,v 1.3 2006/12/23 15:30:08 michael Exp $ */

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <inttypes.h>

#define	VERSION_HI	0
#define	VERSION_LO	3
#define	BUFSIZE		131072				/* default buffer size when reading from stdin */
#undef	USE_DEBUG
#ifdef USE_DEBUG
#define	DEBUG(...)	fprintf(stderr, __VA_ARGS__);	/* define as empty to disable debugging */
#else	/* USE_DEBUG */
#define	DEBUG(...)
#endif	/* USE_DEBUG */

/* Behringer SysEx Header */

struct sysex_header {
	uint8_t start;		/* sysex start - 0xf0 */
	uint8_t mfrid1;		/* manufacturer ID 1 - 0x00 */
	uint8_t mfrid2;		/* manufacturer ID 2 - 0x20 */
	uint8_t mfrid3;		/* manufacturer ID 3 - 0x32 */
	uint8_t devid;		/* device ID */
	uint8_t mdlid;		/* model ID - 0x15 for BCR2000 */
	uint8_t command;	/* command - 0x20 for patch data */
	uint8_t blk_hi;		/* block number high byte */
	uint8_t blk_lo;		/* block number low byte */
	uint8_t end;		/* sysex end - 0xf7 */
};

/* SysEx header for BCR2000 device ID 0, patch data */
const struct sysex_header bcr_syx_hdr = { 0xf0, 0x00, 0x20, 0x32, 0x00, 0x15, 0x20, 0x00, 0x00, 0xf7 };
struct sysex_header hdr;

/* global buffer pointer - the buffer will keep the file read from disk */
uint8_t *buffer;
uint32_t buffer_free;
uint32_t file_size;

void convert_sysex_to_text(FILE *in, FILE *out, uint32_t *data_size, uint32_t *buffer_free) {
	uint8_t *sptr, *eptr, *fptr, *tptr;
	struct sysex_header *tmp2;
	uint32_t tmp;
	uint16_t line;

	DEBUG("Converting block from SysEx to text\n");

	/* initialize buffer scanner */
	sptr = buffer;
	fptr = &(sptr[*data_size]);
	line = (hdr.blk_hi << 7) | hdr.blk_lo;

	/* scan over buffer and isolate text lines */
	while(*data_size > 0) {
		/* isolate sysex message */
		eptr = sptr;

		/* check for sysex message */
		if(*sptr != 0xf0) {
			/* no sysex message, skip until message found or buffer end */
			while(eptr < fptr && *eptr != 0xf0) eptr ++;
			tmp = eptr - sptr;
			fprintf(stderr, "Found garbage instead of SysEx start byte, skipped %d bytes\n", tmp);
		} else {
			/* sysex message found, check for correct type */
			if(!bcmp(sptr, &hdr, sizeof(hdr) - 3)) {
				/* get clock number from message */
				tmp2 = (struct sysex_header *)sptr;
				tmp = (tmp2->blk_hi << 7) | tmp2->blk_lo;

				/* check if block number is okay */
				if(tmp != line) {
					fprintf(stderr, "Found block %d, expected block %d. File corrupted?\n", tmp, line);
					line = tmp;
				}

				/* get pointer to message data */
				eptr = &(sptr[sizeof(hdr) - 1]);
				tptr = eptr;

				/* scan message for end */
				while(eptr < fptr && *eptr != 0xf7) eptr++;

				/* if no sysex end was found, terminate loop */
				tmp = 0;
				if(*eptr != 0xf7) break;
				tmp = eptr - tptr;

				/* output message data */
				if(tmp > 0) fwrite(tptr, 1, tmp, out);
#ifdef __WATCOMC__
				fprintf(out, "\r\n");
#else	/* __WATCOMC__ */
				fprintf(out, "\n");
#endif	/* __WATCOMC__ */
				line++;
			} else {
				/* not of expected type, skip */
				fprintf(stderr, "SysEx message isn't of expected type, skipping\n");
				while(eptr < fptr && *eptr != 0xf7) eptr ++;
			}
			while(eptr < fptr && *eptr == 0xf7) eptr ++;
			tmp = eptr - sptr;
		}

		/* fix pointers for next line */
		sptr = eptr;
		*data_size -= tmp;
		*buffer_free += tmp;
	}

	/* move end of buffer to beginning if something has been left over */
	if(*data_size > 0) {
		eptr = buffer;
		while(sptr < fptr) *(eptr++) = *(sptr++);
	}
}

void convert_text_to_sysex(FILE *in, FILE *out, uint32_t *data_size, uint32_t *buffer_free) {
	uint8_t *sptr, *eptr, *fptr, *tptr;
	uint32_t tmp;
	uint16_t line;

	DEBUG("Converting block from text to SysEx\n");

	/* initialize buffer scanner */
	sptr = buffer;
	fptr = &(sptr[*data_size]);
	line = (hdr.blk_hi << 7) | hdr.blk_lo;

	/* scan over buffer and generate sysex blocks */
	while(*data_size > 0) {
		/* first isolate a complete line */
		eptr = sptr;
		while(eptr < fptr && *eptr != 0x0a && *eptr != 0x0d) eptr++;

		/* if no line terminator was found, terminate loop */
		tmp = 0;
		if(*eptr != 0x0a && *eptr != 0x0d) break;
		tmp = eptr - sptr;

		/* scan for comment char */
		tptr = sptr;
		while(tptr < eptr && *tptr != '#' && *tptr != ';') tptr++;
		
		/* scan backwards to kill whitespaces */
		if(*tptr == '#' || *tptr == ';') while(tptr > sptr) {
			tptr--;
			if(*tptr > 0x20) {
				tptr++;
				break;
			}
		}

		/* calculate length of line */
		tmp = tptr - sptr;

		/* check for invalid characters */
		while((--tptr) >= sptr) if(*tptr >= 0x80) {
			fprintf(stderr, "Found invalid character [%c]. Skipping line.\n", *tptr);
			tmp = 0;
			break;
		}

		/* if useful chars are left over, write a block */
		if(tmp > 0) {
			fwrite(&hdr, 1, sizeof(hdr) - 1, out);
			fwrite(sptr, 1, tmp, out);
			fwrite(&(hdr.end), 1, 1, out);
			line += 1;
			hdr.blk_hi = line >> 7;
			hdr.blk_lo = line & 0x7f;
		}

		/* skip line termination */
		while(eptr < fptr && (*eptr == 0x0a || *eptr == 0x0d)) eptr++;
		tmp = eptr - sptr;

		/* fix pointers for next line */
		sptr = eptr;
		*data_size -= tmp;
		*buffer_free += tmp;
	}

	/* move end of buffer to beginning if something has been left over */
	if(*data_size > 0) {
		eptr = buffer;
		while(sptr < fptr) *(eptr++) = *(sptr++);
	}
}

/* the main program - parameter parsing, main routing */
int main(int argc, char **argv) {
	uint8_t argnum, usage, force_syx, force_txt, device_id, model_id, stream_mode;
	uint32_t tmp;
	char *arg, *endarg, *errmsg, prevarg;
	char *infile, *outfile;
	FILE *in, *out;

	DEBUG("--- Starting ---\n");
	errmsg = "Unknown message";

	/* startup banner */
	fprintf(stderr, "BC-Convert Version %d.%d (C) 2006 Michael Kukat\n", VERSION_HI, VERSION_LO);

	/* default values */
	force_syx = 0;
	force_txt = 0;
	device_id = 127;
	model_id = 127;
	infile = NULL;
	outfile = NULL;
	stream_mode = 1;

	/* initialize argument parser */
	DEBUG("Initializing argument parser\n");
	argnum = 1;
	usage = 0;
	prevarg = 0;

	/* read all arguments and configure variables */
	DEBUG("Parsing arguments:\n");
	while(usage == 0 && argnum < argc) {
		arg = argv[argnum];
		DEBUG("  ARG: %s\n", arg);

		if(prevarg == 0 && arg[0] == '-') {
			switch(arg[1]) {
				case 'i':		/* input file */
				case 'o':		/* output file */
				case 'd':		/* device ID */
				case 'm':		/* model ID */
					prevarg = arg[1];
					break;
				case 's':		/* force sysex output */
					force_syx = 1;
					break;
				case 't':		/* force text output */
					force_txt = 1;
					break;
				case 'h':		/* display help */
					usage = 1;
					errmsg = "Help requested";
					break;
				default:
					errmsg = "Invalid option";
					usage = 1;
					break;
			}
		} else switch(prevarg) {
			case 'i':			/* input file name */
				infile = arg;
				prevarg = 0;
				break;
			case 'o':			/* output file name */
				outfile = arg;
				prevarg = 0;
				break;
			case 'd':			/* device ID */
				device_id = strtoul(arg, &endarg, 10);
				prevarg = 0;
				if(arg[0] == 0 || *endarg != 0) {
					errmsg = "Invalid number";
					usage = 1;
				}
				break;
			case 'm':			/* model ID */
				model_id = strtoul(arg, &endarg, 10);
				prevarg = 0;
				if(arg[0] == 0 || *endarg != 0) {
					errmsg = "Invalid number";
					usage = 1;
				}
				break;
			default:
				errmsg = "Invalid option";
				usage = 1;
				break;
		}
		argnum++;
	}

	/* sysex and text generation can't be used in one run */
	DEBUG("Check force flags\n");
	if(force_syx == 1 && force_txt == 1) {
		errmsg = "Can't output SysEx and text together";
		usage = 1;
	}

	/* check for missing option arguments */
	DEBUG("Check if args are complete\n");
	if(prevarg != 0) {
		errmsg = "Missing option argument";
		usage = 1;
	}

	/* something went wrong, display cmdline usage */
	if(usage) {
		fprintf(stderr, "Usage: %s [options]\n", argv[0]);
		fprintf(stderr, "Error: %s\n", errmsg);
		fprintf(stderr, "Options:\n");
		fprintf(stderr, "\t-i <input file>\t\tInput file (default stdin)\n");
		fprintf(stderr, "\t-o <output file>\tOutput file (default stdout)\n");
		fprintf(stderr, "\t-d <device id>\t\tDevice ID (used on SysEx output only)\n");
		fprintf(stderr, "\t-m <model id>\t\tModel ID (used on SysEx output only)\n");
		fprintf(stderr, "\t-s\t\t\tForce SysEx output (default autodetect)\n");
		fprintf(stderr, "\t-t\t\t\tForce text output (default autodetect)\n");
		fprintf(stderr, "\n");
		fprintf(stderr, "-s and -t can't be used together for obvious reasons.\n");
		return 1;
	}

	/* initialize file pointers */
	DEBUG("Initializing files and buffer pointers\n");
	in = NULL;
	out = NULL;
	buffer = NULL;
	buffer_free = 0;
	file_size = 0;

	/* open input file */
	if(infile != NULL) {
		stream_mode = 0;
		DEBUG("Opening input from file\n");
		in = fopen(infile, "rb");
		if(in == NULL) {
			perror("Cannot open input file");
		} else {
			/* get file size */
			DEBUG("Detemining file size\n");
			fseek(in, 0, SEEK_END);
			buffer_free = ftell(in);
			fseek(in, 0, SEEK_SET);

			/* if file is not empty, allocate buffer and read file */
			DEBUG("File size: %d bytes\n", buffer_free);
			if(buffer_free > 0) {
				/* allocate buffer */
				DEBUG("Allocating buffer\n");
				buffer = malloc(buffer_free);
				if(buffer != NULL) {
					/* read file into buffer */
					DEBUG("Reading file\n");
					while(buffer_free > 0 && !feof(in)) {
						tmp = fread(buffer, 1, buffer_free, in);
						DEBUG("  Read %d bytes\n", tmp);
						if(tmp > 0) {
							buffer_free -= tmp;
							file_size += tmp;
						}
					}
					/* if 0 bytes were read, something strange happened */
					if(file_size == 0) {
						fprintf(stderr, "Problem reading input file - nothing to do\n");
						free(buffer);
						buffer = NULL;
					}
				} else {
					fprintf(stderr, "Cannot allocate file buffer\n");
				}
			} else {
				fprintf(stderr, "Input file is empty - nothing to do\n");
			}
		}
	} else {
		/* for stdin, allocate standard buffer */
		DEBUG("Using stdin\n");
		in = stdin;
		buffer_free = BUFSIZE;
		DEBUG("Allocating default buffer, %d bytes\n", buffer_free);
		buffer = malloc(buffer_free);
		if(buffer == NULL) {
			fprintf(stderr, "Cannot allocate file buffer\n");
		}
	}

	/* if input file opening was successful, open output file */
	if(buffer != NULL) {
		if(outfile != NULL) {
			/* open output file */
			DEBUG("Opening output file\n");
			out = fopen(outfile, "wb");
			if(out == NULL) {
				perror("Cannot open output file");
			}
		} else {
			DEBUG("Using stdout\n");
			out = stdout;
		}
	}

	/* initialize sysex header */
	DEBUG("Initializing SysEx header\n");
	bcopy((void *)&bcr_syx_hdr, &hdr, sizeof(struct sysex_header));

	/* set device and model ID to header */
	hdr.devid = device_id;
	hdr.mdlid = model_id;

	/* if stream mode is active, fill buffer */
	if(stream_mode) {
		DEBUG("Stream mode, filling buffer (current size %d, free %d).\n", file_size, buffer_free);
		while(!feof(in) && buffer_free > 0) {
			tmp = fread(&(buffer[file_size]), 1, buffer_free, in);
			DEBUG("Read from input returned %d\n", tmp);
			if(tmp > 0) {
				buffer_free -= tmp;
				file_size += tmp;
			}
		}
	}

	/* check conversion direction and device/model ID */
	if(file_size > 0) {
		/* we are using a real input file */
		if(!force_syx && !force_txt) {
			DEBUG("Auto-detecting conversion direction\n");
			if(buffer[0] == 0xf0) {
				force_txt = 1;
				fprintf(stderr, "First byte is SysEx start - using SysEx to text conversion.\n");
			} else {
				force_syx = 1;
				fprintf(stderr, "First byte is no SysEx start - using text to SysEx conversion.\n");
			}
		}

		/* check buffer depending on conversion direction */
		if(force_syx) {
			DEBUG("Checking first buffer char for >= 0x80\n");
			if(buffer[0] >= 0x80) {
				fprintf(stderr, "First char of file is invalid, cannot convert to SysEx. Terminating.\n");
				file_size = 0;
				stream_mode = 0;
			}
		} else {
			DEBUG("Checking buffer for Behringer SysEx header\n");
			if(!bcmp(buffer, &hdr, 4)) {
				hdr.devid = buffer[4];
				hdr.mdlid = buffer[5];
				fprintf(out, "# Generated by BC-Convert. Input was DeviceID %d, ModelID %d", hdr.devid, hdr.mdlid);
#ifdef __WATCOMC__
				fprintf(out, "\r\n\r\n");
#else	/* __WATCOMC__ */
				fprintf(out, "\n\n");
#endif	/* __WATCOMC__ */
			} else {
				fprintf(stderr, "This is no Behringer SysEx dump. Terminating.\n");
				file_size = 0;
				stream_mode = 0;
			}
		}

		while(file_size > 0) {
			if(force_syx) {
				convert_text_to_sysex(in, out, &file_size, &buffer_free);
			} else {
				convert_sysex_to_text(in, out, &file_size, &buffer_free);
			}
			if(stream_mode && !feof(in)) {
				DEBUG("Stream mode, filling buffer (current size %d, free %d).\n", file_size, buffer_free);
				while(!feof(in) && buffer_free > 0) {
					tmp = fread(&(buffer[file_size]), 1, buffer_free, in);
					DEBUG("Read from input returned %d\n", tmp);
					if(tmp > 0) {
						buffer_free -= tmp;
						file_size += tmp;
					}
				}
			} else {
				if(file_size != 0) {
					fprintf(stderr, "For some reason, the input file couldn't be handled completely, a reason for\n");
					fprintf(stderr, "this could be a truncated input file, for SysEx files this is a sign for a\n");
					fprintf(stderr, "corrupt file transmission, for a text file, this could just be a missing\n");
					fprintf(stderr, "missing newline at the last line\n");
				}
				file_size = 0;
			}
		}
	}

	/* close all files we opened */
	DEBUG("Freeing buffer\n");
	if(buffer != NULL) free(buffer);
	DEBUG("Closing input file\n");
	if(in != NULL && infile != NULL) fclose(in);
	DEBUG("Closing output file\n");
	if(out != NULL && outfile != NULL) fclose(out);

	DEBUG("--- Terminating ---\n");

	return 0;
}
