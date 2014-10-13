#pragma once
//---------------------------------------------------------------------------
#include "WSOCKET.h"
#include "Communication.h"
//---------------------------------------------------------------------------
#define      TELNET_PORT                         23
#define      TELNET_TX_TIMEOUT                   1000    //(ms)
#define      TELNET_RX_TIMEOUT                   3000     //(ms)

//---------------------------------------------------------------------------

class CTelnet
{
private: 
	WS_SOCKADDR   WS_IP_addr;

	fnCommCallBack		m_pfnCommCallBack;
	unsigned long m_ulCallBackParam2;
public:
	SOCKET        Socket_Telnet;
	std::string    Telnet_Rx_QUE;                         /* To store string of TELNET port received for FindString */
	std::string    Telnet_Rx_QUE_Temp;                    /* To store string of TELNET port received for FindString (temporary) */
	bool          Telnet_Rx_QUE_Lock_flag;               /* The flags of Array Telnet_Rx_QUE[] locked status (true=locked, false=unlocked) */

	bool          Telnet_Connected_flag;                 /* The flags of TELNET ports opened/closed status. (true=opened, false=closed) */

	/* for findstring */
	unsigned long Tel_tick_start;
	unsigned long Tel_tick_end;


	/* for Memo pointer variables */
	//static TMemo        *Memo;                           /* The pointer of TMemo for showing console data */
	//static TMemo        *MemoTest;                        /* The pointer of TMemo for showing procedure */
	bool         tel_break_flag;                        /* The pointer of TMemo for showing procedure */  


	WSOCKET       TelSocket;

	/* for thread variables */
	HANDLE        hThread_rx;                            /* The Handle of thread for processing TELNET port receiving data */
	unsigned long ThreadID_rx;                           /* The Thread ID thread for processing TELNET port receiving data */
	bool          close_rx_thread_flag;
	bool          create_thread_flag;                    /* The flag of the status of threads created (true=yes, false=no) */
	bool          thread_rx_running_flag;                /* The flag of the status of thread_rx running (true=yes, false=no) */
	static unsigned long __stdcall fnThreadProcessTelnetRx(LPVOID *pParm);

	bool fnTelUserBreak(void)const;
	void TELNET_Create_Thread(void);
	void TELNET_Close_Thread(void);
	void TELNET_Init_Buffer(void);



public:
	CTelnet();
	virtual ~CTelnet();
	//////////////////////////////////////////////////////////////////////////
	//函数说明:
	//注册回调函数
	//参数说明:
	//pFnCallBack[IN]回调函数指针
	//返回说明:
	//无
	//////////////////////////////////////////////////////////////////////////
	virtual void fnRegCallBack(const fnCommCallBack pFnCallBack,unsigned long ulParam2)
	{
		m_pfnCommCallBack = pFnCallBack;
		m_ulCallBackParam2 = ulParam2;
	}

	void TELNET_Initial(/*TMemo *memo_tel, TMemo *memo_pce,*/ bool break_flag);

	bool TELNET_Connect(const std::string &IP, int port);

	//     bool TELNET_Login (std::string Login_Prompt,std::string Login,std::string Password_Prompt,std::string Password,std::string Prompt) ;

	void TELNET_Disconnect(void);

	void TELNET_SendChar(const std::string &Command);
	void TELNET_SendString(std::string Command);

	std::string TELNET_SaveString(unsigned long Delaytime);

	void TELNET_SendString_hex(char * str,int length);


	void TELNET_Clear_RxBuf(void);

	bool TELNET_GetString(unsigned long Delaytime, const std::string &findstring, std::string *cap_string, int String_Len);
	bool TELNET_DumpString(unsigned long Delaytime, const std::string &findstring, std::string *rx_que);

	void TELNET_String_Replace(std::string *str, const std::string &old_str, const std::string &new_str);
	bool TELNET_StoreString(unsigned long Delaytime, std::string findstring, std::string *storestring);
	bool TELNET_RevString(std::string *cap_string);


};



