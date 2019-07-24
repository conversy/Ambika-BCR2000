header="""
# Ambika BCR part:%(part)d midichan:%(midichan)d 

$rev R1    # Start BCR configuration
$preset    # Start preset configuration
  .request on
  .tx $F0 $00 $21 $02 $00 $04 $16 %(part)d $00 $00 $F7     # LEARN output
  .init    # Clear current edit buffer
"""

encoder="""
#-------------------------
# Encoder %(numenc)d - %(name)s
#-------------------------

$encoder %(numenc)d # Start encoder configuration
  .easypar CC %(midichan)d %(ccval)d 0 127 absolute
  .showvalue on
  .mode 12dot/off
  .resolution 96 96 96 96
  .default 100
"""

footer="""
$end       # End of BCR configuration
"""

def preset(midichan=-1, part=-2):

    content = header%{'part':part, 'midichan':midichan}

    # group 1
    numenc = 1

    content += encoder%{'name': 'osc1 wave', 'ccval': 16, 'max': 42, 'numenc': numenc, 'midichan': midichan}; numenc+=1;
    content += encoder%{'name': 'osc1 param', 'ccval': 17, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;
    content += encoder%{'name': 'osc1 range', 'ccval': 14, 'max': 48, 'numenc': numenc, 'midichan': midichan}; numenc+=1;
    content += encoder%{'name': 'osc1 tune' , 'ccval': 15, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;

    content += encoder%{'name': 'osc2 wave', 'ccval': 18, 'max': 42, 'numenc': numenc, 'midichan': midichan}; numenc+=1;
    content += encoder%{'name': 'osc2 param', 'ccval': 19, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;
    content += encoder%{'name': 'osc2 range', 'ccval': 20, 'max': 48, 'numenc': numenc, 'midichan': midichan}; numenc+=1;
    content += encoder%{'name': 'osc2 tune ', 'ccval': 21, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;

    # group 2
    numenc = 9
    
    content += encoder%{'name': 'osc mix', 'ccval': 22, 'max': 37, 'numenc': numenc, 'midichan': midichan}; numenc+=1;
    content += encoder%{'name': 'noise level', 'ccval': 27, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;
    content += encoder%{'name': 'sub level', 'ccval': 26, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;
    content += encoder%{'name': 'sub wave' , 'ccval': 25, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;
    content += encoder%{'name': 'osc xmod', 'ccval': 23, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;
    content += encoder%{'name': 'osc xmod amnt', 'ccval': 24, 'max': 48, 'numenc': numenc, 'midichan': midichan}; numenc+=1;
    content += encoder%{'name': 'fuzz' , 'ccval': 12, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;
    content += encoder%{'name': 'crush' , 'ccval': 13, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;

    # first row
    numenc=33

    content += encoder%{'name': 'env1 A', 'ccval': 73, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;
    content += encoder%{'name': 'env1 D', 'ccval': 75, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;
    content += encoder%{'name': 'env1 S', 'ccval': 70, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;
    content += encoder%{'name': 'env1 R', 'ccval': 72, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;

    content += encoder%{'name': 'LFO1 rate', 'ccval': 45, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;
    content += encoder%{'name': 'LFO1 wave', 'ccval': 46, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;
    content += encoder%{'name': 'LFO1 trig', 'ccval': 44, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;

    #cutoff freq
    content += encoder%{'midichan': midichan, 'numenc': 40, 'name': 'cutoff', 'ccval': 74, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;

    # second row
    numenc=41

    content += encoder%{'name': 'env2 A', 'ccval': 81, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;
    content += encoder%{'name': 'env2 D', 'ccval': 83, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;
    content += encoder%{'name': 'env2 S', 'ccval': 78, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;
    content += encoder%{'name': 'env2 R', 'ccval': 80, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;

    content += encoder%{'name': 'LFO2 rate', 'ccval': 53, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;
    content += encoder%{'name': 'LFO2 wave', 'ccval': 54, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;
    content += encoder%{'name': 'LFO2 trig', 'ccval': 52, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;

    #resonance
    content += encoder%{'name': 'res', 'ccval': 71, 'max': 64, 'numenc': numenc, 'midichan': midichan}; numenc+=1;

    # third row
    numenc=49

    content += encoder%{'name': 'env3 A', 'ccval': 89, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;
    content += encoder%{'name': 'env3 D', 'ccval': 91, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;
    content += encoder%{'name': 'env3 S', 'ccval': 86, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;
    content += encoder%{'name': 'env3 R', 'ccval': 88, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;

    content += encoder%{'name': 'LFO3 rate', 'ccval': 61, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;
    content += encoder%{'name': 'LFO3 wave', 'ccval': 62, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;
    content += encoder%{'name': 'LFO3 trig', 'ccval': 60, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;

    content = content%{'midichan': midichan, 'max': 127, 'numenc': numenc}

    # try again
    #content += encoder%{'name': 'osc1 param', 'ccval': 17, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;
    #content += encoder%{'name': 'osc1 wave', 'ccval': 16, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;

    #??
    #content += encoder%{'midichan': midichan, 'numenc': 57, 'name': 'res', 'ccval': 71, 'max': 127, 'numenc': numenc, 'midichan': midichan}; numenc+=1;

    content += footer

    filename="Ambika-BCR-CC-p%d-ch%d.bcl"%(part,midichan)
    print ('generating '+filename)
    open(filename,'w').write(content)

if __name__=='__main__':
    import sys
    p_ch = sys.argv[1].split('-')
    p=int(p_ch[0][1:])
    ch=int(p_ch[1][2:])
    print(p,ch)
    preset(part=p, midichan=ch)
