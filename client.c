#include"../tcpSocket/tcpSocket.h"


int main()
{
	int choice;
	init_Socket();
	SOCKET fd = createClientSocket("127.0.0.2");
	printf("\n====================Welcome to Winsocket====================\n");
	printf("Please select a communication mode!\n1 represents SEND FILE;\n2 represents RECEIVE FILE;\n3 represents CHAT\n");
	printf("Please enter a number>");
	scanf_s("%d", &choice);
	while (choice != 1 && choice != 2 && choice != 3) //����Ƿ���ȷ����������
	{
		printf("Please enter the correct number!>");
		scanf_s("%d", &choice);
	}
	getchar();//�޸�Switch��bug
	switch (choice)
	{
	case 1:
	{
		char open_filename[100];
		printf("Please enter the path of the file or drag it in directly:\nFor example C:\\Users\\temp.txt\n>");
		gets(open_filename);
		DWORD start, end;
		start = GetTickCount();
		sendFile(fd, open_filename);
		end = GetTickCount();
		printf("Time spent=%d ms\n", end - start);
		break;
	}
	case 2:
	{
		recvFile(fd, "../���յ�server�ļ�");
		break;
	}
	case 3:
	{
		char sendbuf[512] = { 0 };
		char recvbuf[512] = { 0 };
		while (true)
		{
			//������Ϣ
			printf("send>");
			gets_s(sendbuf, 512);
			if (SOCKET_ERROR == send(fd, sendbuf, strlen(sendbuf), 0)) //��˭��������buf,����
			{
				printf("send failed code %d", WSAGetLastError());
			}

			//recv���ƶ���socket������Ϣ
			if (0 < recv(fd, recvbuf, 512, 0))
			{
				printf("recv:%s\n", recvbuf);
				memset(sendbuf, 0, sizeof(sendbuf));
				memset(recvbuf, 0, sizeof(recvbuf));
			}
		}
		break;
	}
	}
	closesocket(fd);
	close_Socket();
	printf("\n=========================Client end=========================");
	getchar();
	return 0;

}
