// -----------------------------------------------------------------------------
// This is the standard "isblank" function
// -----------------------------------------------------------------------------

int isblank(int c)
{
  // checks for a blank character; that is, a space or a tab.  This function
  // is a GNU extension.
  return ((c == ' ') || (c == '\t'));
}
