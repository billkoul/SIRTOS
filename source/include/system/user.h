#ifndef USER_H
#define USER_H

#include <lib/types.h>

typedef struct user
{
	char *name;
        char *password;
	int sring;
        
} user_t;

void set_user(user_t u);
char* get_user();
void user_login(char *username);
char* get_privileges();
void authenticate_user(user_t u);
void register_user(char *username, int sring, char *password);

#endif
