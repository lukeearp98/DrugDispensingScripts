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
	
	onDelay=1800;
	offDelay=38200;
	
	printf("%d\n", onDelay);
	for(i = 0; i<300; i++)
	{
		//On
		*pgdr = 0x01;
		*tflg1=0x40; //Clear TOC2 Flag
		*toc2=*tcnt + onDelay; //Read timer and add offset period
		while(((*tflg1)&0x40)==0); //Wait for TOC2 FLAG
	
		//Off
		*pgdr = 0x00;
		*tflg1=0x40; //Clear TOC2 Flag
		*toc2=*tcnt + offDelay; //Read timer and add offset period
		while(((*tflg1)&0x40)==0); //Wait for TOC2 FLAG	
	}
	
	onDelay=3000;
	offDelay=37000;
	printf("%d\n", onDelay);
	for(i = 0; i<300; i++)
	{
		//On
		*pgdr = 0x01;
		*tflg1=0x40; //Clear TOC2 Flag
		*toc2=*tcnt + onDelay; //Read timer and add offset period
		while(((*tflg1)&0x40)==0); //Wait for TOC2 FLAG
	
		//Off
		*pgdr = 0x00;
		*tflg1=0x40; //Clear TOC2 Flag
		*toc2=*tcnt + offDelay; //Read timer and add offset period
		while(((*tflg1)&0x40)==0); //Wait for TOC2 FLAG	
	}
	
	onDelay=4200;
	offDelay=35800;
	printf("%d\n", onDelay);
	for(i = 0; i<300; i++)
	{
		
		//On
		*pgdr = 0x01;
		*tflg1=0x40; //Clear TOC2 Flag
		*toc2=*tcnt + onDelay; //Read timer and add offset period
		while(((*tflg1)&0x40)==0); //Wait for TOC2 FLAG
	
		//Off
		*pgdr = 0x00;
		*tflg1=0x40; //Clear TOC2 Flag
		*toc2=*tcnt + offDelay; //Read timer and add offset period
		while(((*tflg1)&0x40)==0); //Wait for TOC2 FLAG	
	}
}
}

