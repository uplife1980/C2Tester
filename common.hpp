#ifndef __H_COMMON__
#define __H_COMMON__

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <thread>
#include <list>

using std::cout;
using std::endl;
using std::shared_ptr;
using std::make_shared;
using std::string;
using std::thread;
using std::vector;
using std::list;

#define LOG_ERROR(CONTENT) cout<<"[ERROR]  "<<CONTENT<<endl;
#define LOG_WARN(CONTENT) cout << "[WARN]  " << CONTENT << endl;
#define LOG_INFO(CONTENT) cout << "[INFO]  " << CONTENT << endl;

typedef struct
{
    string strAddr;
    string strPort;
    string strOtherPara;
} Endpoint;

namespace basefw
{
    bool string2EndpointPara(const string &src, Endpoint &endpoint);
};

class Command{
    public:
        virtual int run() = 0;
};
#endif
