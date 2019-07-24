header="""
# Ambika BCR part:%(part)d midichan:%(midichan)d 

$rev R1    # Start BCR configuration
$preset    # Start preset configuration
  .request on
  .tx $F0 $00 $21 $02 $00 $04 $16 %(part)d $00 $00 $F7     # LEARN output
  .init    # Clear current edit buffer
"""

misc="""
#-------------------------
# Encoder 1 - MIDI volume
#-------------------------
"""

encoder_cc_sav="""
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

encoder_cc="""
#-------------------------
# Encoder %(numenc)d - %(name)s
#-------------------------

$encoder %(numenc)d # Start encoder configuration
  .easypar CC %(midichan)d %(ccval)d 64 192 absolute
  .showvalue on
  .mode pan
  .resolution 24 48 96 512
  .default 128
"""

encoder_nrpn="""
#-------------------------
# Encoder %(numenc)d - %(name)s
#-------------------------

$encoder %(numenc)d # Start encoder configuration
  .easypar NRPN %(midichan)d %(NRPN)d 255 0 absolute/14
  .showvalue on
  .mode pan
  .resolution 24 48 96 512
  .default 128
  
"""

footer="""
$end       # End of BCR configuration
"""

def preset(midichan=-1, part=-2):

    content = header%{'part':part, 'midichan':midichan}

    # group 1
    content += encoder_nrpn%{'midichan':midichan, 'name': 'osc1 shape', 'numenc':1, 'NRPN':114} #, 'msb':1, 'lsb':115}
    #content += encoder_cc%{'name': 'osc1 shape', 'numenc': 1, 'midichan':midichan, 'ccval': 16}

#     content += encoder%{'name': 'osc1 param', 'numenc': 2, 'midichan':midichan, 'ccval': 17}
#     content += encoder%{'name': 'osc1 range', 'numenc': 3, 'midichan':midichan, 'ccval': 14}
#     content += encoder%{'name': 'osc1 tune' , 'numenc': 4, 'midichan':midichan, 'ccval': 15}

#     content += encoder%{'name': 'osc2 shape', 'numenc': 5, 'midichan':midichan, 'ccval': 18}
#     content += encoder%{'name': 'osc2 param', 'numenc': 6, 'midichan':midichan, 'ccval': 19}
#     content += encoder%{'name': 'osc2 range', 'numenc': 7, 'midichan':midichan, 'ccval': 20}
#     content += encoder%{'name': 'osc2 tune ', 'numenc': 8, 'midichan':midichan, 'ccval': 21}

#     # group 2
#     content += encoder%{'name': 'osc mix', 'numenc': 9, 'midichan':midichan, 'ccval': 22}
#     content += encoder%{'name': 'osc xmod', 'numenc': 10, 'midichan':midichan, 'ccval': 23}
#     content += encoder%{'name': 'osc xmod amnt', 'numenc': 11, 'midichan':midichan, 'ccval': 24}
#     content += encoder%{'name': 'noise level', 'numenc': 12, 'midichan':midichan, 'ccval': 27}

#     content += encoder%{'name': 'sub shape' , 'numenc': 13, 'midichan':midichan, 'ccval': 25}
#     content += encoder%{'name': 'sub level', 'numenc': 14, 'midichan':midichan, 'ccval': 26}

#     numenc=33

#     content += encoder%{'midichan':midichan, 'numenc': numenc, 'name': 'env1 A', 'ccval': 73}; numenc+=1;
#     content += encoder%{'midichan':midichan, 'numenc': numenc, 'name': 'env1 D', 'ccval': 75}; numenc+=1;
#     content += encoder%{'midichan':midichan, 'numenc': numenc, 'name': 'env1 S', 'ccval': 70}; numenc+=1;
#     content += encoder%{'midichan':midichan, 'numenc': numenc, 'name': 'env1 R', 'ccval': 72}; numenc+=1;

#     content += encoder%{'midichan':midichan, 'numenc': numenc, 'name': 'LFO1 rate', 'ccval': 45}; numenc+=1;
#     content += encoder%{'midichan':midichan, 'numenc': numenc, 'name': 'LFO1 wave', 'ccval': 46}; numenc+=1;
#     content += encoder%{'midichan':midichan, 'numenc': numenc, 'name': 'LFO1 trig', 'ccval': 44}; numenc+=1;

#     #cutoff
#     content += encoder%{'midichan':midichan, 'numenc': 40, 'name': 'cutoff', 'ccval': 74}; numenc+=1;

#     numenc=41

#     content += encoder%{'midichan':midichan, 'numenc': numenc, 'name': 'env2 A', 'ccval': 81}; numenc+=1;
#     content += encoder%{'midichan':midichan, 'numenc': numenc, 'name': 'env2 D', 'ccval': 83}; numenc+=1;
#     content += encoder%{'midichan':midichan, 'numenc': numenc, 'name': 'env2 S', 'ccval': 78}; numenc+=1;
#     content += encoder%{'midichan':midichan, 'numenc': numenc, 'name': 'env2 R', 'ccval': 80}; numenc+=1;

#     content += encoder%{'midichan':midichan, 'numenc': numenc, 'name': 'LFO2 rate', 'ccval': 53}; numenc+=1;
#     content += encoder%{'midichan':midichan, 'numenc': numenc, 'name': 'LFO2 wave', 'ccval': 54}; numenc+=1;
#     content += encoder%{'midichan':midichan, 'numenc': numenc, 'name': 'LFO2 trig', 'ccval': 52}; numenc+=1;

#     #res
#     content += encoder%{'midichan':midichan, 'numenc': 48, 'name': 'res', 'ccval': 71}; numenc+=1;

#     numenc=49

#     content += encoder%{'midichan':midichan, 'numenc': numenc, 'name': 'env3 A', 'ccval': 89}; numenc+=1;
#     content += encoder%{'midichan':midichan, 'numenc': numenc, 'name': 'env3 D', 'ccval': 91}; numenc+=1;
#     content += encoder%{'midichan':midichan, 'numenc': numenc, 'name': 'env3 S', 'ccval': 86}; numenc+=1;
#     content += encoder%{'midichan':midichan, 'numenc': numenc, 'name': 'env3 R', 'ccval': 88}; numenc+=1;

#     content += encoder%{'midichan':midichan, 'numenc': numenc, 'name': 'LFO3 rate', 'ccval': 61}; numenc+=1;
#     content += encoder%{'midichan':midichan, 'numenc': numenc, 'name': 'LFO3 wave', 'ccval': 62}; numenc+=1;
#     content += encoder%{'midichan':midichan, 'numenc': numenc, 'name': 'LFO3 trig', 'ccval': 60}; numenc+=1;

    content += footer

    filename="Ambika-BCR-NRPN-p%d-ch%d.bcl"%(part,midichan)
    print ('generating '+filename)
    open(filename,'w').write(content)

if __name__=='__main__':
    import sys
    p_ch = sys.argv[1].split('-')
    p=int(p_ch[0][1:])
    ch=int(p_ch[1][2:])
    preset(part=p, midichan=ch)
