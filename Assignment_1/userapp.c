

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>             
#include <unistd.h>             
#include <sys/ioctl.h>
#include <stdint.h>          
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "chardev.h"



int ioctl_select_channel(int file_desc, int chn)
{
    int ret_val;

    ret_val = ioctl(file_desc,ioctl_selectchannel, chn);

    if (ret_val < 0) {
        printf("ioctl select channel failed:%d\n", ret_val);
        exit(-1);
    }
    return 0;
}


int ioctl_select_allignment(int file_desc,char allgn)
{
    int ret_val;

    ret_val = ioctl(file_desc, ioctl_selectallignment, allgn);

    if (ret_val < 0) {
        printf("ioctl select allignment failed:%d\n", ret_val);
        exit(-1);
    }
    return 0;
}



int main()
{
    int file_desc;
    uint16_t data	;
    int chn;
    char allgn;
    
    printf("hi device open\n");
 file_desc = open("/dev/adc8", O_RDONLY);
    if (file_desc < 0)
    {
        printf("Can't open device file: %s\n","/dev/adc8");
	exit(-1);
  	
     }

	printf("Enter channel no(0 to 7) :\n");
	scanf("%d",&chn);
	
	printf("Select allignment of the adc result (l or r):\n");
	scanf(" %c",&allgn);
   	

	if((chn<0)||(chn>7)||(allgn!='l' && allgn!='r'))
	     {
		printf("Invalid data inserted\n");
		exit(-1);
	      }
	
	
        ioctl_select_channel(file_desc,chn); 
	ioctl_select_allignment(file_desc,allgn);

	if(read( file_desc,&data,sizeof(data)))
	 {

	if (allgn=='l'){

	printf("The received value from adc8  is %i\n",data);
	data=data/64;
	printf("The actual 10 bit result from adc8  is %i\n",data);
	exit(-1);
	}
	else {
		printf("The actual 10 bit result from adc8  is %i\n",data);
		exit(-1);
		}
	}
close(file_desc);
 	   return 0;
}






    


