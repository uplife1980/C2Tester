//ref: https://github.com/yedf/epoll-example/blob/master/main.cc

#include "socket.hpp"

#define exit_if(r, ...)                                                \
    if (r)                                                             \
    {                                                                  \
        printf(__VA_ARGS__);                                           \
        printf("error no: %d error msg %s\n", errno, strerror(errno)); \
        exit(1);                                                       \
    }

void setNonBlock(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    exit_if(flags < 0, "fcntl failed");
    int r = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    exit_if(r < 0, "fcntl failed");
}

void updateEvents(int efd, int fd, int events, int op)
{
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = events;
    ev.data.fd = fd;
    //printf("%s fd %d events read %d write %d\n", op == EPOLL_CTL_MOD ? "mod" : "add", fd, ev.events & EPOLLIN, ev.events & EPOLLOUT);
    int r = epoll_ctl(efd, op, fd, &ev);
    exit_if(r, "epoll_ctl failed");
}

void handleRead(int efd, int fd)
{
    static char* buf = new char[4096];
    int n = 0;
    struct sockaddr_in fMsgAddr;
    socklen_t addrLen = sizeof(fMsgAddr);
    memset(&fMsgAddr, 0, addrLen);
    while ((n = ::recvfrom(fd, buf, sizeof(buf), 0, (sockaddr *)&fMsgAddr, &addrLen)) > 0)
    {
        //printf("read %d bytes\n", n); 
    }
    if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
        return;
    exit_if(n < 0, "read error"); //ʵ��Ӧ���У�n<0Ӧ�������������EINTR
    printf("fd %d closed\n", fd);
    close(fd);
}

void handleWrite(int efd, int fd)
{
    //ʵ��Ӧ��Ӧ��ʵ�ֿ�дʱд�����ݣ������ݿ�д�Źرտ�д�¼�
    updateEvents(efd, fd, EPOLLIN|EPOLLET, EPOLL_CTL_MOD);
}

void loop_once(int efd, int maxWaitTime)
{
    const int kMaxEvents = 200;
    struct epoll_event activeEvs[1000];
    int n = epoll_wait(efd, activeEvs, kMaxEvents, maxWaitTime);
    //printf("epoll_wait return %d\n", n);
    for (int i = 0; i < n; i++)
    {
        int fd = activeEvs[i].data.fd;
        int events = activeEvs[i].events;
        if (events & (EPOLLIN | EPOLLERR))
        {
            handleRead(efd, fd);
        }
        else if (events & EPOLLOUT)
        {
            handleWrite(efd, fd);
        }
        else
        {
            exit_if(1, "unknown event");
        }
    }
}

Socket::Socket(const Endpoint &endpoint)
{
    m_iSockfd = socket(AF_INET, SOCK_STREAM, 0);
    


    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(std::stoi(endpoint.strPort));
    //��IP��ַת���������ֽ��������serveraddr��
    inet_pton(AF_INET, endpoint.strAddr.c_str(), &serveraddr.sin_addr.s_addr);

    /*�ͻ��˵���connect�������ӵ���������*/
    if (connect(m_iSockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
    {
        perror("connect error");
        exit(1);
    }

    int bufSize = TCP_BUFFER_SIZE;
    int err = ::setsockopt(m_iSockfd, SOL_SOCKET, SO_RCVBUF, (char *)&bufSize, sizeof(int));
    setNonBlock(m_iSockfd);
    if (m_iSockfd < 0)
    {
        perror("socket error");
        exit(1);
    }

    string strData("GET "+endpoint.strOtherPara+" HTTP/1.1\r\n");
    string rn("\r\n");
    strData += "Range: bytes=0-\r\nUser-Agent: Samba Streaming Server\r\nHost: ";
    strData += endpoint.strAddr;
    strData += ":";
    strData +=endpoint.strPort;
    strData += "\r\nTransfer-Delay: -4000\r\nIngress-Capacity: 9437184000\r\nConnection: close\r\n\r\n";

    ::write(m_iSockfd, strData.c_str(), strData.size());
}

