#ifndef __H_CONFIGPARSER__
#define __H_CONFIGPARSER__

#include "common.hpp"

typedef struct 
{
    int m_iThreadNum;
    vector<Endpoint> m_vecUrl;
} config;

class configParser{
    public:
        static bool parse(config&);
};

#endif