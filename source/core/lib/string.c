// -----------------------------------------------------------------------------
// This are some standard string functions
// -----------------------------------------------------------------------------

#include <lib/types.h>
#include <lib/ctypes.h>

unsigned strlen(const char *str)
{
    unsigned retval;
    for(retval = 0; *str != '\0'; str++) retval++;
    return retval;
}

char *strstr(string, substring)
    register char *string;	// String to search.
    char *substring;		// Substring to try to find in string.
{
    register char *a, *b;

    // First scan quickly through the two strings looking for a
    // single-character match.  When it's found, then compare the
    // rest of the substring.
    b = substring;
    if (*b == 0) {
	return string;
    }
    for ( ; *string != 0; string += 1) {
	if (*string != *b) {
	    continue;
	}
	a = string;
	while (1) {
	    if (*b == 0) {
		return string;
	    }
	    if (*a++ != *b++) {
		break;
	    }
	}
	b = substring;
    }
    return (char *) 0;
}

int strcmp(char *str1, char *str2)
{
      int i = 0;
      int failed = 0;
      while(str1[i] != '\0' && str2[i] != '\0')
      {
          if(str1[i] != str2[i])
          {
              failed = 1;
              break;
          }
          i++;
      }
      // why did the loop exit?
      if( (str1[i] == '\0' && str2[i] != '\0') || (str1[i] != '\0' && str2[i] == '\0') )
          failed = 1;
  
      return failed;
}

int strncmp( char *dest, char* src, unsigned int len )
{
	char val = 0;
	while ( len > 0 )
	{
		if ( (val = *dest - *src++) != 0 || !*dest++ )
			break;
		len--;
	}
	return val;
}

int strcpy(char *dest, char *src)
{
	char *d = dest;
	char *s = src;
	int count;
	while (*s)
	{
		*(d++) = *(s++);
		count++;
	}
	*(d++) = '\0';	// Null terminate
	return count;
}


int strncpy(char *dest, char *src, int len)
{
	char *d, *s;
	int val = 0;

	d = dest;
	s = src;

	while ( *s != '\0' && len != 0 )
	{
		*d++ = *s++;
		len--;
		val++;
	}
	*d++ = '\0';
	
	return val;
}

char *strcat(char *dest, const char *src)
{
    char *ret = dest;
    while (*dest)
        dest++;
    while (*dest++ = *src++)
        ;
    return ret;
}


// copies a string to another string from a certain offset to a certain len
void str_strncpy( char *dest, char *src, int off, unsigned int len )
{
	char *d, *s;
	int i;

	d = dest; s = src;
	for (i = 0; i < off; i++)
		*(s++);	// move up to offset
	for( i = 0; i < len; i++)
		*(d++) = *(s++);
	
}

char *split(char *str, char ch, int section)
{
	char *dest0 = (char*) kmalloc(32);
	char *dest1 = (char*) kmalloc(32);
	int i;
	for (i = 0; i < strlen(str); i++)
		if (str[i] == ch)
			break;
	str_strncpy(dest0, str, 0, i);
	str_strncpy(dest1, str, i + 1, strlen(str) - i);

	switch (section)
	{
	case 0:
		return dest0;
		break;
	case 1:
		return dest1;
		break;
	}

	return 0;
}

char *strtolower(char *string)
{
	int i;
	char *ret = (char*)kmalloc(strlen(string) + 1);
	for (i = 0; i < strlen(string); i++)
		ret[i] = tolower(string[i]);
	return ret;
}

char *strtoupper(char *string)
{
	int i;
	char *ret = (char*)kmalloc(strlen(string) + 1);
	for (i = 0; i < strlen(string); i++)
		ret[i] = toupper(string[i]);
	return ret;
}

char *strtok(char *str, const char *delim)
{
    char *ret;

    str += strcspn(str, delim);

    if (*str == '\0')
    {
        return NULL;
    }

    ret = str;

    str += strcspn(str, delim);

    if (*str)
    {
        *str++ = '\0';
    }

    return ret;
}

size_t strcspn(const char *str1, const char *str2)
{
  size_t i,j;
  i=0;

  while(*(str1+i)){
    j=0;
    while(*(str2+j)){
      if(*(str1+i) == *(str2+j)){
        break; //Found a match.
      }
      j++;
    }
    if(!*(str2+j)){
      return i; //No match found.
    }
    i++;
  }
  return i;
}

bool isNumericChar(char x)
{
    return (x >= '0' && x <= '9')? true: false;
}

//where did i get atoi from? i'm not sure..

int atoi(char *str)
{
    if (*str == NULL)
       return 0;

    int res = 0;  // Initialize result
    int sign = 1;  // Initialize sign as positive
    int i = 0;  // Initialize index of first digit

    // If number is negative, then update sign
    if (str[0] == '-')
    {
        sign = -1;
        i++;  // Also update index of first digit
    }

    // Iterate through all digits of input string and update result
    for (; str[i] != '\0'; ++i)
    {
        if (isNumericChar(str[i]) == false)
            return 0; // You may add some lines to write error message
                      // to error stream
        res = res*10 + str[i] - '0';
    }

    // Return result with sign
    return sign*res;
}

void strmov(char* dest, char* src){
	int i;
	for(i = 0; i <= strlen(src); i++)
		dest[i] = src[i];
}

void reverse(char s[])
 {
     int i, j;
     char c;
 
     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }

// --------------------------------------------------------------------
//		 	Taken from Linux
// -------------------------------------------------------------------
void itoa (char *buf, int base, int d)
{
       	char *p = buf;
       	char *p1, *p2;
       	unsigned long ud = d;
       	int divisor = 10;
     
       	/* If %d is specified and D is minus, put `-' in the head. */
       	if (base == 'd' && d < 0)
       	{
           	*p++ = '-';
           	buf++;
           	ud = -d;
       	}
       	else if (base == 'x')
         	divisor = 16;
     
       	/* Divide UD by DIVISOR until UD == 0. */
       	do
       	{
           	int remainder = ud % divisor;
     
           	*p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
       	}
       	while (ud /= divisor);
     
       	/* Terminate BUF. */
       	*p = 0;
     
       	/* Reverse BUF. */
       	p1 = buf;
       	p2 = p - 1;
       	while (p1 < p2)
       	{
          	char tmp = *p1;
           	*p1 = *p2;
           	*p2 = tmp;
           	p1++;
           	p2--;
       }
}
// --------------------------------------------------------------------------

