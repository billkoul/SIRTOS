// -----------------------------------------------------------------------------
// This is the standard "floor" function
// -----------------------------------------------------------------------------

int isxdigit(int c)
{
  // checks  for a hexadecimal digits, i.e. one of 0 1 2 3 4 5 6 7 8 9
  // a b c d e f A B C D E F.
  return (((c >= '0') && (c <= '9')) ||
	  ((c >= 'a') && (c <= 'f')) ||
	  ((c >= 'A') && (c <= 'F')));
}
