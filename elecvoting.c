#include <lpc214x.h>          
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

void delay(unsigned int count)
{
  unsigned int j, k;
  for (j = 0; j <= count; j++)
    for (k = 0; k <= 600; k++);
}

void Terminal_INIT(void)
{
	PINSEL0|= 0x00000005;          
  	U0LCR = 0x83;                   
  	U0DLL = 97;                    
  	U0LCR = 0x03; 
}

void LCD_CMD(char command)
{
	IO0PIN = ( (IO0PIN & 0xFFFF00FF) | ((command & 0xF0)<<8) ); 
	IO0SET = 0x00000040;
	IO0CLR = 0x00000030; 
	delay(5);
	IO0CLR = 0x00000040;
	delay(5);
	IO0PIN = ( (IO0PIN & 0xFFFF00FF) | ((command & 0x0F)<<12) );
	IO0SET = 0x00000040;
	IO0CLR = 0x00000030;
	delay(5);
	IO0CLR = 0x00000040; 
	delay(5);	
}

void LCD_INIT(void)
{
	IO0DIR = 0x0000FFF0;
	delay(20);
	LCD_CMD(0x02);
	LCD_CMD(0x28);
	LCD_CMD(0x0C);
	LCD_CMD(0x06);
	LCD_CMD(0x01);
	LCD_CMD(0x80);
}

void LCD_STRING (char* msg)
{
	uint8_t i=0;
	while(msg[i]!=0)
	{
		IO0PIN = ( (IO0PIN & 0xFFFF00FF) | ((msg[i] & 0xF0)<<8) );
		IO0SET = 0x00000050;
		IO0CLR = 0x00000020; 
		delay(2);
		IO0CLR = 0x00000040;
		delay(5);
		IO0PIN = ( (IO0PIN & 0xFFFF00FF) | ((msg[i] & 0x0F)<<12) );
		IO0SET = 0x00000050; 
		IO0CLR = 0x00000020;
		delay(2);
		IO0CLR = 0x00000040; 
		delay(5);
		i++;
	}
}

unsigned int can1 = 0, can2 = 0, can3 = 0, can4 = 0, i=0, n = 8;
char txt1[] = "Candidate 1 :", txt2[] = "Candidate 2 :", txt3[] = "Candidate 3 :", txt4[] = "Candidate 4 :", num[2];
char *info = "Candidate 1: A \n\r\r\nCandidate 2: B \n\r\r\nCandidate 3: C \n\r\r\nCandidate 4: D \n\r\r\n", Wintxt[] = "Winner ", NoWintxt[] = "No Winner";

int main()
{	
	Terminal_INIT();

	while(info[i]!='\0')
	{
		while(!(U0LSR & 0x20));
		U0THR = info[i];
		i++;
	}
	
	LCD_INIT();		

	PINSEL2 = 0xFFFF0000;
	IO1DIR |= 0x00100000;
	IO1DIR &= 0xFFF0FFFF;
	IO1CLR = 0xFFFF0000;
	
	i = 0;
	           
    while(i < n)
    {		
		LCD_CMD(0x80);
		sprintf(num,"%d",can1);
		LCD_STRING(txt1);
		LCD_CMD(0xC0);
		LCD_STRING(num);

		delay(5000);

		LCD_CMD(0x80);
		sprintf(num,"%d",can2);
		LCD_STRING(txt2);
		LCD_CMD(0xC0);
		LCD_STRING(num);
	
		delay(5000);

		LCD_CMD(0x80);
		sprintf(num,"%d",can3);
		LCD_STRING(txt3);
		LCD_CMD(0xC0);
		LCD_STRING(num);
		
		delay(5000);

		LCD_CMD(0x80);
		sprintf(num,"%d",can4);
		LCD_STRING(txt4);
		LCD_CMD(0xC0);
		LCD_STRING(num);
		
		delay(5000);		

		// For Calculating Vote of Candidate 1
		if (IO1PIN & 0x00010000)
		{                 
			can1++;
			IO1SET = 0x00100000;
			delay(200);
			IO1CLR = 0x00100000;
		}
		
		// For Calculating Vote of Candidate 2
		if (IO1PIN & 0x00020000)
		{                 
			can2++;
			IO1SET = 0x00100000;
			delay(200);
			IO1CLR = 0x00100000;
		}
		
		// For Calculating Vote of Candidate 3
		if (IO1PIN & 0x00040000)
		{                 
			can3++;
			IO1SET = 0x00100000;
			delay(200);
			IO1CLR = 0x00100000;
		}
		
		// For Calculating Vote of Candidate 4
		if (IO1PIN & 0x00080000)
		{                 
			can4++;
			IO1SET = 0x00100000;
			delay(200);
			IO1CLR = 0x00100000;
		}
		i++;		         
    }
	sprintf(num,"%d",i);
	LCD_CMD(0x80);
	LCD_STRING("Total Votes: ");
	LCD_CMD(0xC0);
	LCD_STRING(num);

	i = 0;
	if ((can1 > can2) && (can1 > can3) && (can1 > can4))
	{
		while(Wintxt[i]!='\0')
		{
			while(!(U0LSR & 0x20));
			U0THR = Wintxt[i];
			i++;
		}
		i = 0;		

		while(txt1[i+1]!='\0')
		{
			while(!(U0LSR & 0x20));
			U0THR = txt1[i];
			i++;
		}
	}
	else if ((can2 > can1) && (can2 > can3) && (can2 > can4))
	{
		while(Wintxt[i]!='\0')
		{
			while(!(U0LSR & 0x20));
			U0THR = Wintxt[i];
			i++;
		}
		i = 0;

		while(txt2[i+1]!='\0')
		{
			while(!(U0LSR & 0x20));
			U0THR = txt2[i];
			i++;
		}
	}
	else if((can3 > can2) && (can3 > can1) && (can3 > can4))
	{
		while(Wintxt[i]!='\0')
		{
			while(!(U0LSR & 0x20));
			U0THR = Wintxt[i];
			i++;
		}
		i = 0;		

		while(txt3[i+1]!='\0')
		{
			while(!(U0LSR & 0x20));
			U0THR = txt3[i];
			i++;
		}
	}
	else if ((can4 > can2) && (can4 > can3) && (can4 > can1))
	{
		while(Wintxt[i]!='\0')
		{
			while(!(U0LSR & 0x20));
			U0THR = Wintxt[i];
			i++;
		}
		i = 0;

		while(txt4[i+1]!='\0')
		{
			while(!(U0LSR & 0x20));
			U0THR = txt4[i];
			i++;
		}
	}
	else
	{
		while(NoWintxt[i]!='\0')
		{
			while(!(U0LSR & 0x20));
			U0THR = NoWintxt[i];
			i++;
		}
	} 
}