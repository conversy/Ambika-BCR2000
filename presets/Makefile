all: CC

CC: Ambika-BCR-CC-p1-ch5.syx Ambika-BCR-CC-p2-ch6.syx Ambika-BCR-CC-p3-ch7.syx Ambika-BCR-CC-p4-ch8.syx Ambika-BCR-CC-p5-ch9.syx Ambika-BCR-CC-p6-ch10.syx
NRPN: Ambika-BCR-NRPN-p1-ch5.syx 

Ambika-BCR-CC-%.bcl: Ambika-BCR-CC.py
	python $^ $*

Ambika-BCR-CC-%.syx: Ambika-BCR-CC-%.bcl bc-convert
	./bc-convert -s -i $< -o $@

Ambika-BCR-NRPN-%.bcl: Ambika-BCR-NRPN.py
	python $^ $*

Ambika-BCR-NRPN-%.syx: Ambika-BCR-NRPN-%.bcl bc-convert
	./bc-convert -s -i $< -o $@


bc-convert: bc-convert.c
	cc $< -o $@

clean:
	rm -f *.bcl *~ bc-convert

distclean: clean
	rm -f *.syx

sysex_cmd := /Users/conversy/Downloads/portmedia-code-228/portmidi/trunk/pm_test/sysex

upload-to-bcr:
	cat sysex_cmd_list.txt | $(sysex_cmd)
