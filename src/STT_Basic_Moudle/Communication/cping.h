#ifndef CPING_H
#define CPING_H

#include "WSOCKET.h"

class CPing
{
public:
 CPing(void);
 ~CPing(void);

 bool CanPing(const char* pDscAddr);
 bool Ping(const char* pDscAddr,const char* pSscAddr=NULL, int nPackNum=4 ,int nSuccessNum=1 , int nDataSize=64);

private:
 bool Init();
 bool FInit();
 bool ConstructIcmpMessage(char* pDataBuff, int nDataSize);
 USHORT CheckSum(USHORT* pBuff, int nSize);
 bool DecodeIpPacket(char* pBuff, int nBuffSize, sockaddr_in* pAddr);
 bool PingHelper(const char* pDscAddr,const char* pSscAddr=NULL, int nPackNum=4 ,int nSuccessNum=1 , int nDataSize=64);

private:
 SOCKET m_socketRaw;
 bool m_bPrintInfo;
};

#endif // CPING_H
