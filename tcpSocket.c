#include"tcpSocket.h"
#define BUFSIZE 32

long s_fileSzie;           //文件大小
long r_filesize;
char* g_fileBuf;
char* g_fileBuf_rev;
char file_extension_send[BUFSIZE] = { 0 };
char filesize_send[BUFSIZE] = { 0 };
char file_extension_recv[BUFSIZE] = { 0 };
char filesize_recv[BUFSIZE] = { 0 };
char sendbuf[BUFSIZE] = {'ack'};
char recvbuf[BUFSIZE] = { 0 };


bool init_Socket()
{
    WSADATA wsadata;
    if (0 != WSAStartup(MAKEWORD(2,2), &wsadata))//parm1 请求的版本 parm2 传出参数
    {
        printf("WSAStarup failed code %d", WSAGetLastError());
        return false;
    }
    return true;
}

bool close_Socket()
{
    if(0 != WSACleanup())
    {
        printf("WSACleanup failed code %d",WSAGetLastError());
        return false;
    }
    return true;
}

//读取文件
bool readFile(const char* fileName)
{
    //获取文件扩展名
    char* file_extension;                               //文件扩展名
    file_extension = strrchr(fileName,'.');
    strncpy(file_extension_send, file_extension, 5);    //截取文件扩展名
    printf("File format:%s\n", file_extension_send);    //打印
    FILE* read = fopen(fileName, "rb");                 //read为文件指针，以二进制模式打开 允许读写
    if (read == NULL)
    {
        perror("file open failed:\n");
        return false;
    }

    //获取文件大小
    //fseek() parm1指向FILE对象的指针 parm2相对whence的偏移量，以字节为单位 parm3 whence
    fseek(read, 0, SEEK_END);   //指针移动到SEEK_END文件的末尾
    s_fileSzie = ftell(read);   //ftell返回字节数long
    fseek(read, 0, SEEK_SET);   //移动到开头
    printf("Filesize:%d Byte\n", s_fileSzie);
    sprintf(filesize_send, "%d", s_fileSzie); //转成字符串
    //分配内存calloc() parm1要被分配的元素个数 parm2元素的大小 返回一个指针，指向已分配的内存
    //sizeof获取了数据在内存中所占用的存储空间，以字节为单位来计数 sizeof(char)获取某个数据类型所占用空间的字节数 字符串以\0结尾
    g_fileBuf = (char*)calloc(s_fileSzie, sizeof(char));//申请内存，初始化为0 //parm1要被分配的元素个数 parm2元素的大小
    if (!g_fileBuf)
    {
        return false;
    }
    //读入内存
    //fread()读取 内存块的指针;每个元素的大小;元素的个数;指向FILE对象的指针
    fread(g_fileBuf, sizeof(char), s_fileSzie, read);
    fclose(read);   //关闭流 stream
    return true;
}

//发送文件
bool sendFile(SOCKET s, const char* fileName)
{
    readFile(fileName);
    //发扩展名
    while (1)
    {
        //…calculating…
        if (send(s, file_extension_send, BUFSIZE, 0) == SOCKET_ERROR)
        {
            err("sendfail1");
            return false;
        }
        else
        {
            //printf("sent!1\n");
        }

        if (0 < recv(s, recvbuf, BUFSIZE, 0))
        {
            //printf("recv:%s\n", recvbuf);
        }
        else
            printf("fail");

        //发大小
        if (send(s, filesize_send, BUFSIZE, 0) == SOCKET_ERROR)
        {
            err("sendfail2");
            return false;
        }
        else
        {
            //printf("sent!2\n");
        }

        if (0 < recv(s, recvbuf, BUFSIZE, 0))
        {
            //printf("recv:%s\n", recvbuf);
        }
        else
            printf("fail");

        //发文件
        if (send(s, g_fileBuf, s_fileSzie, 0) == SOCKET_ERROR)
        {
            err("sendfail");
            return false;
        }
        else
        {
            printf("Sent!\n");
        }
        break;
    }
    return true;
}

//接收文件
bool recvFile(SOCKET s, const char* fileName)
{
    //收扩展名
    while (1)
    {

        if (recv(s, file_extension_recv, BUFSIZE, 0) > 0)
        {
            //printf("re%s ceived!1\n", file_extension_recv);
        }
        else
        {
            printf("receive failed\n");
            err("recv1");
        }

        if (SOCKET_ERROR == send(s, sendbuf, BUFSIZE, 0)) //给谁发，发送buf,长度
        {
            err("send");
        }

        //收大小
        if (recv(s, filesize_recv, BUFSIZE, 0) >= 0)
        {
            //printf("received!2\n");
        }
        else
        {
            printf("receive failed\n");
            err("recv2");
        }
        if (SOCKET_ERROR == send(s, sendbuf, BUFSIZE, 0)) //给谁发，发送buf,长度
        {
            err("send");
        }
        r_filesize = atol(filesize_recv);
        g_fileBuf_rev = (char*)calloc(r_filesize, sizeof(char));
        
        //收文件
        if (recv(s, g_fileBuf_rev, r_filesize, 0) >= 0)
        {
            printf("Received!\n");
            saveFile(fileName);
            break;
        }
        else
        {
            printf("receive failed\n");
            err("recv");
            break;
        }

    }
    return false;
}

//保存文件
bool saveFile(const char* fileName)
{
    
    char* join_filename = malloc(strlen(fileName) + strlen(file_extension_recv) + 1);
    strcpy(join_filename, fileName);
    strcat(join_filename, file_extension_recv);
    FILE* write = fopen(join_filename, "wb");//文件名
    if (!write)
    {
        perror("file open failed:\n");
        return false;
    }
    //读入内存
    fwrite(g_fileBuf_rev, sizeof(char), r_filesize, write);
    fclose(write);
    printf("File received successful!\nYou can find it in File Socket");
    //printf("%s\n", file_extension_recv);
    //printf("%d\n", r_filesize);
    //printf("%s\n", g_fileBuf_rev);
    return true;
}


//SOCKET createServerSocket()和SOCKET createClientSocket()重要
SOCKET createServerSocket()
{
    //1创建空的socket
    //parm1:af 地址协议族
    //parm2：type 传输协议类型 流式套接字 数据报
    //parm3: protocol 使用具体的传输协议
    SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd == INVALID_SOCKET)
    {
        err("socket");
        return INVALID_SOCKET;    
    }

    //2给socket绑定ip地址和端口号
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);       //大端存储 高地址存低位 小端存储 
    //htons本地字节序->网络字节序
    addr.sin_addr.S_un.S_addr = ADDR_ANY;//inet_addr("127.0.0.1") 
    //ADDR_ANY本地本机网卡的任意ip

    if (SOCKET_ERROR == bind(fd, (struct sockaddr*)&addr, sizeof(addr)))
    {
        err("bind");
        return false;
    }

    //3监听
    listen(fd, 10);
    return fd;

}


SOCKET createClientSocket(const char *ip)//项目-属性-SDL检查已经关闭
{
    //1创建空的socket
    //parm1:af 地址协议族
    //parm2：type 传输协议类型 流式套接字 数据报
    //parm3: protocol 使用具体的传输协议
    SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd == INVALID_SOCKET)
    {
        err("socket");
        return INVALID_SOCKET;
    }

    //2与服务器建立连接的ip
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);       //大端存储 高地址存低位 小端存储 
    //htons本地字节序->网络字节序
    addr.sin_addr.S_un.S_addr = inet_addr(ip);
    //ADDR_ANY本地本机网卡的任意ip

    if (INVALID_SOCKET == connect(fd, (struct sockaddr*)&addr, sizeof(addr)))
    {
        err("connect");
        return false;
    }
    else
    {
        printf("Connected to Server!");
    }

    return fd;
}
