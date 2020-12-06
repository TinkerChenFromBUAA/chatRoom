#include "config.h"
#include <unistd.h>

int nRead(int fd, void* buf, int num)
{
	int nleft = num;
	int n;
	char* tmp = (char*)buf;
	
	while(nleft > 0)
	{
		n = read(fd,tmp,nleft);
		
		if(n <= 0)
			break;
			
		nleft -= n;
		tmp   += n;
	}
	
	return num - nleft;
}

int nWrite(int fd, const void* buf, int num)
{
	int nleft = num;
	int n;
	const char* tmp = buf;
	
	while(nleft > 0)
	{
		n = write(fd,tmp,nleft);
		
		if(n <= 0)
			break;
			
		nleft -= n;
		tmp   += n;
	}
	
	return num - nleft;
}
