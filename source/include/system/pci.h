#ifndef PCI_H
#define PCI_H

typedef struct
{
unsigned long Magic;
unsigned long EntryPoint;
unsigned char Version;
unsigned char Pages;
unsigned char CRC;
} PCI_BIOS_t;

typedef struct
{
unsigned short VendorID;  
unsigned short DeviceID;
unsigned short CommandReg;
unsigned short StatusReg;
unsigned char RevisionID;
unsigned char ProgIF;
unsigned char SubClass;
unsigned char ClassCode;
unsigned char CachelineSize;
unsigned char Latency;
unsigned char HeaderType;
unsigned char BIST;

unsigned int BAR0; //Base Address Register
unsigned int BAR1;
unsigned int BAR2;
unsigned int BAR3;
unsigned int BAR4;
unsigned int BAR5;
unsigned int CardbusCISPtr;
unsigned short SubVendorID; //Subsystem
unsigned short SubDeviceID;
unsigned int ExRomAddress; //Expansion ROM
unsigned int Reserved1;
unsigned int Reserved2;
unsigned char IRQ; //IRQ number
unsigned char PIN; //IRQ PIN number
unsigned char MinGrant;
unsigned char MaxLatency;
} PCI_Device_t;

typedef struct
{
unsigned short VendorID;  
unsigned short DeviceID;
unsigned int BAR0;
unsigned int BAR1;
unsigned int BAR2;
unsigned int BAR3;
unsigned int BAR4;
unsigned int BAR5;
unsigned char IRQ;
} PCI_Args_t;

// I/O Ports (32 bit)
#define PCI_CONFIG 0xCF8
#define PCI_DATA 0xCFC

#endif
