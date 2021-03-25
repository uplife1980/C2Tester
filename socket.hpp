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
#include <chrono>
#include "common.hpp"
#include "Thread.hpp"
#include "StrategeToThread.hpp"

#define TCP_BUFFER_SIZE (64 * 1024)

class Thread;

void loop_once(int efd, int maxWaitTime);
void updateEvents(int efd, int fd, int events, int op) ;
class Socket : public Command, public std::enable_shared_from_this<Socket>
{
public:
    Socket(const Endpoint &endpoint, Thread* callerThread);
    //Socket(Socket* So){};
    
    int getFD(){return m_iSockfd;}

    virtual int run();

private:
    int m_iSockfd;
    Thread* m_callerThread;

    // bool beginflag;
    // std::chrono::high_resolution_clock::time_point lasttime;
    // std::chrono::high_resolution_clock::time_point curtime;
    // std::chrono::high_resolution_clock::time_point begintime;

    SleepPerPolling m_sleepPerPolling;

};
#endif