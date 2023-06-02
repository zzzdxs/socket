#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <stdlib.h>
#pragma comment(lib,"ws2_32.lib")  //加载ws2_32.dll

/*
流程：WSAStartup()->socket()->bind()->listen()->accept()->send()/recv()->closesocket()->WSACleanup() 
*/ 

#define BUF_SIZE 1024
#define SERVER_PORT 8000
#define SHOWTEXT "-----------服务端-----------\n"
#define CHOICETEXT "---SWITCH---\n case 1:server->client\n case 2:client->server\n case 3:file transfer\n case quit:quit\n"

int main()
{
	char sw[3]; 
	char recvbuf[100];
	char sendbuf[100];
	
	/**************************调用WSAStartup()进行初始化******************************/
	
	WSADATA wsaDATA;
	WSAStartup(MAKEWORD(2,2),&wsaDATA);
	
	/**************************调用socket()创建服务端套接字****************************/
	
	int sockfd=socket(AF_INET,SOCK_STREAM,0);//协议族IPv4,TCP Socket编程,协议自动推演
	if(sockfd!=-1)
	{
		printf(SHOWTEXT);
		printf("SOCKET SUCCESS %d\n",sockfd);
	}
	
	/**************************调用bind()将监听套接字绑定本地地址端口******************/
	
	struct sockaddr_in serveraddr;
	serveraddr.sin_family=AF_INET;//协议族IPv4
	serveraddr.sin_port=htons(SERVER_PORT);//端口 
	serveraddr.sin_addr.s_addr=INADDR_ANY;//任意IP 
	bind(sockfd,(SOCKADDR *)&serveraddr,sizeof(serveraddr));//完成绑定 
	
	/**************************调用listen()使socket进入被动监听状态********************/
	
	listen(sockfd,5);//5典型取值，能够接收的最多连接数 
	printf("等待客户端...\n");
	
	/**************************accept()响应客户端请求，建立连接************************/
	
	struct sockaddr_in clientaddr;//创建客户端地址结构体
	int len=sizeof(clientaddr);
	SOCKET aID=accept(sockfd,(SOCKADDR *)&clientaddr,&len);//conection()不断监听  x(int *)sizeof(clientaddr)
	if(aID)
	{
		printf("%d\n",aID); 
		printf("监听到新的客户端...\n"); 
	}
	/**************************数据传输 发送/接收**************************************/
	
	printf(CHOICETEXT);
	sprintf(sendbuf,CHOICETEXT);
	send(aID,sendbuf,strlen(sendbuf)+1,0);
	recv(aID,(char *)sw,3,0);
	if(sw[0]=='1')
	{
		
		printf("server->client\n");
		send(aID,"server->client\n",strlen("server->client\n"),0);
		while(1)
		{
			char sendbuf[100];
			gets(sendbuf);
			if(strcmp(sendbuf,"quit")==0)
			    break;
			send(aID,sendbuf,strlen(sendbuf),0);
		//	recv(aID,recvbuf,100,0);
		}
	}
	else if(sw[0]=='2')
	{
		printf("client->server\n");
		send(aID,"client->server\n",100,0);
		while(1)
		{
			int RecvLen=recv(aID,recvbuf,100,0);//返回值大于0成功，返回值即为接收到的数据长度
			if(RecvLen!=-1)
				printf("%d %s\n",RecvLen,recvbuf);
			else
				break;
			if(strcmp(recvbuf,"quit")==0)
			    break;
		}
	}
	else if(sw[0]=='3')
	{
		while(1)
		{
			char filename[100];
			strcpy(filename,"D:\\STM32\\0_devc\\socket_file\\server\\");
			/*
			char filename[100];
			recv(aID,filename,strlen(filename),0);
			*/
			char file[100];
			int RecvLen=recv(aID,file,100,0);//**********************************************
			if(RecvLen!=-1)
			{
				printf("%s\n",file);
				strcat(filename,file);
				printf("filename:%s\n",filename);
				FILE *fp=fopen(filename,"rb+");//二进制方式打开文件
				if(fp==NULL)
				{
					printf("cannot open file, press any key to exit\n");
					system("pause");
					exit(0);
				}
				else
				{
					char buf[BUF_SIZE];
					int nCount;
					while((nCount=fread(buf,1,BUF_SIZE,fp))>0)
					{
						send(aID,buf,nCount,0);
					}
					shutdown(aID,SD_SEND);
					recv(aID,buf,BUF_SIZE,0);
					
					fclose(fp);
				}
			}
			else
			{
				break;
			}
			
		}
	}
	
	/**************************closesocket()关闭套接字释放资源*************************/
	
	closesocket(aID); 
	closesocket(sockfd);
	
	/**************************WSACleanup()停止对socket的使用**************************/
	
	WSACleanup();
	
	return 0;
}
