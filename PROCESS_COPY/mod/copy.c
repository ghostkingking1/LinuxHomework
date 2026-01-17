#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>


int main(int argc,char*argv[])
{
	const char* srcfile=argv[1];
	const char* destfile=argv[2];
	int pos=atoi(argv[3]);
	int blocksize=atoi(argv[4]);
	int fd_src=open(srcfile,O_RDONLY);
	umask(0000);
	int fd_dest=open(destfile,O_RDWR|O_CREAT,0666);
	if(fd_src==-1)
	{
		perror("srcfile open failed");
		return 0;
	}
	if(fd_dest==-1)
	{
		perror("destfile open failed");
		return 0;
	}
	lseek(fd_src,pos,SEEK_SET);
	lseek(fd_dest,pos,SEEK_SET);
	char buf[blocksize];
	int len=read(fd_src,buf,blocksize);

	write(fd_dest,buf,len);
	close(fd_src);
	close(fd_dest);
}
