# bitten engine save files

## file format

to start out with, I'll show the code that defines the default save as of july 9th 2024
```c
enum saveFormat
{
  HEADER = 0,
  HEADERVERSION = 10,
  SETTINGS = 11,
  SAVEDXPOS = 12,
  SAVEDYPOS = 16,
};
unsigned char saveD[] = {
  'b', 'i', 't', 't', 'e', 'n', 's', 'a', 'v', 0x00, // header, last byte is for different games
  0x00, // version of save format, part of header, but we check this separately
  0b10000000, // first value: music, second value: fullscreen
  0x00, 0x00, 0x00, 0x00, // x position
  0x00, 0x00, 0x00, 0x00, // y position
};
unsigned int saveDlen = 20;
```
