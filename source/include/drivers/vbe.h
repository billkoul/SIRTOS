// Written by: Robert C. Pendleton
// Copyright 1993 by Robert C. Pendleton
// Non-commercial use by individuals is permitted.

#ifndef _VBE_H_
#define _VBE_H_

#include <lib/types.h>

typedef struct
{
    u16int red;
    u16int green;
    u16int blue;
} vgaColor;

// VBE modes.
// Note that ALL standard VGA video modes can be used
// with the vbe functions.

struct
{
    char    VESASignature[4];       /* 'VESA' 4 byte signature          */
    short   VESAVersion;            /* VBE version number               */
    char    *OEMStringPtr;      /* Pointer to OEM string            */
    long    Capabilities;           /* Capabilities of video card       */
    unsigned *VideoModePtr;     /* Pointer to supported modes       */
    short   TotalMemory;            /* Number of 64kb memory blocks     */
    char    reserved[236];          /* Pad to 256 byte block size       */
} VbeInfoBlock;
/* SuperVGA mode information block */

struct
{
   u16int ModeAttributes;
   u8int  WinAAttributes;
   u8int  WinBAttributes;
   u16int WinGranularity;
   u16int WinSize;
   u16int WinASegment;
   u16int WinBSegment;
   void  *WinFuncPtr;
   u16int BytesPerScanLine;
   u16int XRes;
   u16int YRes;
   u8int  Xu8intSize;
   u8int  Yu8intSize;
   u8int  NumberOfPlanes;
   u8int  BitsPerPixel;
   u8int  NumberOfBanks;
   u8int  MemoryModel;
   u8int  BankSize;
   u8int  NumberOfImagePages;
   u8int  res1;
   u8int  RedMaskSize;
   u8int  RedFieldPosition;
   u8int  GreenMaskSize;
   u8int  GreenFieldPosition;
   u8int  BlueMaskSize;
   u8int  BlueFieldPosition;
   u8int  RsvedMaskSize;
   u8int  RsvedFieldPosition;
   //VBE 2.0
   u32int *PhysBasePtr;
   u32int OffScreenMemOffset;
   u16int OffScreenMemSize;
   //VBE 2.1
   u16int LinbytesPerScanLine;
   u8int  BankNumberOfImagePages;
   u8int  LinNumberOfImagePages;
   u8int  LinRedMaskSize;
   u8int  LinRedFieldPosition;
   u8int  LingreenMaskSize;
   u8int  LinGreenFieldPosition;
   u8int  LinBlueMaskSize;
   u8int  LinBlueFieldPosition;
   u8int  LinRsvdMaskSize;
   u8int  LinRsvdFieldPosition;
   u8int  res2[194];
} ModeInfoBlock;

typedef enum
{
    memPL       = 3,                /* Planar memory model              */
    memPK       = 4,                /* Packed pixel memory model        */
    memRGB      = 6,                /* Direct color RGB memory model    */
    memYUV      = 7,                /* Direct color YUV memory model    */
} memModels;
#endif
