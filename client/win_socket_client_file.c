#include <winsock2.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma comment(lib,"ws2_32.lib")

#define BUF_SIZE 1024
#define SERVER_PORT 8000
#define SHOWTEXT "-----------客户端-----------\n"

//WSAStartup()->socket()->connect->send()->recv()->closesock()

int main()
{
	char sw[1];
	
	
	printf(SHOWTEXT);
	/**************************调用WSAStartup()进行初始化******************************/
	
	WSADATA wsaDATA;
	WSAStartup(MAKEWORD(2,2),&wsaDATA);
    
	/**************************调用socket()创建套接字**********************************/
	
	SOCKET sockcl=socket(AF_INET,SOCK_STREAM,0);
	if(sockcl!=-1)
	{
		printf("客户端套接字已经打开!\n");
    }
    else
    {
        printf("客户端套接字打开失败!\n");
        return -1;//结束
	}
	
	/**************************套接字绑定本地地址端口并发送连接请求********************/
	
	SOCKADDR_IN clientaddr;
	clientaddr.sin_addr.S_un.S_addr=inet_addr("172.168.112.63");
	clientaddr.sin_family=AF_INET;
	clientaddr.sin_port=htons(SERVER_PORT);//将主机字节顺序转换为网络字节顺序 
	
	/*
	typedef struct in_addr {
	    union {
	        struct { u_char  s_b1, s_b2, s_b3, s_b4; } S_un_b;
	        struct { u_short s_w1, s_w2; } S_un_w;
	        u_long S_addr;
	    } S_un;
	} IN_ADDR, *PIN_ADDR, *LPIN_ADDR;

	*/
	connect(sockcl,(SOCKADDR *)&clientaddr,sizeof(clientaddr));	
	
	/**************************数据传输 发送/接收**************************************/
	
	char sendbuf[100];
	char recvbuf[100];//接收
	recv(sockcl,recvbuf,100,0);
	printf("%s\n",recvbuf);
	
	gets(sw);
	send(sockcl,sw,strlen(sw),0);
	if(sw[0]=='1')
	{
		char rec[100]={};
		int RecvLen=recv(sockcl,rec,strlen(rec),0);
		if(RecvLen!=-1)
			printf("%s\n",rec);
		while(1)
		{
			char a[100]={};
			int RecvLen=recv(sockcl,a,100,0);//返回值大于0成功，返回值即为接收到的数据长度
			if(RecvLen!=-1)
				printf("%s\n",a);
			else
				break;
			if(strcmp(a,"quit")==0)
			    break;
		}
	}
	else if(sw[0]=='2')
	{
		int RecvLen=recv(sockcl,recvbuf,strlen(recvbuf),0);
		if(RecvLen!=-1)
			printf("%s\n",recvbuf);
		while(1)
		{
			gets(sendbuf);
			if(strcmp(sendbuf,"quit")==0)
			    break;
			send(sockcl,sendbuf,strlen(sendbuf),0);
		}
	}
	else if(sw[0]=='3')
	{
		while(1)
		{
			char filename[100]="D:\\STM32\\0_devc\\socket_file\\client\\";
			/*
			char filename[100];
			printf("输入接收文件夹路径\n");
			scanf("%s",&filename);
			char sendfile[100];
			printf("输入源文件夹路径\n");
			scanf("%s",&sendfile);
			send(sockcl,sendfile,strlen(sendfile),0); 
			*/
			char file[100]={0};
			printf("请输入文件名:");
			scanf("%s",&file);
			if(strcmp(file,"quit")==0)
			    break;
			send(sockcl,file,strlen(file),0);
			printf("success file=%s\n",file);
			strcat(filename,file);
			FILE *fp=fopen(filename,"wb");//二进制方式创建文件 
			if(fp==NULL)
			{
				printf("Cannot open file, press any key to exit...\n");
				system("pause");
				exit(0);
			}
			else
			{
				char buf[BUF_SIZE];
				int nCount;
				while((nCount=recv(sockcl,buf,BUF_SIZE,0))>0)
				{
					fwrite(buf,nCount,1,fp);
				}
				printf("File transfer success! \n");
				//文件接收完毕后直接关闭套接字，无需调用shutdown()
				fclose(fp);
			}
		}
	}
	
	/**************************closesocket()关闭套接字释放资源*************************/
	
	closesocket(sockcl); 
	
	/**************************WSACleanup()停止对socket的使用**************************/
	
	WSACleanup();
	
}
