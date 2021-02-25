#include <fstream>
#include "configParser.hpp"
using std::fstream;
using std::ios;
using namespace basefw;
bool configParser::parse(config &Config)
{
    bool ret = true;

    do
    {
        fstream fs("config.txt", ios::in);
        if(!fs.is_open())
        {
            ret = false;
            LOG_ERROR("cannot open config file!");
            break;
        }

        try
        {
            string strThreadNum;
            fs >> strThreadNum;
            std::string::size_type sz;
            Config.m_iThreadNum = std::stoi(strThreadNum, &sz);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        } 
        LOG_INFO("thread number is " << Config.m_iThreadNum);


        string strTempUrl;
        Endpoint endpoint;
        while(!fs.eof() && fs.good())
        {
            fs >> strTempUrl;
            if(!basefw::string2EndpointPara(strTempUrl, endpoint))
            {
                continue;
            }
            Config.m_vecUrl.push_back(endpoint);
            LOG_INFO("endpoint: "<< endpoint.strAddr<<" port: "<< endpoint.strPort<<" other para is : "<<endpoint.strOtherPara);
        }

        fs.close();
    } while (0);
    
    

    return ret;
}