#include <ioslib.h>
#include <iolib.h>
#include <stdio.h>
#include <rngLib.h>
/* defines */

#define  TASK_PRI         60    /* Priority of spawned tasks */
#define  TASK_STACK_SIZE  10000 /* stack size for spawned tasks */
#define  TIMER_RELATIVE 0       /*  interval relative to the current time */

char msg1[]="Hello, This message is from UART of board\n\r";
char msg2[]="Please give a sentence\n\r";
char msg3[] ="\r\nI get your message, and will close myself\n\r";

int num;

RING_ID rngId;
void demo()
{
	printf("/*******************************************************************/\n");
	printf("This is the demo of Vxworks BSP on Atmel AT91RM9200 Board\n\n");

	printf("1)UART\n");
	printf("If you see this demo, the UART0 is OK\n");
	printf("To test UART1, please connect UART1 to a terminal and run uartTest(\"/tyCo/1\")\n\n");
	
	printf("This is the demo of Vxworks BSP on Atmel AT91RM9200 Board\n");
}


STATUS ttyRecvTask(int fd)
{
	char c;
	char buf[128]={0};
	int i;
	fd_set fdset;
	FD_ZERO(&fdset);
	FD_SET(fd,&fdset);
	if(select(fd+1,NULL,&fdset,NULL,NULL)==ERROR)
	{
		printf("pend on error!\n");
		return ERROR;
	}
	FOREVER {
		if(read(fd,&c,1)==1)
		{
			/*write(fd,&c,1)*/;
			if(!rngIsFull(rngId))
			{
				rngBufPut(rngId,&c,1);
			}
			else
			{
				return ERROR;
			}
			printf("rec:%c(%x)\n",c,c);
			if(c=='\n')
			{
				write(fd,"$\r\n",strlen("$\r\n"));
				break;
			}
		}
	}
	write(fd,msg3,strlen(msg3));
	
	close(fd);
	fd=0;
	num=rngBufGet(rngId,buf,128);
	printf("rngBufGet:\n");
	for(i=0;i<num;i++)
	{
		printf("%c ",buf[i]);
	}
	

}



STATUS uartTest(char* name)
{
	char c;
	int uartFd;
	if(uartFd!=0) close(uartFd);
	printf("open %s ...",name);
	uartFd = open (name, O_RDWR, 0);
	if(uartFd == -1)
	{
		printf("error\n");
		return;
	}
	else
	{
		printf("ok,fd=%d\n",uartFd);
	}
	printf("BAUDRATE:115200\n");
	ioctl (uartFd, FIOBAUDRATE, 115200);
	ioctl (uartFd, FIOSETOPTIONS, OPT_RAW);/*OPT_TERMINAL OPT_RAW*/

	write(uartFd,msg1,strlen(msg1));
	write(uartFd,msg2,strlen(msg2));

	rngId=rngCreate(128);

	
	/* Spwan the tyRecv task */
	
    	if (taskSpawn ("ttyRecvTask", TASK_PRI, 0, TASK_STACK_SIZE, 
                      (FUNCPTR) ttyRecvTask, uartFd, 0, 0, 0, 0, 0, 0, 0, 
                       0, 0) == ERROR)
        {
        	perror ("uartTest: Error in spawning uartTest task");
        	return (ERROR);
        } 
	

	/*
	while(read(uartFd,&c,1)==1)
	{
		printf("rec:%c(%x)\n",c,c);
		write(uartFd,&c,1);
	
		if(c=='\n') break;
	}
	write(uartFd,msg3,strlen(msg3));
	
	close(uartFd);uartFd=0;	
	*/
		
}
