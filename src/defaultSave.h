unsigned char saveD[] = {
  'b', 'i', 't', 't', 'e', 'n', 's', 'a', 'v', 0xc4, // header
  0x01, // version
  0b10000000, // first value: music, second value: fullscreen
  0xa1, 0x01, // location (x,y)
  0x0a
};
unsigned int saveDlen = 15;
