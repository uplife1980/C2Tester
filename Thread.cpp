#include "Thread.hpp"
#include <unistd.h>

#include <sys/sysinfo.h>
#include <sys/syscall.h>
#include "socket.hpp"
#include <ctime>

void ThreadFactory::startThreadFactory(const config& cfg)
{
    m_cfg = cfg;
}

void ThreadFactory::makeThread(vector<shared_ptr<thread>> &vecThread)
{

    int socketPerThread = m_cfg.m_vecUrl.size()/m_cfg.m_iThreadNum;
    int socketRemain = m_cfg.m_vecUrl.size() % m_cfg.m_iThreadNum;
    LOG_INFO("per is "<< socketPerThread<< " and remain is"<< socketRemain);
    auto itr = m_cfg.m_vecUrl.begin();
    int i = 0;
            int j = 0;
    //start threads
    for (; i < m_cfg.m_iThreadNum; i++)
    {
        auto itrBegin = itr;

        //iterate to find last socket that this thread needs.
        if (i < socketRemain)
        {
            for (auto j = 0; j < socketPerThread; j++)
                itr++;
            itr++;
        }
        else{
            for (auto j = 0; j < socketPerThread; j++)
                itr++;
        }

        shared_ptr<thread> pThread(new thread(Thread::ThreadWork, std::ref(m_cfg.m_vecUrl), itrBegin, itr, j++));
        usleep(100*1000);
        vecThread.push_back(pThread);
    }
    
    //wait all threads to stop.
    for (auto i = vecThread.begin(); i != vecThread.end();)
    {
        (*i)->join();
        vecThread.erase(i);
        i = vecThread.begin();
    }

   
}
/*
shared_ptr<ThreadFactory> ThreadFactory::getThreadFactorySingleton()
{
    if (m_pThreadFactory == NULL)
    {
        m_pThreadFactory.reset(new ThreadFactory());
    }

    return m_pThreadFactory;

}*/

ThreadFactory::ThreadFactory()
{
    
}

//end is included
void Thread::ThreadWork(vector<Endpoint> &vecEndpoint, const vector<Endpoint>::iterator begin, const vector<Endpoint>::iterator end, const int cpuNum)
{
    			cpu_set_t mask;

				CPU_ZERO(&mask);
				CPU_SET(cpuNum, &mask);
				pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask);    
    shared_ptr<Thread> pThread(new Thread(vecEndpoint, begin, end));
    pThread->run();
    
}

Thread::Thread(vector<Endpoint> &vecEndpoint, const vector<Endpoint>::iterator begin, const vector<Endpoint>::iterator end)
{
    int tid = syscall(SYS_gettid);

    vector<shared_ptr<Socket>> vecSocket;
    //connect socket 
    for(auto itr = begin; itr != end; itr++)
    {
        shared_ptr<Socket> s(new Socket(*itr, this));
        vecSocket.push_back(s);
        m_lstCommand.push_back(s);
    }
    LOG_INFO("tid " << tid << " is handling "<< vecSocket.size()<<" sockets.");
}

int Thread::run()
{
    while(m_lstCommand.size() > 0)
    {
        auto com = std::move(m_lstCommand.front());
        m_lstCommand.pop_front();
        com->run();
    }

    return 0;
}
