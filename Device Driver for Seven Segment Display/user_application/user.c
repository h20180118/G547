#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define DEVICE "/dev/7-segment-display"


int main()  {
	int i,fd,p;
	char cr;
	char ch,write_buf[2],read_buf[100];
	char string[10];
	fd =open(DEVICE, O_RDWR);
	
	if(fd==-1){
		printf("file %s either doesn't exist or locked by other process",DEVICE);
			exit(-1);
	}
	printf("write to the device\n");
	
	
	
			printf("enter the number to be displayed: \n");
			scanf(" %d",&i);
			i=i+48;
			write_buf[0]=(char) i;
			write_buf[1]='\0';
			write(fd,write_buf,sizeof(write_buf));
			
		
		
	
	close(fd);				




	return 0;
}



