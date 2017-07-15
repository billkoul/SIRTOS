// -----------------------------------------------------------------------------
// This is the standard "pow" function
// -----------------------------------------------------------------------------

double pow(double x, double y)
{
  int count;

	if ((x < 0) && (floor(y) != y))
	{
			return (0);
	}

	if (y == 0)
		x = 1;
	else
		{
			for (count = 1; count < y; count ++)
	x *= x;
		}

	return (x);
}
