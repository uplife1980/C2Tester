#ifndef __H_SOCKET__
#define __H_SOCKET__

#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <cstring>
#include "common.hpp"

#define TCP_BUFFER_SIZE (64 * 1024)
void loop_once(int efd, int maxWaitTime);
void updateEvents(int efd, int fd, int events, int op) ;
class Socket
{
public:
    Socket(const Endpoint &endpoint);
   // void run();
    int getFD(){return m_iSockfd;}

private:
    int m_iSockfd;
};
#endif