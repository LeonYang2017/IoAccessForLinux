#include<sys/io.h>
#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>


#define outb(value,port) __asm__ ("outb %%al,%%dx"::"a" (value),"d" (port))

#define inb(port) ({ unsigned char _v; __asm__ volatile ("inb %%dx,%%al":"=a" (_v):"d" (port)); _v; })

void delay(unsigned int xms)
{

unsigned int i,j;
for(i=xms;i>0;i--)
for(j=110;j>0;j--);

}


void Int_To_Hex( int integer )
{
    int c;
    int i, j, digit;
    
    for ( i = 7, j = 0; i >= 0; i--, j++ )
    {
        digit = (integer >> (i * 4)) & 0xf;
        if (digit < 10)
        {
            c = digit + 0x30;
        }
        else
        {
            c = digit + 0x37;
        }
    }
}


int tolower(int c)
{
    if (c >= 'A' && c <= 'Z')
    {
        return c + 'a' - 'A';
    }
    else
    {
        return c;
    }
}

int htoi(char s[])
{
    int i;
    int n = 0;
    if (s[0] == '0' && (s[1]=='x' || s[1]=='X'))
    {
            i = 2;
        }
    else
    {
            i = 0;
        }
    for (; (s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >='A' && s[i] <= 'Z');++i)
    {
            if (tolower(s[i]) > '9')
            {
                        n = 16 * n + (10 + tolower(s[i]) - 'a');
                    }
            else
            {
                        n = 16 * n + (tolower(s[i]) - '0');
                    }
        }
    return n;
}

void usage(){

	printf("usage:\n./IoRead port\n");
	printf("./IoRead 0x80 \n");
	printf("Note:\n Only for x86 platform!\n");
	printf("Please run with root permission!\n");

}

void version(){

	printf("Version: 1.0.0\n");
	printf("Author : 530954029@qq.com\n");

}

int main(int argc,char *argv[]){

	int ret = 0;
	int intport = 0;
	int Res = 0;
	int file = 0;


	if (argc != 2){
		usage();
		return 1;
	}

	if(strcmp(argv[1] ,"-h")== 0 || strcmp(argv[1] ,"/h")== 0|| strcmp(argv[1] ,"-?")== 0){
		usage();
		return 0;		
	}

	if(strcmp(argv[1] ,"-v")== 0 || strcmp(argv[1] ,"-v")== 0 || strcmp(argv[1] ,"--version")== 0){
		version();
		return 0;		
	}
	
	intport = htoi(argv[1]);
	Int_To_Hex(intport);
	if(intport > 0xffff || intport <= 0){
		printf("Invalid input!!!\n");
		return 1;
	}

	ret = iopl(3);
	delay(100);
	if(ret != 0){
		printf("Run with Root!!!\n");
		return 1;
		}
	Res = inb(intport);
	delay(100);
	Res = inb(intport);
	printf("Io Port 0x%.2x is 0x%.2x via IO port\n",intport,Res);

	
	file = open("/dev/port",O_RDWR);
	lseek(file,intport,SEEK_SET);
	read(file,&Res,1);
	printf("Io Port 0x%.2x is 0x%.2x via memory\n",intport,Res);
	close(file);
	
	delay(100);
	ret = iopl(0);
	
	return 0;
}

