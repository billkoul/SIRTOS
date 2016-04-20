#ifndef BMP_H
#define BMP_H

#include <lib/types.h>

struct BMPheader
{
    unsigned short Type;     //Should be "BM"
    unsigned int Size;       //Size in DWORDS of file
    unsigned int Reserved;   //Reserved 1 and 2 (should be 0)
    unsigned int Offset;     //Data
    unsigned int HeaderSize; //Should be 40
    unsigned int Width;      //Width in px
    unsigned int Height;     //Height in px
    unsigned short Planes;   //Number of planes used
    unsigned short BitsPerPixel;      //BPP (1, 4, 8 or 24)
    unsigned int Compression;         //Should be 0
    unsigned int SizeImage;  //Size of image in bytes
    unsigned int XPixelsPerMeter;     //erm...? (usually 0)  
    unsigned int YPixelsPerMeter;     //... (usually 0)
    unsigned int ColorsUsed;          //Number of colours used
    unsigned int ColorsImportant;     //Important colours
} __attribute__((packed));

typedef struct BMPheader BMPheader_t;

#endif
