
#include "Telnet.h"

//---------------------------------------------------------------------------
#include <winsock2.h>
#include <string.h>
#include "WSOCKET.h"
#include <stdio.h>
//---------------------------------------------------------------------------


/*------------------------------------------------------------------------
 *   Function: Mess
 *    Purpose: The function to add message to the memo of procedure.
 * Parameters:
 *      Input: str   - Added string.
 *     Output: None.
 *    Returns: None.
 *       Note: None.
 *------------------------------------------------------------------------
 */
/*void Telnet::Mess(const AnsiString &str)
{
    if(MemoTest != NULL)
    {
        MemoTest->Lines->Add(str);
    }
}*/

/*------------------------------------------------------------------------
 *   Function: TEL_User_Break
 *    Purpose: The function to check TEL_User_Break
 * Parameters:
 *      Input: None.
 *     Output: None.
 *    Returns: TRUE - break / FALSE - no
 *       Note: None.
 *------------------------------------------------------------------------
 */
bool CTelnet::fnTelUserBreak(void)const
{
    
    if(tel_break_flag)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*------------------------------------------------------------------------
 *   Function: Thread_Process_TELNET_Rx
 *    Purpose: The thread function to process TELNET port receiving data.
 *             Only process Read_Port.
 * Parameters:
 *      Input: None.
 *     Output: None.
 *    Returns: 0.
 *       Note: Reference Win32 API SDK (search "CreateThread").
 *             You can find in BCB Help->Windows SDK.
 *             1. Receiving TELNET port data and store to Telnet_Rx_QUE[]
 *                and Telnet_Rx_Memo.
 *             2. If locked store to temp.
 *------------------------------------------------------------------------
 */

unsigned long __stdcall CTelnet::fnThreadProcessTelnetRx(LPVOID *pParm)
{
    bool        have_data_flag;
    std::string  RxBuf;
    //int         memo_lastline_idx;
   // AnsiString  memo_lastline_str;
    CTelnet *pTelnet = (CTelnet *)pParm;


    pTelnet->thread_rx_running_flag = true;
    while(!pTelnet->close_rx_thread_flag)
    {
        Sleep(1);

        try
        {
            if(!pTelnet->Telnet_Connected_flag)
            {
                continue;
            }

            //FormMain->StatusBar->Panels->Items[2]->Text = IntToStr(Telnet_Rx_QUE.Length());

            have_data_flag = pTelnet->TelSocket.WSOCKET_Receive_String(pTelnet->Socket_Telnet, &RxBuf);//have_data_flag = WSOCKET_Receive_String(Socket_Telnet, &RxBuf);

            if(have_data_flag)
            {
				//调用回调函数
				if (0 != pTelnet->m_pfnCommCallBack)
				{
					pTelnet->m_pfnCommCallBack((unsigned long)RxBuf.c_str(),pTelnet->m_ulCallBackParam2);
				}
                /* Add to Telnet_Rx_QUE */
                if(!pTelnet->Telnet_Rx_QUE_Lock_flag)
                {
                    pTelnet->Telnet_Rx_QUE_Lock_flag = true;
                    if(pTelnet->Telnet_Rx_QUE_Temp.length() > 0)
                    {
                        pTelnet->Telnet_Rx_QUE += pTelnet->Telnet_Rx_QUE_Temp;
                        pTelnet->Telnet_Rx_QUE_Temp = "";
                    }

                    pTelnet->Telnet_Rx_QUE += RxBuf;
                    pTelnet->Telnet_Rx_QUE_Lock_flag = false;
                }
                else
                {
                    pTelnet->Telnet_Rx_QUE_Temp += RxBuf;
                }

                /* Add to Memo */
                /*memo_lastline_idx = Memo->Lines->Count - 1;
                memo_lastline_str = Memo->Lines->Strings[memo_lastline_idx];
                Memo->Lines->Strings[memo_lastline_idx] = memo_lastline_str + RxBuf;
                memo_lastline_str.SetLength(0);*/
                
             }
             else
             {
                /* Move to Telnet_Rx_QUE */
                if(pTelnet->Telnet_Rx_QUE_Temp.length() > 0)
                {
                    if(!pTelnet->Telnet_Rx_QUE_Lock_flag)
                    {
                        pTelnet->Telnet_Rx_QUE_Lock_flag = true;
                        pTelnet->Telnet_Rx_QUE += pTelnet->Telnet_Rx_QUE_Temp;
                        pTelnet->Telnet_Rx_QUE_Temp = "";
                        pTelnet->Telnet_Rx_QUE_Lock_flag = false;
                    }
                }
            }
        }

//         catch (CException exception)
//         {
//             /*Mess("===[TELNET RX Exception]===");
//             Mess(exception.Message);
//             Mess("===========================");*/
// #if DEBUG_MODE
//             *tel_break_flag = true;
// #endif
//         }
// 
        catch (...)
        {
            /*Mess("===[TELNET RX Exception]===");
            Mess("=    Unknow Exception !   =");
            Mess("===========================");*/
#if DEBUG_MODE
            pTelnet->tel_break_flag = true;
#endif
        }
    }

    pTelnet->thread_rx_running_flag = false;
    return 0;
}

/*------------------------------------------------------------------------
 *   Function: TELNET_Create_Thread
 *    Purpose: The function to Create Threads both of Processing Rx and Memo.
 * Parameters:
 *      Input: None.
 *     Output: None.
 *    Returns: None.
 *       Note: Reference Win32 API SDK (Search "CreateThread").
 *             You can find in BCB Help->Windows SDK.
 *             Set both close_thread_flag and create_thread_flag.
 *------------------------------------------------------------------------
 */
void CTelnet::TELNET_Create_Thread(void)
{
    if(!create_thread_flag)
    {
        close_rx_thread_flag = false;

        hThread_rx = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)fnThreadProcessTelnetRx,(LPVOID)this, 0, &ThreadID_rx);
        if(hThread_rx == NULL)
        {
            close_rx_thread_flag = true;
            create_thread_flag = false;
            //Mess("Create Thread for TELNET RX FAIL!");
        }
        else
        {
            create_thread_flag = true;
            SetThreadPriority(hThread_rx, THREAD_PRIORITY_NORMAL);
        }
    }
}
/*------------------------------------------------------------------------
 *   Function: TELNET_Close_Thread
 *    Purpose: The function to close Threads both of Processing Rx.
 * Parameters:
 *      Input: None.
 *     Output: None.
 *    Returns: None.
 *       Note: Reference Win32 API SDK (Search "CreateThread").
 *             You can find in BCB Help->Windows SDK.
 *             Set both close_thread_flag and create_thread_flag.
 *------------------------------------------------------------------------
 */
void CTelnet::TELNET_Close_Thread(void)
{
    unsigned long   exit_code;
    unsigned long   rt;

    if(create_thread_flag)
    {
        /* Wait Thread proccess end. */
        close_rx_thread_flag = true;
        do
        {
            Sleep(1);
        }while(thread_rx_running_flag);

        /* wait for Thread singled */
        do
        {
            rt = WaitForSingleObject(hThread_rx, 1);
        }while(rt == WAIT_TIMEOUT);

        GetExitCodeThread(hThread_rx, &exit_code);
        TerminateThread(hThread_rx, exit_code);
        CloseHandle( hThread_rx );
		hThread_rx = NULL;
        create_thread_flag = false;
    }
}
/*------------------------------------------------------------------------
 *   Function: TELNET_Init_Buffer
 *    Purpose: The function to clear all variables of Telnet.
 * Parameters:
 *      Input: None.
 *     Output: None.
 *    Returns: None.
 *       Note: None.
 *------------------------------------------------------------------------
 */
void CTelnet::TELNET_Init_Buffer(void)
{
    Telnet_Rx_QUE = "";
    Telnet_Rx_QUE_Temp = "";
    Telnet_Rx_QUE_Lock_flag = false;
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*------------------------------------------------------------------------
 *   Function: TELNET_Initial
 *    Purpose: The function to inital variables and flags.
 * Parameters:
 *      Input: None.
 *     Output: None.
 *    Returns: None.
 *       Note: Before using founctions of the module, user must execute
 *             this finction (just once).
 *------------------------------------------------------------------------
 */
void CTelnet::TELNET_Initial(/*TMemo *memo_tel, TMemo *memo_pce,*/ bool break_flag)
{
    TelSocket.WSOCKET_Initialize();//WSOCKET_Initialize();

    /* To appoint the pointer of memo */
    //Memo    = memo_tel;
    //MemoTest = memo_pce;
    tel_break_flag = break_flag;

    /* Inital variables and flags */
    thread_rx_running_flag = false;
    create_thread_flag = false;
}

/*------------------------------------------------------------------------
 *   Function: TELNET_SendChar
 *    Purpose: The function to send string to TELNET.
 * Parameters:
 *      Input: telnet_port - Send TELNET port.
 *             Command  - Send string.
 *     Output: None.
 *    Returns: None.
 *       Note: 1. set Read_Port.
 *             2. Call sio_write() function of PComm.dll
 *------------------------------------------------------------------------
 */
void CTelnet::TELNET_SendChar(const std::string &Command)
{
    if(!Telnet_Connected_flag)
    {
        return;
    }
    TelSocket.WSOCKET_Send_String(Socket_Telnet, Command);//WSOCKET_Send_String(Socket_Telnet, Command);
}


/*------------------------------------------------------------------------
 *   Function: TELNET_SendString
 *    Purpose: The function to send string to TELNET.
 * Parameters:
 *      Input: telnet_port - Send TELNET port.
 *             Command  - Send string.
 *     Output: None.
 *    Returns: None.
 *       Note: 1. set Read_Port.
 *             2. Added '\r' to the end of string.
 *             4. Frush TELNET port out buffer.
 *             3. Call sio_write() function of PComm.dll
 *------------------------------------------------------------------------
 */
void CTelnet::TELNET_SendString(std::string Command)
{
    /*if(Telnet_Connected_flag == false)
    {
        return;
    }
    WSOCKET_Send_String(Socket_Telnet, Command + "\r"); */
    TELNET_SendChar(Command);
//     TELNET_SendChar('\r');
}

// llg add for send hex
void CTelnet::TELNET_SendString_hex(char * str,int length)
{
    if(!Telnet_Connected_flag)
    {
        return;
    }
    TelSocket.WSOCKET_Send_String_hex(Socket_Telnet, str ,length);//WSOCKET_Send_String_hex(Socket_Telnet, str ,length);
}



/*------------------------------------------------------------------------
 *   Function: TELNET_GetString
 *    Purpose: The function to get specific string in Telnet_Rx_QUE.
 * Parameters:
 *      Input: telnet_port    - The TELNET port of finding string.
 *             Delaytime   - The max time of finding string.
 *             findstring  - Searched string.
 *             *cap_string - Points to a AnsiString variable to store
 *                           the acquired string.
 *             String_Len  - Number of characters acquired.
 *     Output: The acquired string will store to cap_string.
 *    Returns: TRUE - Success. / FALSE - Fail (When time out).
 *       Note: If found, delete Telnet_Rx_QUE from begins to the string
 *             of found position.
 *------------------------------------------------------------------------
 */
bool CTelnet::TELNET_GetString(unsigned long Delaytime, const std::string &findstring, std::string *cap_string, int String_Len)
{
    int     str_ptr;
    int     iiilen;


    if(!Telnet_Connected_flag)
    {
        return false;
    }

    Tel_tick_start = ::GetTickCount();
    do
    {
        Sleep(1);

        if(fnTelUserBreak())
        {
            return false;
        }

        if(!Telnet_Rx_QUE_Lock_flag)
        {
            Telnet_Rx_QUE_Lock_flag = true;
            str_ptr = Telnet_Rx_QUE.find(findstring);
            if(str_ptr >= 0)
            {
                iiilen = Telnet_Rx_QUE.length() - str_ptr - findstring.length();// - String_Len;
                if(iiilen > 0)
                {
                    *cap_string = Telnet_Rx_QUE.substr(str_ptr + findstring.length(), String_Len);
                    Telnet_Rx_QUE.erase(1, str_ptr + findstring.length() - 1);
                    Telnet_Rx_QUE_Lock_flag = false;
                    return true;
                }
            }

            Telnet_Rx_QUE_Lock_flag = false;
        }
        Tel_tick_end = ::GetTickCount();

    }while( (Tel_tick_end - Tel_tick_start) < Delaytime );

     return false;
}

/*------------------------------------------------------------------------
 *   Function: TELNET_DumpString
 *    Purpose: The function to get specific string in Telnet_Rx_QUE.
 * Parameters:
 *      Input: telnet_port    - The TELNET port of finding string.
 *             Delaytime   - The max time of finding string.
 *             findstring  - Searched string.
 *             *rx_que     - Points to a AnsiString variable to store
 *                           the acquired string.
 *     Output: The acquired string will store to rx_que.
 *    Returns: TRUE - Success. / FALSE - Fail (When time out).
 *       Note: If found, clear Telnet_Rx_QUE.
 *------------------------------------------------------------------------
 */
bool CTelnet::TELNET_DumpString(unsigned long Delaytime, const std::string &findstring, std::string *rx_que)
{
    int     str_ptr;
    bool    lock_que_flag;


    if(!Telnet_Connected_flag)
    {
        return false;
    }

    Tel_tick_start = ::GetTickCount();
	do
    {
        Sleep(1);
        
        if(fnTelUserBreak())
    	{
            return false;
        }
        Tel_tick_end = ::GetTickCount();

    }while( (Tel_tick_end - Tel_tick_start) < Delaytime );

    lock_que_flag = false;
    do
    {
        if(!Telnet_Rx_QUE_Lock_flag)
        {
            Telnet_Rx_QUE_Lock_flag = true;
            lock_que_flag = true;
            str_ptr = Telnet_Rx_QUE.find(findstring);
            if(0 <= str_ptr)
            {
                Telnet_Rx_QUE.erase(1, str_ptr + findstring.length()-1);
                *rx_que = Telnet_Rx_QUE;
                Telnet_Rx_QUE = "";
                Telnet_Rx_QUE_Lock_flag = false;
                return true;
            }
            Telnet_Rx_QUE_Lock_flag = false;
        }
        Sleep(1);
    }while(!lock_que_flag);

    return false;
}

/*------------------------------------------------------------------------
 *   Function: TELNET_Connect
 *    Purpose: The function to connect to telnet.
 * Parameters:
 *      Input: IP -
 *     Output: None.
 *    Returns: TRUE - Success / FALSE - Fail.
 *       Note: None.
 *------------------------------------------------------------------------
 */
bool CTelnet::TELNET_Connect(const std::string &IP, int port)
{
    bool     flag;

    //Mess("Connect to Telnet ...");

    flag = TelSocket.WSOCKET_Startup(WS_VER_2_0);//flag = WSOCKET_Startup(WS_VER_2_0);
    if(!flag)
    {
        //...
        return false;
    }

    /* Configure Socket parameters. */
    WS_IP_addr.sin_port             = htons( port );
    WS_IP_addr.sin_family           = AF_INET;
    WS_IP_addr.sin_addr.S_un.S_addr = inet_addr(IP.c_str());

    Socket_Telnet = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);      /* Create a socket. */
    if(INVALID_SOCKET == Socket_Telnet)
    {
        //Mess("Connect to Telnet Success!");
        return false;
    }

    /*struct linger so_linger;
    so_linger.l_onoff = 1;
    so_linger.l_linger = 0;
    flag = setsockopt(Socket_Telnet,SOL_SOCKET,SO_LINGER,(char *)&so_linger, sizeof(so_linger));
    if(flag==false)
    {

        return false;
    }*/

    // Setting Sending Timeout.
    flag = TelSocket.WSOCKET_Set_Send_TimeOut(Socket_Telnet, TELNET_TX_TIMEOUT);//flag = WSOCKET_Set_Send_TimeOut(Socket_Telnet, TELNET_TX_TIMEOUT);
    if(!flag)
    {
        //Mess("Connect to Telnet Success!");
        closesocket(Socket_Telnet);
        return false;
    }

    // Setting Receive Timeout.
    flag = TelSocket.WSOCKET_Set_Receive_TimeOut(Socket_Telnet, TELNET_RX_TIMEOUT);//flag = WSOCKET_Set_Receive_TimeOut(Socket_Telnet, TELNET_RX_TIMEOUT);
    if(!flag)
    {
        //Mess("Connect to Telnet Success!");
        closesocket(Socket_Telnet);
        return false;
    }

    if( connect(Socket_Telnet,(LPSOCKADDR)&WS_IP_addr,sizeof(SOCKADDR)) == SOCKET_ERROR )
    {
        closesocket(Socket_Telnet);
        Socket_Telnet = INVALID_SOCKET;
        return false;
    }
    Telnet_Connected_flag = true;

    TELNET_Init_Buffer();
    TELNET_Create_Thread();

    //Mess("Connect to Telnet Success!");
    return true;
}

/*------------------------------------------------------------------------
 *   Function: TELNET_Disconnect
 *    Purpose: The function to disconnect telnet.
 * Parameters:
 *      Input: ComPort  -
 *     Output: None.
 *    Returns: None.
 *       Note: None.
 *------------------------------------------------------------------------
 */
void CTelnet::TELNET_Disconnect(void)
{
    if(!Telnet_Connected_flag)
    {
        return;
    }

    TELNET_Close_Thread();

    if(INVALID_SOCKET != Socket_Telnet)
    {
        closesocket(Socket_Telnet);
    }
    TelSocket.WSOCKET_Cleanup();//WSOCKET_Cleanup();
    Telnet_Connected_flag = false;
    //Mess("Telnet disconnected !");
}
/*------------------------------------------------------------------------
 *   Function: TELNET_Clear_RxBuf
 *    Purpose: The function to clear Rx Buffer of Telnet.
 * Parameters:
 *      Input: None.
 *     Output: None.
 *    Returns: None.
 *       Note: None.
 *------------------------------------------------------------------------
 */
void CTelnet::TELNET_Clear_RxBuf(void)
{
    while(Telnet_Rx_QUE_Lock_flag)
    {
        Sleep(1);
    }
    Telnet_Rx_QUE_Lock_flag = true;
    Telnet_Rx_QUE = "";
    Telnet_Rx_QUE_Temp = "";
    Telnet_Rx_QUE_Lock_flag = false;
}
/*------------------------------------------------------------------------
 *   Function: TELNET_String_Replace
 *    Purpose: The function to replaces occurrences of the substring
 *             specified by old_str with the substring specified by new_str.
 * Parameters:
 *      Input: *str    - str is the pointer of source string, whose substrings are changed.
 *             old_str - old_str is the substring to locate and replace with new_str.
 *             new_str - new_str is the substring to substitute for occurrences of old_str.
 *     Output: str.
 *    Returns: None.
 *       Note: None.
 *------------------------------------------------------------------------
 */
void CTelnet::TELNET_String_Replace(std::string *str, const std::string &old_str, const std::string &new_str)
{
    int         old_len;
    int         strange_str_len;
    int         ptr;
    std::string  strange_str;


    strange_str = "!#~?";
    old_len = old_str.length();
    strange_str_len = strange_str.length();
    do
    {
        ptr = str->find(old_str);
        if(0 < ptr)
        {
            str->erase(ptr, old_len);
            str->insert(ptr,strange_str);
        }
    }while(0 < ptr);

    do
    {
        ptr = str->find(strange_str);
        if(0 < ptr)
        {
            str->erase(ptr, strange_str_len);
            str->insert(ptr,new_str);
        }
    }while(0 < ptr);

}


bool CTelnet::TELNET_StoreString(unsigned long Delaytime, std::string findstring, std::string *storestring)
{
    int     str_ptr;

    if(!Telnet_Connected_flag)
    {
        return false;
    }

    Tel_tick_start = ::GetTickCount();
    do
    {
        Sleep(1);

        if(fnTelUserBreak())
        {
    	    return false;
        }

        if(!Telnet_Rx_QUE_Lock_flag)
        {
            Telnet_Rx_QUE_Lock_flag = true;
            str_ptr = Telnet_Rx_QUE.find(findstring);
            if( str_ptr >= 0 )
        	{
                *storestring = Telnet_Rx_QUE;
                Telnet_Rx_QUE.erase(1, str_ptr + findstring.length()-1);
                Telnet_Rx_QUE_Lock_flag = false;
                return true;
            }

            Telnet_Rx_QUE_Lock_flag = false;
        }
        Tel_tick_end = ::GetTickCount();

    }while( (Tel_tick_end - Tel_tick_start) < Delaytime );

    return true;
}

bool CTelnet::TELNET_RevString(std::string *cap_string)
{


    if(!Telnet_Connected_flag)
    {
        return false;
    }
	
	if(fnTelUserBreak())
	{
		return false;
	}
	
	if(!Telnet_Rx_QUE_Lock_flag)
	{
		Telnet_Rx_QUE_Lock_flag = true;
		*cap_string = Telnet_Rx_QUE;
		Telnet_Rx_QUE = "";
		if ("" == *cap_string)
		{			
			Telnet_Rx_QUE_Lock_flag = false;
			return false;
		}
		
		Telnet_Rx_QUE_Lock_flag = false;
	}

     return true;
}

/*------------------------------------------------------------------------
 *   Function: TELNET_Login
 *    Purpose: The function to Login to telnet.
 * Parameters:
 *      Input: IP -
 *     Output: None.
 *    Returns: TRUE - Success / FALSE - Fail.
 *       Note: None.
 *------------------------------------------------------------------------
  */

// bool CTelnet::TELNET_Login (std::string Login_Prompt,std::string Login,std::string Password_Prompt,std::string Password,std::string Prompt)
// {
//     bool flag;
//     for(int i = 0 ; i < 5 ; i++)
//     {
//         //flag = TELNET_FindString(5000,Login_Prompt);
//         flag = TELNET_FindString(3000,Login_Prompt);
//         if(flag)
//         {
//             break;
//         }
//         else
//         {
//             TELNET_SendString("");
//             continue;
//         }
//     }
// 
//     if(flag)
//     {
//         TELNET_SendString(Login);
//         //flag = TELNET_FindString(5000,Password_Prompt);
//         flag = TELNET_FindString(3000,Password_Prompt);
//     }
//     if(flag)
//     {
//         TELNET_SendString(Password);
//         flag = true;
//         //flag = TELNET_FindString(5000,Prompt);
//     }
//     return flag;
// }

/*************************************************************************
功  能:  保存一定时间内的telnet返回的字符串
参  数： 延时时间
返回值:  得到的字符串
*************************************************************************/
std::string CTelnet::TELNET_SaveString(unsigned long Delaytime)
{
    std::string SaveString;
    if(!Telnet_Connected_flag)
    {
        return NULL;
    }
    Tel_tick_start = ::GetTickCount();
    do
    {
        Sleep(1);
        if(fnTelUserBreak())
        {
    	    return NULL;
        }
        if(!Telnet_Rx_QUE_Lock_flag)
        {
            Telnet_Rx_QUE_Lock_flag = true;
            SaveString = Telnet_Rx_QUE;
            Telnet_Rx_QUE_Lock_flag = false;
        }
        Tel_tick_end = ::GetTickCount();

    }while( (Tel_tick_end - Tel_tick_start) < Delaytime );

    return SaveString;
}

CTelnet::CTelnet()
{
	m_pfnCommCallBack = 0;
	hThread_rx = 0;
}

CTelnet::~CTelnet()
{
	if (0 != hThread_rx)
	{
		CloseHandle(hThread_rx);
		hThread_rx = 0;
	}
}
