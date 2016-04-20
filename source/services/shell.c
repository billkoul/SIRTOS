/* Copyright (c) 2015, 2016 All Right Reserved, Vasileios Kouliaridis

THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

	SHELL
*/
#include <initrd/initrd.h>
#include <arch/common.h>
#include <arch/task.h>
#include <arch/asm.h>
#include <system/version.h>
#include <system/user.h>
#include <gui/screen.h>

extern user_t current_user;
extern u32int memsize;
extern unsigned char second;
extern unsigned char minute;
extern unsigned char hour;
extern unsigned char day;
extern unsigned char month;
extern unsigned int year;
extern task_t *currentTask;


typedef struct shell_entry
{
	uint8_t name[16];
	uint8_t desc[50];
	void (*function)(char *args);
} shell_entry_t;

shell_entry_t *shell_entries;
int sidx = 0;
bool shell_running = false;
int shell_length = 1;
char *current_dir = "Shell";
char *username = "guest";
extern int cycle;

//functions
void print_output(void)
{
	username = current_user.name;
        printf("%s@%s $>", username, current_dir);
}

void parse_input(char *input)
{
	int i;
	if(input != "")
	{
	char *name = split(input, ' ', 0);
	char *args = split(input, ' ', 1);

	for (i = 0; i < sidx; i++)
	{
		if (!strcmp(strtolower(name), shell_entries[i].name))
		{
			shell_entries[i].function(args);
			print_output();
			return;
		} 
	}

	printf("Shell: %s is not a valid command\n", name);
	print_output();
	}
}

void add_entry(uint8_t name[16], uint8_t desc[64], void (*function)(char *args))
{
	shell_entries[sidx].function = function;
	strncpy(shell_entries[sidx].name, name, strlen(name));
	strncpy(shell_entries[sidx].desc, desc, strlen(desc));
	sidx++;
}

void help(void)
{
	cli();
	printf("Commands:\n");
	int i;
	for (i = 0; i < sidx; i++)
	{
		printf("%s\t%s\n", shell_entries[i].name, shell_entries[i].desc);

	}
	sti();
}


void sh_echo(char *args)	{ printf("%s\n", args); }
void sh_date(char *args)	
{ 
	printf("%d:%d:%d %d/%d/%d\n",hour, minute, second, day, month, year);  
}
void sh_memory(void)		{ printf("%dMB / %dMB\n",get_memory_usage()/1024/1024, ((memsize/1024)/1024)); }
void sh_version(void)		{ printf("ShoRTOS %s built on %s at %s.\n",OS_VERSION,OS_BUILD_DATE,OS_BUILD_TIME); }
void sh_exit()		    	{ shell_running = false; exit(); }

void sh_ls()
{
	int i = 0;
	struct dirent *node = 0;
	while ( (node = readdir_fs(fs_root, i)) != 0)
	{
 		printf(node->name);
  		fs_node_t *fsnode = finddir_fs(fs_root, node->name);
	
  		if ((fsnode->flags&0x7) == FS_DIRECTORY)
    			printf(" (directory)\n");
		else
			printf("\n");
  		i++;
	} 
}

void sh_te(char *args)
{
	fs_node_t *fsnode = finddir_fs(fs_root, args);
     	char buf[256];
     	u32int sz = read_fs(fsnode, 0, 256, buf);
	if(sz == 0)
		printf("Error - File not found!\n");
	else
	{
     		for (int j = 0; j < sz; j++)
    			printf("%c", buf[j]);
	}
}

void sh_user()
{
	printf("Username: %s \n", current_user.name);
	printf("Privileges: %s \n", get_privileges());
	printf("Password: %s \n", current_user.password); //debug
	return;
}

void sh_login(char *args)
{
	user_login(args);
}

void sh_kill(char *args)
{
	if(kill_task(abs((int)args)))
        printf("Task %d was not found\n",abs((int)args));
}

void process0()
{
	for(int i=0;i<3;i++){}
	if(cycle - currentTask->cycle > currentTask->deadline)
		printf("[!%d]", getpid());
	else
		printf("[%d]", getpid());
	exit();
}

void process1()
{
	for(int i=0;i<3;i++)
	{
		if(cycle - currentTask->cycle > currentTask->deadline)
			printf("[!%d]", getpid());
		else
			printf("[%d]", getpid());
	}
	exit();
}

void process2()
{
	for(int i=0;i<3;i++)
	{
		if(cycle - currentTask->cycle > currentTask->deadline)
			printf("[!%d]", getpid());
		else
			printf("[%d]", getpid());
		sleep(0);
	}
	exit();
}

void sh_proc()
{
	start_task(process0, 0, 100, 50);
}

void sh_ptest1()
{
	printf("Each of two process has same time to run but different deadlines\n");
	start_task(process2, 0, deadline_MEDIUM, resources_HIGH);
	start_task(process2, 0, deadline_LOW, resources_HIGH); //den exei deadline	
}

void sh_ptest2()
{
	printf("All processes have the same deadlines but different run times\n");
	start_task(process2, 0, deadline_MEDIUM, resources_AUTO);
	start_task(process2, 0, deadline_MEDIUM, resources_AUTO);
	start_task(process2, 0, deadline_MEDIUM, resources_AUTO);
}

void sh_ptest3()
{
	printf("Each of two process has a sleep() command to make them miss deadlines\n");
	start_task(process2, 0, deadline_LOW, resources_HIGH);
	start_task(process2, 0, deadline_MEDIUM, resources_HIGH);
}

void shutd()
{
	shutdown();
}

void init_shell()
{
	username = get_user();
	
	char* command = "";
	
	printf("For shell information, type \"help\"\n");
	shell_entries = (shell_entry_t*) kmalloc((sizeof(shell_entry_t) * 32));
	add_entry("help", "- Show shell commands", help);
	add_entry("ls", "- List files    ", sh_ls);
	add_entry("te", "- Print file contents (te <filaname)", sh_te);	
	add_entry("user", "- Print username and privileges", sh_user);
	add_entry("login", "- Login as different user (login <user>)", sh_login);
	add_entry("date", "- Current date       ", sh_date);
	add_entry("echo", "- Echos arguments (echo <...>)", sh_echo);
	add_entry("memu", "- Memory usage       ", sh_memory);
	add_entry("proc", "- Starts a task	", sh_proc);
	add_entry("ptest1", "- Starts a process test (ptest <1-3>)", sh_ptest1);
	add_entry("ptest2", "- Starts a process test (ptest <1-3>)", sh_ptest2);
	add_entry("ptest3", "- Starts a process test (ptest <1-3>)", sh_ptest3);
	add_entry("kill", "- Stops a task (kill <pid>)", sh_kill);
	add_entry("version", "- Show OS version   ", sh_version);
	add_entry("shutd", "- Computer shutdown   ", shutd);
	add_entry("reboot", "- Reboots computer   ", reboot);
	add_entry("exit", "- Exit shell         ", sh_exit);

	shell_running = 1;
	print_output();
		
	while(shell_running) 
	{
		sleep(1);
		//command = gets();
		//parse_input(command);
	}

}

