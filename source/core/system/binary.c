#include <fs/fs.h>

void execute_binary(unsigned int addr)
{
     	__asm__ __volatile__("call *%0" : : "a" (addr));
}

//Loads a flat binary into memory and executes it
void execute_module_binary(char* name, u32int size)
{
    	fs_node_t *modulenode = finddir_fs(fs_root, name);
    	char* modulebuffer = (char*)kmalloc(size);
    	unsigned int modulesize = read_fs(modulenode, 0, size, modulebuffer);
    	unsigned int moduleptr = kmalloc(modulesize);
    	memcpy((void*)moduleptr,(void*) modulebuffer, modulesize);
    	execute_binary(moduleptr);
}
