// ----------------------------------------------------------------------------
// This are some standard math functions
// -----------------------------------------------------------------------------

/*
double sqrt(double x)
{
  double res;
  __asm__ __volatile__ ("fsqrt" : "=t" (res) : "0" (x));
  return res;
}
*/
int isfinite(double x) {
  unsigned short *val = (unsigned short *) &x;

  return (val[3] & 0x7ff0) != 0x7ff0;
}

int IsEqual(int x, int y)
{
   int e = 0;
   if(x == y)
	e = 1;
   return e;   
}

int GetLess(int x, int y) 
{
  int l;
  if(x<y)
  	l = y;
  else
	l = x;
  return l;
}

int GetGreater(int x, int y) 
{
  int g;
  if(x<y)
  	g = y;
  else
	g = x;
  return g;
}


