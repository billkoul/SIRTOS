// -----------------------------------------------------------------------------
// This is the standard "isalpha" function
// -----------------------------------------------------------------------------

int isalpha(int c)
{
	// checks for an alphabetic character; in the standard "C"	locale, it is
	// equivalent	to (isupper(c) || islower(c)).	In some locales, there may
	// be additional characters for which isalpha() is true -- letters which
	// are neither upper case nor lower case.
	return (((c >= 'a') && (c <= 'z')) ||
		((c >= 'A') && (c <= 'Z')));
}
