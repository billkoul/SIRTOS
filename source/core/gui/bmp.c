#include <gui/bmp.h>

int displaybmp_8bit(int x, int y, BMPheader_t* bmp);
int displaybmp_32bit(int x, int y, BMPheader_t* bmp);

/* FOR EACH PIXEL
R = 54 + (Color*4)+2
G = 54 + (Color*4)+1
B = 54 + (Color*4)
finalpixel = R<<16 | G << 8 | B 
where Color is the value at the offset */

int displaybmp(int x, int y, BMPheader_t* bmp)
{
    if(bmp->Type != 0x4D42)return 1;
	if(bmp->Compression != 0)return 1;
	
	switch(bmp->BitsPerPixel)
	{
        case 1:
        break;
		  
		case 4:
		break;
		  
		case 8:
			if(displaybmp_8bit(x, y, bmp) != 0) return 1;
		break;
		  
		case 24:
			if(displaybmp_32bit(x, y, bmp) != 0) return 1;
		break;
		  
		default:
			return 1;
		break;
	}	  
	return 0;   
}

int displaybmp_8bit(int x, int y, BMPheader_t* bmp)
{
	if(bmp->BitsPerPixel != 8)return 1;
	
	static u32int w, h;
	h = bmp->Height;
	w = bmp->Width;
	
	while(w % 4)
	{
		  //Width not multiple of 4
		  w++;
	}
	
	for(y=h-1;y>=0;y--)
		 for(x=0;x<w;x++)
			  //Only print actual pixels, for null values just iterate
			  if(x <= bmp->Width)
				   //TODO: get RGB value from quad/pallette
				   putPixel_32bpp(x,y,(u32int)((bmp + bmp->Offset)+(y*w)+x));
			  
	return 0;
}

int displaybmp_32bit(int x, int y, BMPheader_t* bmp)
{
	if(bmp->BitsPerPixel != 24)return 1;
	
	static u32int w, h, pixel;
	static int R, G, B;
	h = bmp->Height;
	w = bmp->Width;
	
	while(w % 4)
	{
		//Width not multiple of 4
		w++;
	}
	
	for(y=h-1;y>=0;y--)
		for(x=0;x<w;x++)
		    //Only print actual pixels, for null values just iterate
			pixel = 0;
			if(x <= bmp->Width)
				R = (int)((bmp + bmp->Offset)+(y*w)+x)+2;
				G = (int)((bmp + bmp->Offset)+(y*w)+x)+1;
				B = (int)((bmp + bmp->Offset)+(y*w)+x);
				pixel = R<<16 | G<<8 | B;
				putPixel_32bpp(x,y,pixel);
	return 0;
}
