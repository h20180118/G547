# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>

# define file1 "/dev/ADXL_x"
# define file2 "/dev/ADXL_y"
# define file3 "/dev/ADXL_z"


char buffer[100];

char file;

int i;
int f1,f2,f3;

int main (void)

{

int fdold;

printf("enter the file name x or y or z \n");
scanf ("%c",&file);


switch(file)


{

case 'x':
{ fdold = f1;
	f1=open (file1, O_RDWR);
	  
	read(f1,buffer,sizeof(buffer));
	printf("10 bit decimal value is:");	

	for(i=0;i<10;i++){
		
		printf("%d",buffer[i]);
		}
	close(f1); 
break;
}

case 'y': {fdold = f2;
	f2=open (file2, O_RDWR);
	read (f2,buffer,sizeof(buffer));
printf("10 bit decimal value is:");	
for(i=0;i<10;i++){
		
		printf("%d",buffer[i]);
		}
     
	close(f2); 
        break;
}
case 'z':{ fdold =f3;
	f3= open (file3,O_RDWR);
	read (f3,buffer,sizeof(buffer));
printf("10 bit decimal value is:");	
	

for(i=0;i<10;i++){
		
		printf("%d",buffer[i]);
		}
     
	close(f3);      
break;
}

default: printf("file not found\n");
	break;
}
}




