#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <stdlib.h>
#pragma comment(lib,"ws2_32.lib")  //����ws2_32.dll

/*
���̣�WSAStartup()->socket()->bind()->listen()->accept()->send()/recv()->closesocket()->WSACleanup() 
*/ 

#define BUF_SIZE 1024
#define SERVER_PORT 8000
#define SHOWTEXT "-----------�����-----------\n"
#define CHOICETEXT "---SWITCH---\n case 1:server->client\n case 2:client->server\n case 3:file transfer\n case quit:quit\n"

int main()
{
	char sw[3]; 
	char recvbuf[100];
	char sendbuf[100];
	
	/**************************����WSAStartup()���г�ʼ��******************************/
	
	WSADATA wsaDATA;
	WSAStartup(MAKEWORD(2,2),&wsaDATA);
	
	/**************************����socket()����������׽���****************************/
	
	int sockfd=socket(AF_INET,SOCK_STREAM,0);//Э����IPv4,TCP Socket���,Э���Զ�����
	if(sockfd!=-1)
	{
		printf(SHOWTEXT);
		printf("SOCKET SUCCESS %d\n",sockfd);
	}
	
	/**************************����bind()�������׽��ְ󶨱��ص�ַ�˿�******************/
	
	struct sockaddr_in serveraddr;
	serveraddr.sin_family=AF_INET;//Э����IPv4
	serveraddr.sin_port=htons(SERVER_PORT);//�˿� 
	serveraddr.sin_addr.s_addr=INADDR_ANY;//����IP 
	bind(sockfd,(SOCKADDR *)&serveraddr,sizeof(serveraddr));//��ɰ� 
	
	/**************************����listen()ʹsocket���뱻������״̬********************/
	
	listen(sockfd,5);//5����ȡֵ���ܹ����յ���������� 
	printf("�ȴ��ͻ���...\n");
	
	/**************************accept()��Ӧ�ͻ������󣬽�������************************/
	
	struct sockaddr_in clientaddr;//�����ͻ��˵�ַ�ṹ��
	int len=sizeof(clientaddr);
	SOCKET aID=accept(sockfd,(SOCKADDR *)&clientaddr,&len);//conection()���ϼ���  x(int *)sizeof(clientaddr)
	if(aID)
	{
		printf("%d\n",aID); 
		printf("�������µĿͻ���...\n"); 
	}
	/**************************���ݴ��� ����/����**************************************/
	
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
			int RecvLen=recv(aID,recvbuf,100,0);//����ֵ����0�ɹ�������ֵ��Ϊ���յ������ݳ���
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
				FILE *fp=fopen(filename,"rb+");//�����Ʒ�ʽ���ļ�
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
	
	/**************************closesocket()�ر��׽����ͷ���Դ*************************/
	
	closesocket(aID); 
	closesocket(sockfd);
	
	/**************************WSACleanup()ֹͣ��socket��ʹ��**************************/
	
	WSACleanup();
	
	return 0;
}
