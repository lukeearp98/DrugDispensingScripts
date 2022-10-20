#include <stdio.h>

void main(void)
{
	unsigned int *tcnt,*toc2,onDelay, i;
	unsigned long offDelay;
	unsigned char *pactl, *tflg1,*tctl1,*pgddr,*pgdr;
	tctl1=(unsigned char*)0x20;
	tflg1=(unsigned char*)0x23;
	toc2=(unsigned int*)0x18;
	tcnt=(unsigned int*)0x0e;
	pgddr=(unsigned char*)0x3;
	pgdr=(unsigned char*)0x02;
	pactl=(unsigned char*)0x26;
	*pactl=0x03;
	*pgddr =0xff;
	*tctl1=0x00;
	
	for(;;)
	{
		offDelay=500;
		//step 1
		*pgdr = 0x01;
		for(i = 0; i < offDelay; i++);
		//step 2
		*pgdr = 0x03;
		for(i = 0; i < offDelay; i++);
		//step 3
		*pgdr = 0x02;
		for(i = 0; i < offDelay; i++);
		//step 4
		*pgdr = 0x06;
		for(i = 0; i < offDelay; i++);
		//step 5
		*pgdr = 0x04;
		for(i = 0; i < offDelay; i++);
		//step 6
		*pgdr = 0xc;
		for(i = 0; i < offDelay; i++);
		//step 7
		*pgdr = 0x08;
		for(i = 0; i < offDelay; i++);
		//step 8
		*pgdr = 0x09;
		for(i = 0; i < offDelay; i++);
	}
}

