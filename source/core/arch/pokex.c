
void _farpokeb(unsigned short selector, unsigned long offset, unsigned char value)
{
  __asm__ __volatile__ ("movw %w0,%%fs\n"
      "	.byte 0x64 \n"
      "	movb %b1,(%k2)"
      :
      : "rm" (selector), "qi" (value), "r" (offset));
}

unsigned char _farpeekb(unsigned short selector, unsigned long offset)
{
  unsigned char result;
  __asm__ __volatile__ ("movw %w1,%%fs \n"
      "	.byte 0x64 \n"
      "	movb (%k2),%b0"
      : "=q" (result)
      : "rm" (selector), "r" (offset));
  return result;
}

void _farpokew(unsigned short selector, unsigned long offset, unsigned short value)
{
  __asm__ __volatile__ ("movw %w0,%%fs \n"
      "	.byte 0x64 \n"
      "	movw %w1,(%k2)"
      :
      : "rm" (selector), "ri" (value), "r" (offset));
}

void _farpokel(unsigned short selector, unsigned long offset, unsigned long value)
{
  __asm__ __volatile__ ("movw %w0,%%fs \n"
      "	.byte 0x64 \n"
      "	movl %k1,(%k2)"
      :
      : "rm" (selector), "ri" (value), "r" (offset));
}


unsigned long _farpeekl(unsigned short selector, unsigned long offset)
{
  unsigned long result;
  __asm__ __volatile__ ("movw %w1,%%fs\n"
      "	.byte 0x64\n"
      "	movl (%k2),%k0"
      : "=r" (result)
      : "rm" (selector), "r" (offset));
  return result;
}
