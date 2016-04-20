#define cpuid(in, a, b, c, d) __asm__("cpuid": "=a" (a), "=b" (b), "=c" (c), "=d" (d) : "a" (in));

#include <lib/types.h>

char *Intel[] = {
	"Brand ID Not Supported.", 
	"Intel(R) Celeron(R) processor", 
	"Intel(R) Pentium(R) III processor", 
	"Intel(R) Pentium(R) III Xeon(R) processor", 
	"Intel(R) Pentium(R) III processor", 
	"Reserved", 
	"Mobile Intel(R) Pentium(R) III processor-M", 
	"Mobile Intel(R) Celeron(R) processor", 
	"Intel(R) Pentium(R) 4 processor", 
	"Intel(R) Pentium(R) 4 processor", 
	"Intel(R) Celeron(R) processor", 
	"Intel(R) Xeon(R) Processor", 
	"Intel(R) Xeon(R) processor MP", 
	"Reserved", 
	"Mobile Intel(R) Pentium(R) 4 processor-M", 
	"Mobile Intel(R) Pentium(R) Celeron(R) processor", 
	"Reserved", 
	"Mobile Genuine Intel(R) processor", 
	"Intel(R) Celeron(R) M processor", 
	"Mobile Intel(R) Celeron(R) processor", 
	"Intel(R) Celeron(R) processor", 
	"Mobile Geniune Intel(R) processor", 
	"Intel(R) Pentium(R) M processor", 
	"Mobile Intel(R) Celeron(R) processor"
};

char *Intel_Other[] = {
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Intel(R) Celeron(R) processor", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Intel(R) Xeon(R) processor MP", 
	"Reserved", 
	"Reserved", 
	"Intel(R) Xeon(R) processor", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved", 
	"Reserved"
};

char *cpuid_features(void)
{
	uint32_t unused, edx;
	char *string = " ";

	cpuid(1, unused, unused, unused, edx);
	string = "CPU Features";
	if (edx & 0x00000001)	strcpy(string, " Floating Point Unit");
	if (edx & 0x00000002)	strcpy(string, " Virtual Mode Extensions");
	if (edx & 0x00000040)	strcpy(string, " Physical Address Extensions");
	if (edx & 0x00000200)	strcpy(string, " On-chip APIC");
	if (edx & 0x40000000)	strcpy(string, " IA-64 CPU");
	cpuid(0x80000001, unused, unused, unused, edx);
	strcpy(string, "Extended Features ");
	if (edx & (1<<29))		
		strcpy(string, " AMD64 Compliant");

	return string;
}

char *cpuid_intel()
{
	uint32_t eax, ebx, ecx, edx, max_eax, unused, signature;
	int model, family, type, brand, stepping, reserved;
	int extended_family = -1;
		
	char *string = "Intel: ";

	cpuid(1, eax, ebx, unused, unused);
	model		= (eax >> 4) & 0xF;
	family		= (eax >> 8) & 0xF;
	type		= (eax >> 12) & 0x3;
	brand		= (ebx & 0xFF);
	stepping		= (eax & 0xF);
	reserved		= (eax >> 14);
	signature	= (eax);

	switch (type)
	{
		case 0:
			strcpy(string,"Original OEM ");
			break;
		case 1:
			strcpy(string,"Overdrive ");
			break;
		case 2:
			strcpy(string,"Dual Capable ");
			break;
		case 3:
			strcpy(string,"Reserved ");
			break;
	}
	switch (family)
	{
		case 3:
			strcpy(string,"i386");
			break;
		case 4:
			strcpy(string,"i486");
			break;
		case 5:
			strcpy(string,"Pentium");
			break;
		case 6:
			strcpy(string,"Pentium Pro");
			break;
		case 15:
			strcpy(string,"Pentium 4 ");
			break;
	}
	if(family == 15) 
	{
		extended_family = (eax >> 20) & 0xff;
		strcpy(string,"Extended family");
	}
	if (family != 15) 
	{
		strcpy(string,"Model ");
		switch(family) 
		{
			case 3:
				break;
			case 4:
				switch(model)
				{
					case 0:
					case 1:
						strcpy(string,"DX");
						break;
					case 2:
						strcpy(string,"SX");
						break;
					case 3:
						strcpy(string,"487/DX2");
						break;
					case 4:
						strcpy(string,"SL");
						break;
					case 5:
						strcpy(string,"SX2");
						break;
					case 7:
						strcpy(string,"Write-back enhanced DX2");
						break;
					case 8:
						strcpy(string,"DX4");
						break;
				}
				break;
			case 5:
				switch(model) 
				{
					case 1:
						strcpy(string,"60/66");
						break;
					case 2:
						strcpy(string,"75-200");
						break;
					case 3:
						strcpy(string,"for 486 system");
						break;
					case 4:
						strcpy(string,"MMX");
						break;
				}
				break;
			case 6:
				switch(model) 
				{
					case 1:
						strcpy(string,"Pentium Pro");
						break;
					case 3:
						strcpy(string,"Pentium II Model 3");
						break;
					case 5:
						strcpy(string,"Pentium II Model 5/Xeon/Celeron");
						break;
					case 6:
						strcpy(string,"Celeron");
						break;
					case 7:
						strcpy(string,"Pentium III/Pentium III Xeon - external L2 cache");
						break;
					case 8:
						strcpy(string,"Pentium III/Pentium III Xeon - internal L2 cache");
						break;
				}
				break;
			}
	}

	cpuid(0x80000000, max_eax, unused, unused, unused);
	if(brand > 0) 
	{
		strcpy(string,brand);
		if(brand < 0x18) 
		{
			if(signature == 0x000006B1 || signature == 0x00000F13) 
				strcpy(string, Intel_Other[brand]);
			 else
				strcpy(string, Intel[brand]);
		} 
		else 
			strcpy(string,"Reserved");
	}
	return string;
}

char *cpuid_vendor()
{
	int i;
	uint32_t unused, ebx, ecx, edx;
	cpuid(0, unused, ebx, ecx, edx);
	char vendor[13];
	vendor[12] = '\0';
	for (i = 0; i < 4; i++)
	{
		vendor[i]	= ebx >> (8 * i);
		vendor[i + 4]	= edx >> (8 * i);
		vendor[i + 8]	= ecx >> (8 * i);
	}
	return vendor;
}

char *cpuid_detect()
{
	uint32_t eax, ebx, ecx, edx;
	cpuid(0, eax, ebx, ecx, edx);

	char *string;

	string = cpuid_vendor();

	switch (ebx)
	{
		case 0x756E6547:
			string = cpuid_intel();
			break;
		case 0x68747541:
			string = "Unknown x86 CPU";
			break;
		default:
			string = "Unknown x86 CPU";
			break;
	}
	return string;
}

