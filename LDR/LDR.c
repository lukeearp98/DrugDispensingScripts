#include <stdio.h>

void main(void)
{	
	unsigned char in, *porta, *ddra;
	porta=(unsigned char *)0x00; /* This is casting values into pointers */
	ddra=(unsigned char *)0x01;
	*ddra=0xfe; /* This is saying that DDRA is to 7 outputs and 1 input */
	
	for(;;)
	{
		printf("\nporta:%u", porta);
	}
}

