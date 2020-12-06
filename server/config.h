#ifndef _CONFIG_H
#define _CONFIG_H

#include <sys/socket.h>
#include <netinet/in.h>

#ifndef FD_SETSIZE
#define FD_SETSIZE	1024
#endif

#define MAX_LINE       8192
#define PORT  		8888
#define LISTENQ	1024

/*标志*/
enum Flag{
	YES,	/*代表被禁言*/
	NO		/*代表没有被禁言*/
};

/*定义服务器--客户端 消息传送类型*/
enum MessageType{	
	REGISTER = 1,	/*注册请求*/		
	LOGIN,		/*登陆请求*/
	HELP,		/*帮助请求*/
	EXIT,				/*退出请求*/
	VIEW_USER_LIST,		/*查看在线列表*/
	GROUP_CHAT,		/*群聊请求*/
	PERSONAL_CHAT,		/*私聊请求*/
	VIEW_RECORDS,		/*查看聊天记录请求*/
	RESULT,				/*结果消息类型*/
	UNKONWN				/*未知请求类型*/
};

/*定义操作结果 */
enum StateRet{
    EXCEED, //已达服务器链接上限
    SUCCESS, //成功
	FAILED,  //失败
    DUPLICATEID, //重复的用户名
	INVALID,	//不合法的用户名
    ID_NOT_EXIST, //账号不存在
    WRONGPWD, //密码错误
	ALREADY_ONLINE,		//已经在线
	ID_NOT_ONLINE,	//账号不在线
	ALL_NOT_ONLINE, 	//无人在线
	MESSAGE_SELF	//消息对象不能选择自己
};

/*定义服务器 -- 客户端 消息传送结构体*/
typedef struct _Message{
	char content[2048];		/*针对聊天类型的消息，填充该字段*/
	int msgType;	/*消息类型 即为MessageType中的值*/
	int msgRet;		/*针对操作结果类型的消息，填充该字段*/
	struct sockaddr_in sendAddr; /*发送者IP*/
	struct sockaddr_in recvAddr;
	char sendName[20]; /*发送者名称*/
	char recvName[20]; /*接收者名称*/
	char msgTime[20];  /*消息发送时间*/
}Message;

/*定义用户信息的数据结构*/
typedef struct _User
{
   	char username[64];   /*用户名称*/
	char password[64];   /*用户密码*/
	struct sockaddr_in userAddr;  /*用户的IP地址*/
	int sockFd;					  /*当前用户的套接字文件描述符*/
	int speak;                    /*是否禁言*/
	char registerTime[20];        /*注册时间*/
}User;

/*在线用户的链表结构体*/
typedef struct _NodeList
{
	User user;                     /*用户信息*/
	struct _NodeList* next;        /*指向下一个用户信息节点的指针*/
}NodeList;

//封装后的读取函数
int nRead(int fd, void* buf, int num);

#endif
