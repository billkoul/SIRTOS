// -----------------------------------------------------------------------------
// This is the standard "isalnum" function
// -----------------------------------------------------------------------------

int isalnum(int c)
{
	// checks for an alphanumeric character; it is equivalent to
	// (isalpha(c) || isdigit(c)).
	return (((c >= 'a') && (c <= 'z')) ||
		((c >= 'A') && (c <= 'Z')) ||
		((c >= '0') && (c <= '9')));
}
