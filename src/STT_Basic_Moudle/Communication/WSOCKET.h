//---------------------------------------------------------------------------
#include <winsock2.h>
#ifndef WSOCKETH
#define WSOCKETH
//--------------------------------------------------------------------------- 
#include <string>
#define      WS_RCV_BUF_LEN                 1024
#define      WS_PROCESS_NULL_CHAR           1

/*    Winsock API retuen Value Definition    */
#define      WS_SUCCESS                     0L


/*    Winsock API Version Definition    */
#define      WS_VER_1_0                     0x0001      // Version 1.0
#define      WS_VER_1_1                     0x0101      // Version 1.1
#define      WS_VER_2_0                     0x0002      // Version 2.0


/* runtime message define */
#define      WS_MSG_SUCCESS                 "Success"
#define      WS_MSG_ERROR                   "Winsock Error"
#define      WS_MSG_VER_NOT_SUPPORT         "Sockets version is not supported by Windows Sockets."
#define      WS_MSG_WS_ALREADY_START        "Sockets is already startup !"


/* Winsock WS_ERRor code Message Definition */
#define      WS_ERR_MSG_UNKNOW              "Unknow error !"
#define      WS_ERR_MSG_10004               "IntWS_ERRupted system call."
#define      WS_ERR_MSG_10009               "Bad file number."
#define      WS_ERR_MSG_10013               "Permission Denied."
#define      WS_ERR_MSG_10014               "Bad Address."
#define      WS_ERR_MSG_10022               "Invalid Argument."
#define      WS_ERR_MSG_10024               "Too many open files."
#define      WS_ERR_MSG_10035               "Operation would block."
#define      WS_ERR_MSG_10036               "Operation now in progress."
#define      WS_ERR_MSG_10037               "Operation already in progress."
#define      WS_ERR_MSG_10038               "Socket operation on nonsocket."
#define      WS_ERR_MSG_10039               "Destination address required."
#define      WS_ERR_MSG_10040               "Message too long."
#define      WS_ERR_MSG_10041               "Protocol wrong type for socket."
#define      WS_ERR_MSG_10042               "Protocol not available."
#define      WS_ERR_MSG_10043               "Protocol not supported."
#define      WS_ERR_MSG_10044               "Socket type not supported."
#define      WS_ERR_MSG_10045               "Operation not supported on socket."
#define      WS_ERR_MSG_10046               "Protocol family not supported."
#define      WS_ERR_MSG_10047               "Address family not supported by protocol family."
#define      WS_ERR_MSG_10048               "Address already in use."
#define      WS_ERR_MSG_10049               "Can not assign requested address."
#define      WS_ERR_MSG_10050               "Network is down."
#define      WS_ERR_MSG_10051               "Network is unreachable."
#define      WS_ERR_MSG_10052               "Network dropped connection."
#define      WS_ERR_MSG_10053               "Software caused connection abort."
#define      WS_ERR_MSG_10054               "Connection reset by peer."
#define      WS_ERR_MSG_10055               "No buffer space available."
#define      WS_ERR_MSG_10056               "Socket is already connected."
#define      WS_ERR_MSG_10057               "Socket is not connected."
#define      WS_ERR_MSG_10058               "Can not send after socket shutdown."
#define      WS_ERR_MSG_10059               "Too many references: can not splice."
#define      WS_ERR_MSG_10060               "Connection timed out."
#define      WS_ERR_MSG_10061               "Connection refused."
#define      WS_ERR_MSG_10062               "Too many levels of symbolic links."
#define      WS_ERR_MSG_10063               "File name too long."
#define      WS_ERR_MSG_10064               "Host is down."
#define      WS_ERR_MSG_10065               "No route to host."
#define      WS_ERR_MSG_10066               "Directory not empty."
#define      WS_ERR_MSG_10067               "Too many processes."
#define      WS_ERR_MSG_10068               "Too many users."
#define      WS_ERR_MSG_10069               "Disk quota exceeded."
#define      WS_ERR_MSG_10070               "Stale NFS file handle."
#define      WS_ERR_MSG_10071               "Too many levels of remote in path."
#define      WS_ERR_MSG_10091               "Network subsystem is unusable."
#define      WS_ERR_MSG_10092               "Winsock DLL cannot support this application."
#define      WS_ERR_MSG_10093               "Winsock not initialized."
#define      WS_ERR_MSG_10101               "Disconnect."
#define      WS_ERR_MSG_11001               "Host not found."
#define      WS_ERR_MSG_11002               "Nonauthoritative host not found."
#define      WS_ERR_MSG_11003               "Nonrecoverable WS_ERRor."
#define      WS_ERR_MSG_11004               "Valid name, no data record of requested type."
//---------------------------------------------------------------------------
typedef struct sockaddr_in   WS_SOCKADDR;
//---------------------------------------------------------------------------
class WSOCKET
{
public:
	//WSOCKET(void);
	//~WSOCKET(void);
	WSADATA   WS_Data;                 /*  winsock API variable declare */
	bool      winsock_startup_flag;    /*  winsock API flag variable declare */
	char      ws_recv_buf[WS_RCV_BUF_LEN];

	/*  winsock function Definition  */
	void WSOCKET_Initialize(void);

	bool WSOCKET_Startup(WORD version);
	bool WSOCKET_Cleanup(void);

	void WSOCKET_Send_String_hex(SOCKET aSocket, char *chastr , int length);
	void WSOCKET_Send_String(SOCKET aSocket, std::string str);
	bool WSOCKET_Receive_String(SOCKET aSocket, std::string *rcv_que);

	bool WSOCKET_Set_Send_TimeOut(SOCKET aSocket, int iSendout);
	bool WSOCKET_Set_Receive_TimeOut(SOCKET aSocket, int iRcvout);

	std::string WSOCKET_Get_Port(SOCKET aSocket);

	std::string WSOCKET_Get_Host_Name(SOCKET aSocket);
	std::string WSOCKET_Get_Local_IP(void);

private:
	void WSOCKET_POP_Error_Msg(int error_code)const;
	std::string WSOCKET_Get_Peer_IP(SOCKET aSocket);

};
#endif


