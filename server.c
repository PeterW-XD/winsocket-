#include"../tcpSocket/tcpSocket.h"


int main()
{
    int i = 0;
    int choice;

    init_Socket();
    SOCKET serfd = createServerSocket();
    //提示信息
    printf("====================Welcome to Winsocket====================\n");
    printf("Waiting for client connection...\n");
    //如果有客户端请求连接
    struct sockaddr_in clientaddr;
    int addrlen = sizeof(clientaddr);
    SOCKET clifd = accept(serfd, (struct sockaddr *)&clientaddr, &addrlen);
    if (INVALID_SOCKET == clifd)
    {
        printf("accept failed code %d", WSAGetLastError());
    }
    else
    {
        printf("Connected!\n");
        printf("Client ip:%s, port:%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
    }

    //提示信息
    printf("Please select a communication mode!\n1 represents SEND FILE;\n2 represents RECEIVE FILE;\n3 represents CHAT\n");
    printf("Please enter a number>");
    scanf_s("%d", &choice);
    while (choice != 1 && choice != 2 && choice != 3) //检测是否正确输入了数字
    {
        printf("Please enter the correct number!>");
        scanf_s("%d", &choice);
    }
    //记住要先打开收！！！！！
    getchar();//修复Switch的bug
    switch (choice)
    {
    case 1:
    {
        char open_filename[100];
        printf("Please enter the path of the file or drag it in directly:\nFor example C:\\Users\\temp.txt\n>");
        gets(open_filename);
        DWORD start, end;
        start = GetTickCount();
        sendFile(clifd, open_filename);
        end = GetTickCount();
        printf("Time spent=%d ms\n", end - start);
        break;
    }
    case 2:
    {
        recvFile(clifd, "../接收的client文件");
        break;
    }
    case 3:
    {
        // 缓冲区
        char sendbuf[512] = { 0 };
        char recvbuf[512] = { 0 };
        while (1)
        {

            //recv从制定的socket接收信息
            if (0 < recv(clifd, recvbuf, 512, 0))
            {
                printf("recv:%s\n", recvbuf);
                memset(recvbuf, 0, sizeof(recvbuf));
            }

            //发送消息
            memset(sendbuf, 0, sizeof(sendbuf));
            printf("send>");
            gets_s(sendbuf, 512);
            if (SOCKET_ERROR == send(clifd, sendbuf, strlen(sendbuf), 0)) //给谁发，发送buf,长度
            {
                err("send");
                return false;
            }
        }
        break;
    }
    }
    
    closesocket(clifd);
    closesocket(serfd);
    close_Socket();
    printf("\n=========================Server end=========================");
    getchar();
    return 0;
}