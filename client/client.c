#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "config.h"

int main(int argc, char* argv[])
{
	int choice;
	int sock;
	struct sockaddr_in clientAddr;
	Message message;
	/*检查输入参数数量是否正确*/
	if(argc != 2)
	{
		printf("Invalid Input\n");
		return -1;
	}
	
	while(1)
	{
		/*1.创建套接子*/
		if((sock = socket(AF_INET,SOCK_STREAM,0)) == -1)
		{
			perror("socket");
			return -1;
		}
		
		/*2.初始化sockaddr_in结构体*/
		bzero(&clientAddr,sizeof(struct sockaddr_in));
		clientAddr.sin_family = AF_INET;
		clientAddr.sin_port   = htons(PORT);
		if(1 != inet_pton(AF_INET,argv[1],&clientAddr.sin_addr))
		{
			perror("inet_pton");
			return -1;
		}
		
		/*3.创建连接*/
		if(-1 == connect(sock,(struct sockaddr*)&clientAddr,sizeof(struct sockaddr_in)))
		{
			perror("connect");
			return -1;
		}
		
		mainInterface();
		setbuf(stdin,NULL);
		scanf("%d",&choice);
		printf("choice = %d\n",choice);	
			
		while(choice != 1 && choice != 2 && choice != 3 && choice != 4)
		{
			printf("choice = %d\n",choice);
			scanf("%d",&choice);
		}
		
		switch(choice)
		{
			case REGISTER:
				memset(&message,0,sizeof(Message));
				message.msgType = REGISTER;
				strcpy(message.content," ");
				nWrite(sock,&message,sizeof(Message));
				printf("client already Send\n");					
				break;
			case LOGIN:
				break;
			case HELP:
				break;
			case EXIT:
				close(sock);
				printf("exit\n");
				exit(0);
				break;
			default:
				printf("Invalid Input\n");
				break;	
		}

		printf("client quit\n");
	}
	
	close(sock);
	return 0;
}
