#include "common.hpp"
#include "configParser.hpp"
#include "Thread.hpp"
int main()
{

    //parse config file
    config cfg;
    if(!configParser::parse(cfg))
    {
        LOG_ERROR("cannot get config");
        return 0;
    }
    LOG_INFO("==========  Parse config success! ===========");

    shared_ptr<ThreadFactory> threadFacory(new ThreadFactory());

    threadFacory->startThreadFactory(cfg);

    vector<shared_ptr<thread>> vecThread;
    threadFacory->makeThread(vecThread);

    return 0;
}