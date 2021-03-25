#include "common.hpp"

namespace basefw{
    string GetKeyValue(const string &strSrc, string strKeyItem, string strEndKey, int &iStartPos)
    {
        std::string strStd = strSrc;
        std::string strItems[10];

        std::string::size_type iPos1 = 0;
        std::string::size_type iPos2 = strKeyItem.find("\\", iPos1);
        std::string::size_type i = 0;

        for (i = 0; iPos2 != std::string::npos; i++)
        {
            strItems[i] = strKeyItem.substr(iPos1, iPos2 - iPos1);
            iPos1 = iPos2 + 1;
            iPos2 = strKeyItem.find("\\", iPos1);
        }

        strItems[i] = strKeyItem.substr(iPos1, strKeyItem.length() - iPos1);

        std::string::size_type iBegin = 0;
        std::string::size_type j = 0;
        for (j = 0; j <= i; j++)
        {
            iBegin = strStd.find(strItems[j], iStartPos + iBegin) + strItems[j].length();
            if (strItems[j].length() - 1 == iBegin)
            {
                return "";
            }
        }

        std::string strEnds[10];
        iPos1 = 0;
        iPos2 = strEndKey.find("|", iPos1);
        for (i = 0; iPos2 != std::string::npos; i++)
        {
            strEnds[i] = strEndKey.substr(iPos1, iPos2 - iPos1);
            iPos1 = iPos2 + 1;
            iPos2 = strEndKey.find("|", iPos1);
        }

        strEnds[i] = strEndKey.substr(iPos1, strEndKey.length() - iPos1);
        strEnds[++i] = "\r\n";
        strEnds[++i] = "\n";

        std::string::size_type iEndPos = std::string::npos;
        for (j = 0; j <= i; j++)
        {
            if (strEnds[j].empty())
            {
                iEndPos = strStd.length();
                break;
            }

            iEndPos = strStd.find(strEnds[j], iBegin);
            if (iEndPos != std::string::npos)
            {
                break;
            }
        }

        if (iEndPos == std::string::npos)
        {
            iEndPos = strSrc.size();
        }

        iStartPos = iEndPos;

        return strSrc.substr(iBegin, iEndPos - iBegin);
    }

    string GetKeyValue(const string &strSrc, string strKeyItem, string strEndKey)
    {
        int iPos = 0;
        return GetKeyValue(strSrc, strKeyItem, strEndKey, iPos);
    }


    bool string2EndpointPara(const string &src, Endpoint &endpoint)
    {
        string strAddrAndPort = GetKeyValue(src, "http://", "/");
        endpoint.strAddr = GetKeyValue(strAddrAndPort, "", ":");
        endpoint.strPort = GetKeyValue(strAddrAndPort, ":", "");
        endpoint.strOtherPara = GetKeyValue(src, strAddrAndPort, "");
        return true;
    }
    
}