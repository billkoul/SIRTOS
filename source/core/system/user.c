/* Copyright (c) 2015, 2016 All Right Reserved, Vasileios Kouliaridis

THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

	USER
*/
#include <security/md5.h>
#include <system/user.h>

user_t user_list[10];
user_t current_user;
int uid = -1;
extern byte hash1;
MD5_CTX ctx;

static int getnewuid()
{
       uid++;
       return uid;
}

void register_user(char *username, int sring, char *password)
{
    	int i = getnewuid();
    	user_list[i].name = username;
    	user_list[i].sring = sring;
    	user_list[i].password = password;
	current_user = user_list[i];
}

void user_login(char *username)
{
	for(int i=0; i<=uid; i++)
	{
		if(strcmp(user_list[i].name, username) == 0)
		{
			authenticate_user(user_list[i]);
			return;
		}
	}
	printf("Wrong username! \n");
}

void authenticate_user(user_t u)
{
	char *pass;
        bool move_on = false;
	printf("Trying to log in as %s \n", u.name);

	while(move_on == false)
	{
		printf("Password: ");
		pass = gets();
		if(validateUser(pass, u) == 0)
			move_on = true;
	}
}

int validateUser(char *pass, user_t u)
{
	char *encrypted_pass = get_encrypted_text(pass);

	if(u.password == encrypted_pass)
	{
		current_user = u;
		printf("Succesfuly logged in as %s \n", u.name);
		return 0;
	}
	return 1;
}

char* get_user()
{
    return current_user.name;
}

char* get_privileges()
{
	char* priv;
	switch(current_user.sring)
	{
		case 0: priv = "111111"; break;
		case 1: priv = "111110"; break;
		case 2: priv = "111100"; break;
		case 3: priv = "111000"; break;
		case 4: priv = "110000"; break;
		case 5: priv = "100000"; break;
		default: priv = "100000"; break;
	}
	return priv;
}

