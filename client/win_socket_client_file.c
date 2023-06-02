#include <winsock2.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma comment(lib,"ws2_32.lib")

#define BUF_SIZE 1024
#define SERVER_PORT 8000
#define SHOWTEXT "-----------�ͻ���-----------\n"

//WSAStartup()->socket()->connect->send()->recv()->closesock()

int main()
{
	char sw[1];
	
	
	printf(SHOWTEXT);
	/**************************����WSAStartup()���г�ʼ��******************************/
	
	WSADATA wsaDATA;
	WSAStartup(MAKEWORD(2,2),&wsaDATA);
    
	/**************************����socket()�����׽���**********************************/
	
	SOCKET sockcl=socket(AF_INET,SOCK_STREAM,0);
	if(sockcl!=-1)
	{
		printf("�ͻ����׽����Ѿ���!\n");
    }
    else
    {
        printf("�ͻ����׽��ִ�ʧ��!\n");
        return -1;//����
	}
	
	/**************************�׽��ְ󶨱��ص�ַ�˿ڲ�������������********************/
	
	SOCKADDR_IN clientaddr;
	clientaddr.sin_addr.S_un.S_addr=inet_addr("172.168.112.63");
	clientaddr.sin_family=AF_INET;
	clientaddr.sin_port=htons(SERVER_PORT);//�������ֽ�˳��ת��Ϊ�����ֽ�˳�� 
	
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
	
	/**************************���ݴ��� ����/����**************************************/
	
	char sendbuf[100];
	char recvbuf[100];//����
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
			int RecvLen=recv(sockcl,a,100,0);//����ֵ����0�ɹ�������ֵ��Ϊ���յ������ݳ���
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
			printf("��������ļ���·��\n");
			scanf("%s",&filename);
			char sendfile[100];
			printf("����Դ�ļ���·��\n");
			scanf("%s",&sendfile);
			send(sockcl,sendfile,strlen(sendfile),0); 
			*/
			char file[100]={0};
			printf("�������ļ���:");
			scanf("%s",&file);
			if(strcmp(file,"quit")==0)
			    break;
			send(sockcl,file,strlen(file),0);
			printf("success file=%s\n",file);
			strcat(filename,file);
			FILE *fp=fopen(filename,"wb");//�����Ʒ�ʽ�����ļ� 
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
				//�ļ�������Ϻ�ֱ�ӹر��׽��֣��������shutdown()
				fclose(fp);
			}
		}
	}
	
	/**************************closesocket()�ر��׽����ͷ���Դ*************************/
	
	closesocket(sockcl); 
	
	/**************************WSACleanup()ֹͣ��socket��ʹ��**************************/
	
	WSACleanup();
	
}
