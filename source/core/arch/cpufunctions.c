// CPU specific functions for x86
#include <system.h>
#include <gui/window.h>
#include <gui/gfx.h>

extern bool graphical_mode;
extern void destroy_info_window();

unsigned char readCMOS(unsigned char addr)
{
   unsigned char ret;
   outb(0x70,addr);
   __asm__ __volatile__ ("jmp 1f; 1: jmp 1f;1:");
   ret = inb(0x71);
   __asm__ __volatile__ ("jmp 1f; 1: jmp 1f;1:");
   return ret;
}

void writeCMOS(unsigned char addr, unsigned int value)
{
   outb(0x70, addr);
   __asm__ __volatile__ ("jmp 1f; 1: jmp 1f;1:");
   outb(0x71, value);
   __asm__ __volatile__ ("jmp 1f; 1: jmp 1f;1:");
}

void reboot()
{
    outb (0x64, 0xFE); // send reboot command
}

void sti()
{
     __asm__ __volatile__ ("sti");
}

void cli()
{
     __asm__ __volatile__ ("cli");
}

void halt()
{
     __asm__ __volatile__ ("hlt");
}

//---------------------------------------------------------------------------------------------
//---------- code for shutdown writen by Napalm and Falkman ------------
uint8_t crc8(const void *vptr, int len)
{
	const uint8_t *data = vptr;
	unsigned crc = 0;
	int i, j;
	for (j = len; j; j--, data++) {
		crc ^= (*data << 8);
		for(i = 8; i; i--) {
			if (crc & 0x8000)
				crc ^= (0x1070 << 3);
			crc <<= 1;
		}
	}
	return (uint8_t)(crc >> 8);
}

#define SLP_EN  (1 << 13)
void shutdown() 
{
    unsigned int i, j, len, count, found, *ptr, *rsdp = 0, *rsdt = 0, *facp = 0, *dsdt = 0;
    unsigned char *data, slp_typ[2];
     
    // find acpi RSDP table pointer
    for(ptr = (unsigned int *)0x000E0000; ptr < (unsigned int *)0x000FFFFF; ptr++){
        if(*ptr == ' DSR' && *(ptr + 1) == ' RTP'){ // "RSD PTR "
            if(crc8((unsigned char *)ptr, 20)) continue;
            rsdp = ptr;
            break;
        }
    }
    if(!rsdp) goto haltonly;
     
    // find RSDT table pointer
    ptr = (unsigned int *)*(ptr + 4);
    if(crc8((unsigned char *)ptr, *(ptr + 1))){
	if(!graphical_mode)
        	printf("Error: RSDT checksum mismatch.\n");
        goto haltonly;
    }
    rsdt  = ptr;
    count = (*(ptr + 1) - 36) / 4;
     
    // find FACP table pointer
    ptr += 9; // skip RSDT entries
    for(i = 0; i < count; i++){
        for(j = 0; j < 24; j++){
            if(*(unsigned int *)*ptr == 'PCAF'){ // "FACP"
                facp = (unsigned int *)*ptr;
                i = count;
                break;
            }
        }
    }
    if(!facp){
	if(!graphical_mode)
        	printf("Error: Could not find FACP table.\n");
        goto haltonly;
    }
    if(crc8((unsigned char *)facp, *(facp + 1))){
	if(!graphical_mode)
        	printf("Error: FACP checksum mismatch.\n");
        goto haltonly;
    }
     
    // find DSDT table pointer
    ptr = (unsigned int *)*(facp+10); // DSDT address
    if(*ptr != 'TDSD'){ // "DSDT"
	if(!graphical_mode)
        	printf("Error: Could not find DSDT table.\n");
        goto haltonly;
    }
    if(crc8((unsigned char *)ptr, *(ptr + 1))){
	if(!graphical_mode)        
		printf("Error: DSDT checksum mistmatch.\n");
        goto haltonly;
    }
    dsdt = ptr;
     
    // Search DSDT byte-code for ACPI _S5 signature
    found = 0;
    len   = *(dsdt + 1) - 36;
    data  = (unsigned char *)(dsdt + 36);
    while(len--){
        if((*(unsigned int *)data & 0x00FFFFFF) == 0x0035535F){ // "_S5"
            data += 4;
            if(*data == 0x12){ // 0x012 = package opcode
                data += 3; // 0x0A = 8bit integer opcode
                slp_typ[0] = (*data == 0x0A) ? *++data : *data;
                data++;
                slp_typ[1] = (*data == 0x0A) ? *++data : *data;
                found = 1;
            }
            break;
        }
        data++;
    }
    if(!found) goto haltonly;
     
    	// execute the actual shutdown and power-off
    	outw(*(facp + 16), slp_typ[0] | SLP_EN);     // FACP[64] = PM1a_CNT_BLK
    	if(*(facp + 17))
        	outw(*(facp + 17), slp_typ[1] | SLP_EN); // FACP[68] = PM1b_CNT_BLK
	if(!graphical_mode)
    		printf("Shutting down...\n");
    	halt();
     
	haltonly:
		if(!graphical_mode)
    			printf("It is now safe to turn-off your computer.\n");
		else
		{
			int info = create_info_window(300, 300, "!", "It is now safe to turn-off your computer", COLOR_BACKGROUND, COLOR_BLACK, 1);
			sleep(3000);
			destroy_info_window(info);
		}
    		halt();
     
}
//-------------------------------------------------------------------------
