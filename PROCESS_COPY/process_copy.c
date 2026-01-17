#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>



int Check_pram(const char* srcfile,int pronum,int argc);
int Block_cur(const char* srcfile,int pronum);
int Process_create(const char* srcfile,const char*destfile,int pronum,int blocksize);


int main(int argc,char*argv[])
{
	const char* srcfile=argv[1];
	int pronum=3;
	if(argv[3]!=0)
	{
		pronum=atoi(argv[3]);
	}
	int res=Check_pram(srcfile,pronum,argc);
	if(res==-1)
	{
		return 0;
	}
	//任务切割，根据进程数量切片
	
	int blocksize=Block_cur(srcfile,pronum);
	if(blocksize==-1)
	{
		printf("blocksize error\n");
		return 0;
	}


	//创建子进程并重载完成拷贝
	res=Process_create(srcfile,argv[2],pronum,blocksize);
	if(res==-1)
	{
		printf("process create error\n");
		return 0;
	}

	while(1)
	{
		pid_t zpid;
		zpid=wait(NULL);
		if(zpid>0)
		{
			printf("wait success pid %d\n",zpid);
			break;
		}
		sleep(1);
	}
	return 0;
}

int Check_pram(const char* srcfile,int pronum,int argc)
{
	//参数数量验证
	if(argc<3)
	{
	    perror("argc num error");
		return -1;
	}
	//进程数量验证
	if(pronum<0||pronum>50)
	{
		return -1;
	}
	//源文件是否存在
	if((access(srcfile,F_OK))!=0)
	{
		return -1;
	}
	return 0;

}


int Block_cur(const char* srcfile,int pronum)
{
	//获得文件大小
	int fd=open(srcfile,O_RDONLY);
	if(fd==-1)
	{
		perror("open srcfile failed:");
		return -1;
	}
	int filesize=lseek(fd,0,SEEK_END);
	int blocksize;
	if(filesize%pronum==0)
	{
		blocksize=filesize/pronum;	
	}
	else
	{
		blocksize=filesize/pronum+1;
	}
	return blocksize;
}

int Process_create(const char* srcfile,const char*destfile,int pronum,int blocksize)
{
	int i=0;
	pid_t pid;
	for(i;i<pronum;i++)
	{
		pid=fork();
		if(pid==0)
		{
			break;
		}
	}
	if(pid>0)
	{

	}
	else if(pid==0)
	{
		int pos=i*blocksize;
		char strpos[10];
		char strblocksize[10];
		sprintf(strpos,"%d",pos);
		sprintf(strblocksize,"%d",blocksize);
		execl("/home/colin/20241119/process/PROCESS_COPY/mod/copy","copy",srcfile,destfile,strpos,strblocksize,NULL);
	}
	else
	{
		perror("fork failed:");
		return -1;
	}
	return 0;
}

