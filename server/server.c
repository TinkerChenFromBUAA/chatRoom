#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include "config.h"

void* HandleRequest(void* arg);

int main(int argc, char* argv[])
{
	int listenFd,CliFd,MaxFd,Maxi,i,nready;
	int FdSet[FD_SETSIZE];
	int opt = 1;	
	fd_set Reset,Allset;
	struct sockaddr_in serverAddr;
	pthread_t pid;
	
	if(-1 == (listenFd = socket(AF_INET, SOCK_STREAM, 0)))
	{
		perror("socket");
		return -1;
	}
	
	setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR,(char*)&opt, sizeof(opt));

	bzero(&serverAddr,sizeof(struct sockaddr_in));
	serverAddr.sin_family      = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port        = htons(PORT);
	
	if(-1 == bind(listenFd,(struct sockaddr*)&serverAddr,sizeof(struct sockaddr_in)))
	{
		perror("bind");
		return -1;
	}
	
	if(-1 == listen(listenFd,LISTENQ))
	{
		perror("listen");
		return -1;
	}
	
	MaxFd = listenFd;
	Maxi = -1;
	
	printf("FD_SETSIZE = %d\n",FD_SETSIZE);
	
	for(i = 0; i < FD_SETSIZE; ++i)
		FdSet[i] = -1;
		
	FD_ZERO(&Allset);
	FD_SET(listenFd,&Allset);
	
	while(1)
	{
		Reset = Allset;
		nready = select(MaxFd+1,&Reset,NULL,NULL,NULL);
		
		if(FD_ISSET(listenFd,&Reset))
		{
		
			if(-1 == (CliFd = accept(listenFd, NULL, 0)))
			{
				perror("accept");
				return -1;
			}
			
			printf("Clifd = %d\n",CliFd);
			/************测试添加程序************/
			if(0 != pthread_create(&pid,NULL,HandleRequest,(void*)&CliFd))
			{
				perror("pthread_create");
				continue;
			}
			/***********************************/
			/*
			for(i = 0; i < FD_SETSIZE; ++i)
			{
				printf("Fdset[%d] = %d\n",i,FdSet[i]);
			
				if(FdSet[i] < 0)
				{
					FdSet[i] = CliFd;
					printf("index = %d ClidFd = %d\n",i,CliFd);
					break;				
				}

			}
			
			if(i == FD_SETSIZE)
			{
				printf("too many Link\n");
				return -1;
			}
			
			FD_SET(CliFd,&Allset);
			
			if(CliFd > MaxFd)
				MaxFd = CliFd;
			
			if(i > Maxi)
				Maxi = i;
				
			if(--nready < 0)
				continue;
			*/
		}
		/*
		for(i = 0; i <= Maxi; ++i)
		{
			printf("Maxi = %d\n",Maxi);
			
			if(FdSet[i] < 0)
				continue;
				
			if(FD_ISSET(FdSet[i],&Reset))
			{
				printf("index = %d fd = %d pthread_create\n",i,FdSet[i]);
				pthread_create(&pid,NULL,HandleRequest,(void*)&FdSet[i]);
			}
			
			if(--nready < 0)
				break;				
			
			FD_CLR(FdSet[i],&Allset);
			FdSet[i] = -1;
			
			MaxFd = listenFd;
		}
		*/
	}
	
	close(listenFd);
	return 0;
}

void* HandleRequest(void* arg)
{
	int sockFd = *((int*)arg);
	int n;
	//char buf[MAX_LINE] = {0};
	Message message;
	
	printf("sockFd = %d\n",sockFd);
	memset(&message,0,sizeof(Message));
	//memset(buf,0,sizeof(buf));
	n = nRead(sockFd,&message,sizeof(Message));
	printf("n = %d server receive\n",n);
	if(n <= 0)
	{
		fflush(stdout);
		close(sockFd);
		*((int*)arg) = -1;
		return NULL;
	}
	else
	{
		//memcpy(&message,buf,sizeof(buf));
		
		switch(message.msgType)
		{
			case REGISTER:
				printf("register\n");
				break;
			case LOGIN:
				printf("login\n");
				break;
			default:
				printf("unknow opertion\n");
				break;
		}
	}
	
	close(sockFd);
	*((int*)arg) = -1;	
	pthread_exit(0);
}
