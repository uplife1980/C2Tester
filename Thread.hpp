#ifndef __H_THREAD__
#define __H_THREAD__

#include "common.hpp"
#include "configParser.hpp"

class ThreadFactory
{
    public:
        void startThreadFactory(const config &);
        void makeThread(vector<shared_ptr<thread>>&);
        //static shared_ptr<ThreadFactory> getThreadFactorySingleton();
        ThreadFactory();

    private:
        
        config m_cfg;
        //static shared_ptr<ThreadFactory> m_pThreadFactory;
};


class Thread
{
    public:
        static void ThreadWork(vector<Endpoint> &, const vector<Endpoint>::iterator begin, const vector<Endpoint>::iterator end, const int);
};
#endif