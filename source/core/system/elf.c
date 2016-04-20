#include <system/elf.h>

static u32int symtab_address, dynamic_address, shstrndx_address;

int openModule_elf(char* name, int offset)
{
     printf("[openModule_elf] Opening %s (ELF module)\n",name);
     
     elfHeader* header;
     
     /* Put header information into header struct */
     header = (elfHeader*) offset;
     
     /* Check for any errors */
     if(header->ident[0] != 0x7f || header->ident[1] != 'E' || header->ident[2] != 'L' || header->ident[3] != 'F')
     {
          printf("[openModule_elf] Error: not a valid ELF binary\n");
          return 1;
     }
     
     /* Bits */
     if(header->ident[4] != 1) //1 = 32, 2 = 64
     { 
          printf("[openModule_elf] Error: wrong architecture bit number\n");
          return 1;
     }
     
     /* Endian type */
     if(header->ident[5] != 1) //1 = little, 2 = big
     {
          printf("[openModule_elf] Error: wrong architecture endianess\n");
          return 1;
     }
     
     debug_sectionHeaders(header);
     
     if(relocModule_elf(header,offset) != 1)
     {
          printf("[openModule_elf] Error: relocation failed\n");
          return 1;
     }
     
     printf("[openModule_elf] Calling executable at 0x%x\n",header->entry);
     
     /* Call the program */    
     __asm__ __volatile__("call *%0" : : "a" (header->entry));
     
     return 0;
}


int relocModule_elf(elfHeader* header, int offset)
{
    int i;
    static programHeader* pHeader;
    //static noteEntry* note;
    
    printf("[relocModule_elf] %d Program Headers found.\n",header->phnum);
    
    for(i = 0; i < header->phnum; i++)
    {
          pHeader = (programHeader*) ((unsigned long) header + header->phoff + i * header->phentsize);
          
          /* Check if its loadable */
          switch(pHeader->type)
          {
          //case PT_NULL:
          //break;
          
          case PT_LOAD:
              printf("[relocModule_elf] Program header #%d Type:LOAD Addr:0x%x Size:0x%x!\n",i,pHeader->vaddr,pHeader->fileSize);
              //Map the virtual address to an area in the stack
              virtual_map_pages(pHeader->vaddr, pHeader->fileSize, 1, 1);
              
              /* Copy the program data to vaddr */
              memcpy((char*)pHeader->vaddr,(char*)offset+pHeader->offset, pHeader->fileSize);
              if(pHeader->fileSize < pHeader->memSize)
                   /* memset() the remaining memory with zeroes - stack */
                   memset((char *)pHeader->vaddr+pHeader->fileSize, 0, pHeader->memSize-pHeader->fileSize);
          break;
          
          case PT_DYNAMIC:
               printf("[relocModule_elf] Program header #%d Type:DYNAMIC Addr:0x%x Size:0x%x!\n",i,pHeader->vaddr,pHeader->fileSize);
              //I failed to understand what needs to be done here
          break;

          case PT_INTERP:
               printf("[relocModule_elf] Program header #%d Type:INTERP Addr:0x%x Size:0x%x!\n",i,pHeader->vaddr,pHeader->fileSize);
              
              //This is an external program that needs to be relocated and called.
              static unsigned char* interp;
              
              memcpy((char*)interp,(char*)offset+pHeader->offset, pHeader->fileSize);
              
              printf("\tInterpreter: \"");
              monitor_write(interp);
              printf("\"\n");
          break;
          
          case PT_NOTE:
               printf("[relocModule_elf] Program header #%d Type:NOTE Addr:0x%x Size:0x%x!\n",i,pHeader->vaddr,pHeader->fileSize);

               debug_noteEntries((u32int*)(offset+pHeader->offset));
          break;
          
          //case PT_SHLIB:
          //break;
          
          //case PT_PHDR:
          //break;
          
          default:
          break;
          }       
    }
    return 1;
}

void debug_sectionHeaders(elfHeader* header)
{
     int i;
     sectionHeader* sHeader = (sectionHeader*) ((unsigned long) header + header->shoff);
     
     shstrndx_address = (u32int)(header + sHeader[header->shstrndx].sh_offset) >> 8;
     printf("[debug_sectionHeaders] shstrndx_address: 0x%x\n",shstrndx_address);
     
     printf("[debug_sectionHeaders] Debugging started for %d Section headers\n",header->shnum);

     for(i = 0; i < header->shnum; i++)
     {    
          switch(sHeader[i].sh_type)
          {
             /*null*/
             case 0: 
                    printf("[debug_sectionHeaders] Null section found [#%d]\n",i);
             break;
             
             /*symtab - symbol table for linking*/
             case 2: 
                    printf("[debug_sectionHeaders] Symbol Table found [#%d]\n",i);
                    //symtab_address = (u32int)(header + sHeader[i].sh_offset);
                    //debugSymbols(header,sHeader);
             break;
             case 0x200:
                    printf("[debug_sectionHeaders] Symbol Table found [#%d]\n",i);
             break;
             
             /*strtab - string table -> debugging*/
             /*these are individual strings*/
             case 3: 
                    printf("[debug_sectionHeaders] String Table found [#%d]",i);
                    if(i == header->shstrndx)
                         printf(" (SHSTRNDX)");
                    printf("\n[debug_sectionHeaders] SH_SIZE: 0x%x [size]\n",sHeader[i].sh_size >> 8);
                    printf("[debug_sectionHeaders] SH_OFFSET: 0x%x SH_ADDR: 0x%x\n",sHeader[i].sh_offset >> 8,sHeader[i].sh_addr);
             break;
             case 0x300:
                    printf("[debug_sectionHeaders] String Table found [#%d]",i);
                    if(i == header->shstrndx)
                         printf(" (SHSTRNDX)");
                    printf("\n[debug_sectionHeaders] SH_SIZE: 0x%x [size]\n",sHeader[i].sh_size >> 8);
                    printf("[debug_sectionHeaders] SH_OFFSET: 0x%x SH_ADDR: 0x%x\n",sHeader[i].sh_offset >> 8,sHeader[i].sh_addr);
             break;
             
             /*hash - hash table required for dynamic linking*/
             case 5: 
                    printf("[debug_sectionHeaders] Hash Table found [#%d]\n",i);
                    printf("[debug_sectionHeaders] SH_LINK: 0x%x [strtab index] SH_INFO: 0x%x [0x0]\n",sHeader[i].sh_link >> 8,sHeader[i].sh_info >> 8);
             break;
             case 0x500:
                    printf("[debug_sectionHeaders] Hash Table found [#%d]\n",i);
                    printf("[debug_sectionHeaders] SH_LINK: 0x%x [strtab index] SH_INFO: 0x%x [0x0]\n",sHeader[i].sh_link >> 8,sHeader[i].sh_info >> 8);
             break;
             
             /*dynamic - holds information for dynamic linking*/
             case 6: 
                    printf("[debug_sectionHeaders] Dynamic section found [#%d]\n",i);
                    printf("[debug_sectionHeaders] SH_LINK: 0x%x [strtab index] SH_INFO: 0x%x [S0x0]\n",sHeader[i].sh_link >> 8,sHeader[i].sh_info >> 8);
                    //dynamic_address = (u32int)(header + sHeader[i].sh_offset) >> 8;
                    //debug_dynamicEntries();
             break;
             case 0x600:
                    printf("[debug_sectionHeaders] Dynamic section found [#%d]\n",i);
                    printf("[debug_sectionHeaders] SH_LINK: 0x%x [strtab index] SH_INFO: 0x%x [0x0]\n",sHeader[i].sh_link >> 8,sHeader[i].sh_info >> 8);
                    //dynamic_address = (u32int)(header + sHeader[i].sh_offset) >> 8;
                    //debug_dynamicEntries();
             break;
             
             /*note - extra information*/
             case 7:
                    printf("[debug_sectionHeaders] Note section found [#%d]\n",i);
                    printf("[debug_sectionHeaders] Note address: 0x%x (offset: 0x%x)\n",(header + sHeader[i].sh_offset),sHeader[i].sh_offset);
             break;
             case 0x700:
                    printf("[debug_sectionHeaders] Note section found [#%d]\n",i);
                    printf("[debug_sectionHeaders] Note address: 0x%x (offset: 0x%x)\n",(header + sHeader[i].sh_offset),sHeader[i].sh_offset);
             break;
             
             /*dynsym - dynamic symbol table for dynamic linking*/
             case 11: 
                    printf("[debug_sectionHeaders] Dynamic Symbol Table found [#%d]\n",i);
                    printf("[debug_sectionHeaders] SH_LINK: 0x%x [strtab index] SH_INFO: 0x%x\n",sHeader[i].sh_link >> 8,sHeader[i].sh_info >> 8);
             break;
             case 0xb00:
                    printf("[debug_sectionHeaders] Dynamic Symbol Table found [#%d]\n",i);
                    //printf("[debug_sectionHeaders] Name: %s\n",shstrndx_address + sHeader[i].sh_name);
                    printf("[debug_sectionHeaders] SH_LINK: 0x%x [strtab index] SH_INFO: 0x%x\n",sHeader[i].sh_link >> 8,sHeader[i].sh_info >> 8);
             break;
             
             default:
                     //printf("(0x%x) ",sHeader[i].sh_type,sHeader[i].sh_type);
                     //printf("[debug_sectionHeaders] Un-needed section [#%d]\n",i,sHeader[i].sh_type);
             break;
          }
     }
     printf("[debug_sectionHeaders] Debugging ended\n");
}

int load_elf(int inodenumber, int size)
{
     printf("[load_elf] Called for inode %d - %d bytes of data allocated\n",inodenumber,size);
     
     struct dirent *testmodule = 0;
     if((testmodule = readdir_fs(fs_root, inodenumber)) == 0)
                    return 1;
     else
     {
                    printf("[load_elf] Directory entry read correctly\n");
                    printf("[load_elf] Looking for \"%s\"\n",testmodule->name);
                    
                    fs_node_t *modulenode = finddir_fs(fs_root, testmodule->name);
                    
                    printf("[load_elf] Reading file\n");
                    
                    char* modulebuffer = (char*)kmalloc(size);
                    unsigned int modulesize = read_fs(modulenode, 0, size, modulebuffer);
                    
                    printf("[load_elf] Allocating space\n");
                    
                    unsigned int moduleptr = kmalloc(modulesize);
                    memcpy((void*)moduleptr,(void*) modulebuffer, modulesize);
                    
                    printf("[load_elf] Data moved into allocated space\n");
                    
                    if(openModule_elf(testmodule->name, moduleptr))
                         printf("[load_elf] openModule_elf returned with an error\n");
     }
     return 0;
}

void debug_noteEntries(u32int* address)
{
     printf("[debug_noteEntries] Starting NOTE analysis at 0x%x\n",address);
     
     noteEntry* Note;
     Note = (noteEntry*)address;
     
     printf("[debug_noteEntries] NT_NAMESZ: %d\n",Note->nt_namesz);
     printf("[debug_noteEntries] NT_DESCSZ: %d\n",Note->nt_descsz);
     printf("[debug_noteEntries] NT_TYPE: %d\n",Note->nt_type);
     
     //This is a bit of a hack - it only works for strings up to 8 bytes in length.
     //TODO: work out a way to actually get this working properly
     printf("[debug_noteEntries] NT_NAME: %s\n",Note->nt_name);
}

/////////////////////////////////////////////////////////////////////////////////    WIPs
void debug_dynamicEntries()
{
     printf("[debug_dynamicEntries] Starting _DYNAMIC analysis\n");
     dynEntry* _DYNAMIC;
     _DYNAMIC = (dynEntry*) dynamic_address;
     
     while(_DYNAMIC->d_tag != DT_NULL)
          printf("[debug_dynamicEntries] d_tag: %d d_val: %d d_ptr: 0x%x \n",_DYNAMIC->d_tag,_DYNAMIC->d_un.d_val,_DYNAMIC->d_un.d_ptr);
}

void debugSymbols(elfHeader* header, sectionHeader* sHeader)
{
     symbolEntry* symbol = (symbolEntry*)symtab_address;
     int i = 0;
     u8int* stringTable = 0;
     
     printf("[debugSymbols] Beginning symbol analysis\n",i);
     printf("[debugSymbols] %d symbols found\n",header->shnum);
     
     sectionHeader* tmpsheader = (sectionHeader*)((unsigned long) header + header->shoff + (sHeader->sh_link * header->shentsize));
     stringTable = (u8int*)(header + tmpsheader->sh_offset);
     
     while(i <= header->shnum)
     {
          if(symbol->st_name != 0)
          {    
               printf("[debugSymbols] Symbol #%d - %s\n",i,(stringTable + symbol->st_name));
          }
           
          i++;
          symbol = (symbolEntry*)(symtab_address + (i*header->shentsize));
     }
}

u32int* getSymbolAddr(elfHeader* header, u8int* name)
{
     //We need to get the first symbol table entry
     symbolEntry* symbol = (symbolEntry*)symtab_address;
     u32int* addr = (u32int*)-1;
     
     //Special relocation
     switch(symbol->st_shndx)
     {
          case SHN_ABS: 
                //This is the address of the function
                addr = (u32int*)symbol->st_value;
                return addr;
          break;
          case SHN_COMMON:
               //We cannot deal with this
               //compile with -fno-common
               printf("[getSymbolAddr] SHN_COMMON symbol found - exiting...\n");
               return addr;
          break;
     }
     
     //No special relocation type
     sectionHeader* tmp = (sectionHeader*)(header + header->shoff + (header->shentsize * symbol->st_shndx));
     addr = (u32int*)(symbol->st_value + (header + tmp->sh_offset));
     
     return addr;
}
