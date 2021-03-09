#ifndef __H_THREAD__
#define __H_THREAD__

#include "common.hpp"
#include "configParser.hpp"

class Command;
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


class Thread: public Command, public std::enable_shared_from_this<Thread>
{
    public:
        static void ThreadWork(vector<Endpoint> &, const vector<Endpoint>::iterator begin, const vector<Endpoint>::iterator end, const int);


        Thread(vector<Endpoint> &vecEndpoint, const vector<Endpoint>::iterator begin, const vector<Endpoint>::iterator end);
        //Thread(Thread* th){};

        void addCommand(shared_ptr<Command> com){m_lstCommand.push_back((com));}
        virtual int run();
    private:
        list<shared_ptr<Command>> m_lstCommand;
};
#endif