// --------------------------------------------------------------
//  PCI driver/scanner (most of the code can be found in OSDEV
// --------------------------------------------------------------
#include <lib/types.h>
#include <system/pci.h>

PCI_BIOS_t *pci_bios;

static u32int pci_read(int bus, int dev, int func, int reg);
static PCI_Device_t* pci_readdevice(int bus, int dev, int func);
unsigned pci_find_bios();
static PCI_Args_t* pci_fill_struct(PCI_Device_t* dev);

char* devices[100];
int count = 0;

//Reads DWORD config register
static u32int pci_read(int bus, int dev, int func, int reg)
{
  outl(0xCF8,
	    ((unsigned long) 0x80000000 | (bus << 16) | (dev << 11) |
	     (func << 8) | reg));
  return inl(0xCFC);
}


PCI_Device_t* ret;

static PCI_Device_t* pci_readdevice(int bus, int dev, int func)
{ 
  int place, total = sizeof(PCI_Device_t) / sizeof(unsigned long);

  for (place = 0; place < total; place++)
    ((u32int*)ret)[place] =  pci_read(bus, dev, func, (place * sizeof (unsigned long)));
  
  return ret;
}

unsigned pci_find_bios()
{
	unsigned long addr;
	unsigned char crc;
	int i;

  	for (addr = 0xE0000; addr < 0xFFFFF; addr += 0x10)
    	{
      		pci_bios = (PCI_BIOS_t *) addr;
      		if (pci_bios->Magic == 0x5F32335F)
		{
	  		for (i = 0, crc = 0; i < (pci_bios->Pages * 16); i++)
	    		crc += *((unsigned char *) (addr + i));
	  		if (crc == 0)
	    		{
            			//found bios
          			//printf("[pci_find_bios] PCI BIOS found at 0x%x\n",addr);
	      			return addr;
	    		}
		}
    	}
  	//no bios
  	//printf("[pci_find_bios] PCI BIOS not found\n");
  	return 0;
}

int init_pci()
{    

     //Check if there is a bios
     if(pci_find_bios())
     {
          //printf("\nPCI not found...         ");
          return 0;
     }
     
     ret = (PCI_Device_t*)kmalloc(sizeof(PCI_Device_t));

     int bus, device, function;
     PCI_Device_t *tmp;
     
     for (bus = 0; bus <= 0xFF; bus++)
     {
          for (device = 0; device < 32; device++)
	      {
               for (function = 0; function < 8; function++)
               {
	                tmp = pci_readdevice(bus, device, function);
	                if((tmp->VendorID == 0) || (tmp->VendorID == 0xFFFF) || (tmp->DeviceID == 0xFFFF)) {}
	                else{ 
                 
                         	count++; 
                         
                                   switch(tmp->ClassCode)
                                   {
                                        //Pre-PCI Items
                                        case 0x00:
                                             if(tmp->SubClass == 0x01) //VGA
                                             {
						  devices[count] = "VGA";	
                                                  break;
                                             }
                                        break;
                                        
                                        //Mass storage devices
                                        case 0x01:
                                             if(tmp->SubClass == 0x01) //IDE (And possibly CD?)
                                             {
						  devices[count] = "ATA (IDE)";
						  //ide_initialize(tmp->BAR0,tmp->BAR1, tmp->BAR2, tmp->BAR3, tmp->BAR4);
                                                  break;
                                             }
                                             
                                             if(tmp->SubClass == 0x02) //Floppy
                                             {
						  devices[count] = "Floppy";
                                                  break;
                                             }
                                                  
                                             if(tmp->SubClass == 0x80)
                                             {
						  devices[count] = "Mass Storage Device Controller";
                                                  break;
                                             }
                                        break;
                                        
                                        //NIC's
                                        case 0x02:
                                             if(tmp->SubClass == 0x00)
                                             {
						  devices[count] = "Ethernet";
                                                  
                                                  switch(tmp->VendorID)
                                                  {
                                                       /* Realtek RTL8139 and clones */
                                                       case 0x10EC:
                                                            if(tmp->DeviceID == 0x8139 || tmp->DeviceID == 0x8138)
                                                            {
								 devices[count] = "Realtek RTL8139";
                                                            }
                                                       break;
                                                       
                                                       case 0x1113:
                                                            if(tmp->DeviceID == 0x1211)
                                                            {
								 devices[count] = "SMC1211TX EZCard 10/100 (RTL8139 clone)";
                                                            }
                                                       break;
                                                       
                                                       case 0x1500:
                                                            if(tmp->DeviceID == 0x1360)
                                                            {
								 devices[count] = "Delta Electronics 8139 10/100BaseTX (RTL8139 clone";
                                                            }
                                                       break;
                                                       
                                                       case 0x4033:
                                                            if(tmp->DeviceID == 0x1360)
                                                            {
								 devices[count] = "Addtron Technology 8139 10/100BaseTX (RTL8139 clone)";
                                                            }
                                                       break;
                                                       
                                                       case 0x1186:
                                                            if(tmp->DeviceID == 0x1300)
                                                            {
								 devices[count] = "D-Link DFE-538TX (RTL8139 clone)";
                                                            }
                                                       break;
                                                       /* ------------------- */

                                                       
                                                       default:
							    devices[count] = "Unknown Ethernet Card";
                                                       break;
                                                  }
                                                  break;
                                             }
                                             
                                             if(tmp->SubClass == 0x80)
                                             {
						  devices[count] = "Network Controller";
                                                  break;
                                             }
                                        break;
                                        
                                        //Video
                                        case 0x03:
                                             if(tmp->SubClass == 0x00)
                                             {
						  devices[count] = "VGA";
                                                  break;
                                             }
                                                  
                                             if(tmp->SubClass == 0x01)
                                             {
						  devices[count] = "XGA/VESA";
                                                  break;
                                             }
                                        break;
                                        
                                        //Bridge Devices
                                        case 0x06:                                                  
                                             if(tmp->SubClass == 0x00)
                                             {
						  devices[count] = "Host/PCI Bridge";
                                                  break;
                                             }
                                             
                                             if(tmp->SubClass == 0x01)
                                             {
						  devices[count] = "PCI/ISA Bridge";
                                                  break;
                                             }
                                             
                                             if(tmp->SubClass == 0x04)
                                             {
						  devices[count] = "PCI/PCI Bridge";
                                                  break;
                                             }
                                        break;
                                        
                                        //Other
                                        case 0x0C:
                                             if(tmp->SubClass == 0x03)
                                             {
						  devices[count] = "USB Controller";
                                                  break;
                                             }
                                        break;
                                        
                                        default:
						devices[count] = "Unknown Device";
                                        break;         
                                   }
                                   
                                        //printf("[init_pci] VID:0x%x DID:0x%x\n",tmp->VendorID,tmp->DeviceID);
                                        //printf("[init_pci] CC:0x%x SCC:0x%x\n",tmp->ClassCode,tmp->SubClass);
                                        //printf("[init_pci] BAR0:0x%x BAR1:0x%x BAR2:0x%x\n",tmp->BAR0,tmp->BAR1,tmp->BAR2);
                                        //printf("[init_pci] BAR3:0x%x BAR4:0x%x BAR5:0x%x\n",tmp->BAR3,tmp->BAR4,tmp->BAR5);
                                        //printf("[init_pci] IRQ:%d\n",tmp->IRQ);

                    		}
               		}
	      }
     }
     kfree(ret);
     return 1;
}
