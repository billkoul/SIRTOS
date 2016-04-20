#ifndef ELF_H
#define ELF_H

#include <lib/types.h>
#include <fs/fs.h>

/* programHeader defines */
/* type */
#define PT_NULL 0             /* Unused */
#define PT_LOAD 1             /* Loadable segment (fileSize bytes) */
#define PT_DYNAMIC 2          /* Dynamic segment */
#define PT_INTERP 3           /* Size and location of null-ended path to interpreter */
#define PT_NOTE 4             /* Note segment */
#define PT_SHLIB 5            /* Reserved */
#define PT_PHDR 6             /* Size and location of program header */
#define PT_HIPROC 0x7fffffff  /* Reserved */
#define PT_LOPROC 0x70000000  /* Reserved */
/* !programHeader defines */

/* symEntry defines */
/* st_info */
#define ELF32_SYM_BIND(i) ((i)>>4)
#define ELF32_SYM_TYPE(i) ((i)&0xf)
#define ELF32_SYM_INFO(b,t) (((b)<<4)+((t)&0xf))
/* !symEntry defines*/

/* Section defines */
#define SHN_ABS 0xfff1
#define SHN_COMMON 0xfff2
/* !Section defines */

/* Dynamic defines */
/* d_tag */
#define DT_NULL 0
#define DT_NEEDED 1
#define DT_PLTRELSZ 2
#define DT_PLTGOT 3
#define DT_HASH 4
#define DT_STRTAB 5
#define DT_SYMTAB 6
#define DT_RELA 7
#define DT_RELASZ 8
#define DT_RELAENT 9
#define DT_STRSZ 10
#define DT_SYMENT 11
#define DT_INIT 12
#define DT_FINI 13
#define DT_SONAME 14
#define DT_RPATH 15
#define DT_SYMBOLIC 16
#define DT_REL 17
#define DT_RELSZ 18
#define DT_RELENT 19
#define DT_PLTREL 20
#define DT_DEBUG 21
#define DT_TEXTREL 22
#define DT_JMPREL 23
/* !Dynamic defines */

struct elfhdr
{
    unsigned  char    ident[16];   /* Elf identification */
    unsigned  short   type;        /* Type of ELF - 2 for Exectuable, 3 for Lib */
    unsigned  short   machine;     /* Architecture - 7 for x86 */
    unsigned  int     version;     /* 0 for invalid, 1 for valid */
    unsigned  int     entry;       /* Entry point - Point EIP here */
    unsigned  int     phoff;       /* Address of programHeader struct */
    unsigned  int     shoff;       /* Address of sectionHeader */
    unsigned  int     flags;       /* Machine flags - 0 for x86 */
    unsigned  short   ehsize;      /* elfHeader size */
    unsigned  short   phentsize;   /* Entry size from programHeader */
    unsigned  short   phnum;       /* Number of entries in programHeader */
    unsigned  short   shentsize;   /* Entry size of sectionHeader */
    unsigned  short   shnum;       /* Number of entries in sectionHeader */
    unsigned  short   shstrndx;    /* sectionHeader string index */
} __attribute__((packed));

typedef struct elfhdr elfHeader;

struct prgmhdr
{
    unsigned  int   type;          /* This contains the segment type */
    unsigned  int   offset;        /* Beginning of program */
    unsigned  int   vaddr;         /* Virtual addressm to relocate to */
    unsigned  int   paddr;         /* Physical address of above - Ignore */
    unsigned  int   fileSize;      /* Size of program */
    unsigned  int   memSize;       /* Memory size of program */
    unsigned  int   flags;         /* Flags relevent to segment */
    unsigned  int   alignment;     /* Alignment - 0 or 1 for none, Positive for align to value */
                                   /* NOTE: vaddr should equal offset */
} __attribute__((packed));

typedef struct prgmhdr programHeader;

struct sctnhdr
{
  unsigned int	  sh_name;		    /* Section name - index to string table */
  unsigned int	  sh_type;		    /* Section type */
  unsigned int	  sh_flags;		    /* Section flags */
  unsigned int	  sh_addr;		    /* Section virtual address at execution */
  unsigned int    sh_offset;		/* Section file offset */
  unsigned int	  sh_size;		    /* Section size in bytes */
  unsigned int	  sh_link;		    /* Link to another section - dependant on type of section */
  unsigned int	  sh_info;		    /* Additional section information */
  unsigned int	  sh_addralign;		/* Section alignment */
  unsigned int	  sh_entsize;		/* Entry size if section holds table */
} __attribute__((packed));

typedef struct sctnhdr sectionHeader;

struct syment
{
  unsigned int	  st_name;		    /* Symbol name - string table index */
  unsigned int	  st_value;		    /* Symbol value - absolute address */
                                    /* In executable/DLL objects this hold the 
                                       address of the function the symbol entry
                                       is referencing */
  unsigned int	  st_size;		    /* Size - bytes in the object */
  unsigned char	  st_type:4;		/* Types:
                                               None [0]
                                               Object [1] - data object, such as an array
                                               Function [2] - part of executable code
                                               Section [3] - associated with a section
                                               File [4] - something to do with linkers */
 unsigned char    st_bind:4;        /* Binding:
                                               Local [0] - exists only in the object file
                                               Global [1] - exists in all object files
                                               Weak [2] - see Global, but with less precedence */
  unsigned char   st_other;		    /* No meaning - zero'd */
  unsigned short  st_shndx;		    /* sectionHeader index */
} __attribute__((packed));

typedef struct syment symbolEntry;

struct noteent
{
  unsigned int  nt_namesz;        /* Name size */
  unsigned int  nt_descsz;        /* Descriptor size */
  unsigned int  nt_type;          /* Type */
  char*         nt_name[8];       /* NT_NAME - Data/text (4 byte aligned, null terminated) */
  /* NT_DESC - Descriptor */
} __attribute__((packed));

typedef struct noteent noteEntry;

struct dynent
{
  unsigned int d_tag; 
  union{
        unsigned int   d_val;        /* Value - various interpretations */
        unsigned int  d_ptr;         /* Pointer to function (virtual address) */
  } d_un;
} __attribute__((packed));

typedef struct dynent dynEntry;

extern int openModule_elf(char* name, int offset);
extern int relocModule_elf(elfHeader* header, int offset);
extern int load_elf(int inodenumber, int size);
extern void debug_sectionHeaders(elfHeader* header);

extern void debugSymbols(elfHeader* header, sectionHeader* sHeader);
u32int* getSymbolAddr(elfHeader* header, u8int* name);
void debug_dynamicEntries();
void debug_noteEntries(u32int* address);

#endif
