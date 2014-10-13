//---------------------------------------------------------------------------

#include "WSOCKET.h"

//---------------------------------------------------------------------------
#pragma comment(lib,"ws2_32.lib")

/*WSOCKET::WSOCKET(void)
{
    //winsock_startup_flag = false;
    //ws_recv_buf[WS_RCV_BUF_LEN] ={'\0};
}
WSOCKET::~WSOCKET(void)
{

}*/
void WSOCKET::WSOCKET_Initialize(void)
{
    winsock_startup_flag = false;
}

//---------------------------------------------------------------------------
bool WSOCKET::WSOCKET_Startup(WORD version)
{
    int ws_return;

    if(winsock_startup_flag)
    {
        return true;
    }

    ws_return = WSAStartup(version, &WS_Data);

    if(ws_return != WS_SUCCESS)
    {
        WSOCKET_POP_Error_Msg( WSAGetLastError() );
        return false;
    }
    if(WS_Data.wVersion < version)
    {
        MessageBoxA(NULL, WS_MSG_VER_NOT_SUPPORT, WS_MSG_ERROR, MB_ICONERROR );
        return false;
    }
    winsock_startup_flag = true;
    
    return true;
}


//---------------------------------------------------------------------------
bool WSOCKET::WSOCKET_Cleanup(void)
{
    int ws_return;

    if(!winsock_startup_flag)
    {
        return true;
    }

    if (WSAIsBlocking())
    {
        ws_return = WSACancelBlockingCall();
        if(WS_SUCCESS != ws_return)
        {
            WSOCKET_POP_Error_Msg( WSAGetLastError() );
            return false;
        }
    }

    ws_return = WSACleanup();
    if(WS_SUCCESS != ws_return)
    {
        WSOCKET_POP_Error_Msg( WSAGetLastError() );
        return false;
    }

    winsock_startup_flag = false;

    return true;
}


//---------------------------------------------------------------------------
void WSOCKET::WSOCKET_POP_Error_Msg(int error_code)const
{
        std::string msg;
		char buf[20] = {0};
        msg = "Error Code: ";
        msg += itoa(error_code,buf,20);
        msg +=  "\n\n";

        switch(error_code)
        {
                case 10004:    msg += WS_ERR_MSG_10004;    break;
                case 10009:    msg += WS_ERR_MSG_10009;    break;
                case 10013:    msg += WS_ERR_MSG_10013;    break;
                case 10014:    msg += WS_ERR_MSG_10014;    break;
                case 10022:    msg += WS_ERR_MSG_10022;    break;
                case 10024:    msg += WS_ERR_MSG_10024;    break;
                case 10035:    msg += WS_ERR_MSG_10035;    break;
                case 10036:    msg += WS_ERR_MSG_10036;    break;
                case 10037:    msg += WS_ERR_MSG_10037;    break;
                case 10038:    msg += WS_ERR_MSG_10038;    break;
                case 10039:    msg += WS_ERR_MSG_10039;    break;
                case 10040:    msg += WS_ERR_MSG_10040;    break;
                case 10041:    msg += WS_ERR_MSG_10041;    break;
                case 10042:    msg += WS_ERR_MSG_10042;    break;
                case 10043:    msg += WS_ERR_MSG_10043;    break;
                case 10044:    msg += WS_ERR_MSG_10044;    break;
                case 10045:    msg += WS_ERR_MSG_10045;    break;
                case 10046:    msg += WS_ERR_MSG_10046;    break;
                case 10047:    msg += WS_ERR_MSG_10047;    break;
                case 10048:    msg += WS_ERR_MSG_10048;    break;
                case 10049:    msg += WS_ERR_MSG_10049;    break;
                case 10050:    msg += WS_ERR_MSG_10050;    break;
                case 10051:    msg += WS_ERR_MSG_10051;    break;
                case 10052:    msg += WS_ERR_MSG_10052;    break;
                case 10053:    msg += WS_ERR_MSG_10053;    break;
                case 10054:    msg += WS_ERR_MSG_10054;    break;
                case 10055:    msg += WS_ERR_MSG_10055;    break;
                case 10056:    msg += WS_ERR_MSG_10056;    break;
                case 10057:    msg += WS_ERR_MSG_10057;    break;
                case 10058:    msg += WS_ERR_MSG_10058;    break;
                case 10059:    msg += WS_ERR_MSG_10059;    break;
                case 10060:    msg += WS_ERR_MSG_10060;    break;
                case 10061:    msg += WS_ERR_MSG_10061;    break;
                case 10062:    msg += WS_ERR_MSG_10062;    break;
                case 10063:    msg += WS_ERR_MSG_10063;    break;
                case 10064:    msg += WS_ERR_MSG_10064;    break;
                case 10065:    msg += WS_ERR_MSG_10065;    break;
                case 10066:    msg += WS_ERR_MSG_10066;    break;
                case 10067:    msg += WS_ERR_MSG_10067;    break;
                case 10068:    msg += WS_ERR_MSG_10068;    break;
                case 10069:    msg += WS_ERR_MSG_10069;    break;
                case 10070:    msg += WS_ERR_MSG_10070;    break;
                case 10071:    msg += WS_ERR_MSG_10071;    break;
                case 10091:    msg += WS_ERR_MSG_10091;    break;
                case 10092:    msg += WS_ERR_MSG_10092;    break;
                case 10093:    msg += WS_ERR_MSG_10093;    break;
                case 10101:    msg += WS_ERR_MSG_10101;    break;
                case 11001:    msg += WS_ERR_MSG_11001;    break;
                case 11002:    msg += WS_ERR_MSG_11002;    break;
                case 11003:    msg += WS_ERR_MSG_11003;    break;
                case 11004:    msg += WS_ERR_MSG_11004;    break;
                default:       msg += WS_ERR_MSG_UNKNOW;   break;
        }
        MessageBoxA(NULL, msg.c_str(), WS_MSG_ERROR, MB_ICONERROR );
}


//---------------------------------------------------------------------------
std::string WSOCKET::WSOCKET_Get_Port(SOCKET aSocket)
{
    struct sockaddr_in   AddrIn;
    int                  iReturn;
    int                  AddrLen ;


    AddrLen = sizeof(AddrIn) ;
    iReturn = getsockname(aSocket, (struct sockaddr *)&AddrIn, &AddrLen);
    if (SOCKET_ERROR == iReturn)
    {
                WSOCKET_POP_Error_Msg( WSAGetLastError() );
                return 0;
    }
    else
    {
        // 將16 位元短整數(short integer)
        // 從網路位元順序(network byte order)轉換至
        // 主機位元順序(host byte order)
		char buf[20] = {0};
		std::string strPort = itoa(ntohs(AddrIn.sin_port),buf,20);
        return strPort;
    }
}


//---------------------------------------------------------------------------
std::string WSOCKET::WSOCKET_Get_Peer_IP(SOCKET aSocket)
{
    sockaddr_in    AddrIn;
    int                   iReturn;
    int                   AddrLen ;
	std::string		strResult = "";

    AddrLen = sizeof(AddrIn) ;
    iReturn = getpeername(aSocket, (sockaddr *)&AddrIn, &AddrLen);
    if (SOCKET_ERROR == iReturn)
    {
        WSOCKET_POP_Error_Msg( WSAGetLastError() );
        return strResult;
    }
    else
    {
		strResult = inet_ntoa(AddrIn.sin_addr);
        return strResult;
    }
}


//---------------------------------------------------------------------------
std::string WSOCKET::WSOCKET_Get_Host_Name(SOCKET aSocket)
{
    hostent     *remote_host;
    unsigned    int ip_addr;
	std::string strResult = "";
    
    ip_addr = inet_addr( WSOCKET_Get_Peer_IP(aSocket).c_str()  );
    remote_host = gethostbyaddr( (char *)&ip_addr, sizeof(ip_addr), AF_INET );
    if(WSAGetLastError() != 0)
    {
        WSOCKET_POP_Error_Msg( WSAGetLastError() );
        return strResult;
    }
	strResult = remote_host->h_name;
    return strResult;
}



//---------------------------------------------------------------------------
std::string WSOCKET::WSOCKET_Get_Local_IP(void)
{
    char        hostname[128];
    hostent    *tmp_addr;
	std::string strResult = "";

    if(gethostname(hostname, sizeof(hostname)) != WS_SUCCESS)
    {
        WSOCKET_POP_Error_Msg( WSAGetLastError() );
        return strResult;
    }
    tmp_addr = gethostbyname(hostname);
	strResult = inet_ntoa(*((in_addr *)tmp_addr->h_addr));
    return strResult;
}


//---------------------------------------------------------------------------
void WSOCKET::WSOCKET_Send_String(SOCKET aSocket, std::string str)
{
    int      ilength ;
    char*    Buff;

    if (INVALID_SOCKET != aSocket)
    {
        ilength = str.length();
        Buff = new char[ilength + 1];
		memset(Buff,0,ilength + 1);
        strcpy(Buff, str.c_str());
        send(aSocket, Buff, ilength, 0);
        delete[] Buff;
    }
}



//---------------------------------------------------------------------------
void WSOCKET::WSOCKET_Send_String_hex(SOCKET aSocket, char *chastr,int length)
{
    if (INVALID_SOCKET != aSocket)
    {
        send(aSocket, chastr, length, 0);
    }

}


//---------------------------------------------------------------------------
bool WSOCKET::WSOCKET_Receive_String(SOCKET aSocket, std::string *rcv_que)
{
    int    recv_byte;

    memset(ws_recv_buf,0,sizeof(char)*WS_RCV_BUF_LEN);
    *rcv_que = "";

    recv_byte = recv(aSocket, ws_recv_buf, WS_RCV_BUF_LEN-1, 0);
//FCT_Procedure_Msg(IntToStr(recv_byte));
    if(recv_byte == 0 || recv_byte == WSAECONNRESET || recv_byte == SOCKET_ERROR)
    {
        return false;
    }
    else
    {
#if WS_PROCESS_NULL_CHAR
        for(int i=0; i<WS_RCV_BUF_LEN-1; i++)
        {
            if(ws_recv_buf[i] == '\0' && ws_recv_buf[i+1] == '\0')
            {
                break;
            }
            
            if(ws_recv_buf[i] == '\0')
            {
                ws_recv_buf[i] = ' ';
            }
        }
#endif

        *rcv_que = ws_recv_buf;
        return true;
    }
}



//---------------------------------------------------------------------------
bool WSOCKET::WSOCKET_Set_Send_TimeOut(SOCKET aSocket, int iSendout)
{
    int     ws_return;

    ws_return = setsockopt( aSocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&iSendout, sizeof(iSendout) );
    if(ws_return != WS_SUCCESS)
    {
        WSOCKET_POP_Error_Msg( WSAGetLastError() );
        return false;
    }
    else
    {
        return true;
    }
}



//---------------------------------------------------------------------------
bool WSOCKET::WSOCKET_Set_Receive_TimeOut(SOCKET aSocket, int iRcvout)
{
    int     ws_return;
    
    ws_return = setsockopt( aSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&iRcvout, sizeof(iRcvout) );
    if(ws_return != WS_SUCCESS)
    {
        WSOCKET_POP_Error_Msg( WSAGetLastError() );
        return false;
    }
    else
    {
        return true;
    }
}



 
