#ifndef CMU_H
#define CMU_H
#include "Communication/Communication.h"
#include "stt_basic_moudle.h"
#include <iostream>
#include <map>
using namespace std;


struct CMU_INFO
{
    ///
    /// \brief CMU_Table
    ///  name
    ///
    std::map<std::string, CCommunication*> name_commu;
    ///
    /// \brief anonymous_commu
    ///
    CCommunication* anonymous_commu;
};

///
/// \brief CMU_Table
///
static std::map<int,CMU_INFO> CMU_Table;



#define D_CONNECT ("\
#########################################################\r\n\
# 函数说明:\r\n\
#   打开并连接通讯 目前支持 telnet、console、系统命令行  \r\n\
#   \r\n\
# 参数说明:\r\n\
#   arg1: 连接模式[telnet;console,cmd] ----------必选参数\r\n\
#   arg2: 是否使用配置登陆,0默认登陆,1不登陆 [0;1]---可选\r\n\
#   arg3: 匿名通讯会自动关闭，给当前通讯命名[任意名字]   \r\n\
#   arg4: 无                                             \r\n\
#   arg5: 无                                             \r\n\
#   arg6: 无                                             \r\n\
#  示例:                                                 \r\n\
#    connect = telnet , 0 , cpe_telnet                   \r\n\
#                                                        \r\n\
#########################################################")
bool connect (int test_id,const  char * arg1,const char * arg2,const  char * arg3,const  char * arg4, const  char * arg5,const  char * arg6,const char * arg7,const  char * arg8,const  char * arg9);


#define D_CLOSE ("\
#########################################################\r\n\
# 函数说明:\r\n\
#   关闭通讯 目前支持 telnet、console、系统命令行  \r\n\
#   \r\n\
# 参数说明:\r\n\
#   arg1: 连接模式[telnet;console,cmd] ----------必选参数\r\n\
#   arg2: 匿名通讯会自动关闭，关闭通讯命名[任意名字] ---可选\r\n\
#  示例:                                                 \r\n\
#    connect = telnet ,  cpe_telnet                   \r\n\
#                                                        \r\n\
#########################################################")
bool close (int test_id,const  char * arg1,const char * arg2,const  char * arg3,const  char * arg4, const  char * arg5,const  char * arg6,const char * arg7,const  char * arg8,const  char * arg9);


#define D_SEND ("\
#########################################################\r\n\
# 函数说明:\r\n\
#   发送命令到通讯 目前支持 telnet、console、系统命令行  \r\n\
#   \r\n\
# 参数说明:\r\n\
#   arg1: 发送的命令 ----------必选参数\r\n\
#   arg2: 发送目标通讯，无为匿名通讯 通讯命名[任意名字] ---可选\r\n\
#  示例:                                                 \r\n\
#    send = dir ,  cpe_telnet                   \r\n\
#                                                        \r\n\
#########################################################")
bool send (int test_id,const  char * arg1,const char * arg2,const  char * arg3,const  char * arg4, const  char * arg5,const  char * arg6,const char * arg7,const  char * arg8,const  char * arg9);


#define D_RECEIVE ("\
#########################################################\r\n\
# 函数说明:\r\n\
#   发送命令到通讯 目前支持 telnet、console、系统命令行  \r\n\
#   \r\n\
# 参数说明:\r\n\
#   arg1: 延时时间 ----------必选参数\r\n\
#   arg2: 为正则表达式 ----------必选参数\r\n\
#   arg3: 需要查找的值在正则表达式的第几组内 ----------必选参数\r\n\
#   arg4: 获取的结果保存到变量,默认RECEIVE ----------可选参数\r\n\
#   arg5: 发送目标通讯，无为匿名通讯 通讯命名[任意名字] ---可选\r\n\
#  示例:                                                 \r\n\
#    receive = 2000,Mac.Address.{2}(.*).,1,para                  \r\n\
#                                                        \r\n\
#########################################################")
bool receive (int test_id,const  char * arg1,const char * arg2,const  char * arg3,const  char * arg4, const  char * arg5,const  char * arg6,const char * arg7,const  char * arg8,const  char * arg9);



#define D_PING ("\
#########################################################\r\n\
# 函数说明:\r\n\
#   ping  \r\n\
#   \r\n\
# 参数说明:\r\n\
#   arg1: 目标IP ----------必选参数\r\n\
#   arg2: 源IP  [使用默认路由设置] ----------可选参数\r\n\
#   arg3: PING包的次数 默认4 ----------可选参数\r\n\
#   arg4: PING通的连续成功次数,默认1 ----------可选参数\r\n\
#   arg5: PING包的大小 默认64 ---可选\r\n\
#  示例:                                                 \r\n\
#   ping = %ip%,,60,3,64                  \r\n\
#                                                        \r\n\
#########################################################")
bool ping (int test_id,const  char * arg1,const char * arg2,const  char * arg3,const  char * arg4, const  char * arg5,const  char * arg6,const char * arg7,const  char * arg8,const  char * arg9);

#define D_SLEEP ("\
#########################################################\r\n\
# 函数说明:\r\n\
#   示设置延时的时间  \r\n\
#   \r\n\
# 参数说明:\r\n\
#   arg1: 延时时间，时间单位为毫秒 ----------必选参数\r\n\
#  示例:                                                 \r\n\
#   sleep = 2000                 \r\n\
#                                                        \r\n\
#########################################################")
bool sleep (int test_id,const  char * arg1,const char * arg2,const  char * arg3,const  char * arg4, const  char * arg5,const  char * arg6,const char * arg7,const  char * arg8,const  char * arg9);

#define D_FIND ("\
#########################################################\r\n\
# 函数说明:\r\n\
#   查找通讯返回信息相应的值  \r\n\
#   \r\n\
# 参数说明:\r\n\
#   arg1: 延时时间，时间单位为毫秒 ----------必选参数\r\n\
#   arg2: 要查找得值 ----------必选参数\r\n\
#   arg3: 发送目标通讯，无为匿名通讯 通讯命名[任意名字] ---可选\r\n\
#  示例:                                                 \r\n\
#   find = 3000,succee                 \r\n\
#                                                        \r\n\
#########################################################")
bool find (int test_id,const  char * arg1,const char * arg2,const  char * arg3,const  char * arg4, const  char * arg5,const  char * arg6,const char * arg7,const  char * arg8,const  char * arg9);



#define D_LOOPCHK ("\
#########################################################\r\n\
# 函数说明:\r\n\
#   在一定的时间内下一串指令然后通过正则表达式查找是否有返回所需要的值  \r\n\
#   \r\n\
# 参数说明:\r\n\
#   arg1: 发送的指令 ----------必选参数\r\n\
#   arg2: 总时间        ----------必选参数\r\n\
#   arg3: 正则表达式 ----------必选参数\r\n\
#   arg4: 需要查找的值在正则表达式的第几组内 ----------必选参数\r\n\
#   arg5: 发送完指令后等待时间 默认0----------可选参数\r\n\
#   arg6: 获取的结果保存到变量,默认RECEIVE ----------可选参数\r\n\
#   arg7: 目标通讯，无为匿名通讯 通讯命名[任意名字] ---可选\r\n\
#  示例:                                                 \r\n\
#    loopchk = adsl info,5000,Path.*\D\d.*\D(\d*)\DKbps.*Downstream,1,1                  \r\n\
#                                                        \r\n\
#########################################################")
bool loopchk (int test_id,const  char * arg1,const char * arg2,const  char * arg3,const  char * arg4, const  char * arg5,const  char * arg6,const char * arg7,const  char * arg8,const  char * arg9);

#define D_CHECK ("\
#########################################################\r\n\
# 函数说明:\r\n\
#   如果需要比较相同，则使用一个参数，如果需要比较范围，则需要使用两个参数，用来表示上下限  \r\n\
#   \r\n\
# 参数说明:\r\n\
#   arg1: 比较值1   为数值择是上线                    ----------必选参数\r\n\
#   arg2: 比较值2  不为空则是数值范围比较下线 ----------可选参数\r\n\
#   arg3: 目标变量,默认RECEIVE                       ----------可选参数\r\n\
#  示例:                                                 \r\n\
#    check = %up_rate%,%down_rate%                   \r\n\
#                                                        \r\n\
#########################################################")
bool check (int test_id,const  char * arg1,const char * arg2,const  char * arg3,const  char * arg4, const  char * arg5,const  char * arg6,const char * arg7,const  char * arg8,const  char * arg9);


#define D_SET ("\
#########################################################\r\n\
# 函数说明:\r\n\
#   用来将receive指令通过正则表达匹配到的值保存一个变量里  \r\n\
#   如果有四个参数:set = 新变量名,变量名,分隔符类型(数字),分隔数量  \r\n\
#   分隔符类型有:0->无,1->:,2->-,3->.,4->%3A,5->空格,6->空格  \r\n\
#   \r\n\
# 参数说明:\r\n\
#   arg1: 目标变量名                                                 ----------必选参数\r\n\
#   arg2: 源数据                                                        ----------可选参数\r\n\
#   arg3: 分隔符类型 0->无,1->:,2->-,3->.,4->%3A,5->空格,6->空格(除了空格，数据会变成十六进制)----------可选参数\r\n\
#   arg4: 分隔数量                                                     ----------可选参数\r\n\
#  示例:                                                 \r\n\
#    set = %newmac%,%mac%,1,2                 \r\n\
#                                                        \r\n\
#########################################################")
bool set (int test_id,const  char * arg1,const char * arg2,const  char * arg3,const  char * arg4, const  char * arg5,const  char * arg6,const char * arg7,const  char * arg8,const  char * arg9);

#define D_STRINGADD ("\
#########################################################\r\n\
# 函数说明:\r\n\
#   根据具体截取数值确定累积参数增加  \r\n\
#   \r\n\
# 参数说明:\r\n\
#   arg1: 目的截取后的数值字符串                             ----------必选参数\r\n\
#   arg2: stringAdd指令所用到参数的个数（包含本身) ----------必选参数\r\n\
#   arg3: 前缀字符串                                                 ----------必选参数\r\n\
#   arg4: 括号内的第一个参数表示被截取的字符串，第二个参数表示起始位，第三个参数表示位数长度 ----------必选参数\r\n\
#   arg..:   同[arg4] \r\n\
#  示例:                                                 \r\n\
#    stringAdd = %StrNew%,4,Str(WLAN_),Sub(&mac&;9;4)                 \r\n\
#                                                        \r\n\
#########################################################")
bool stringAdd (int test_id,const  char * arg1,const char * arg2,const  char * arg3,const  char * arg4, const  char * arg5,const  char * arg6,const char * arg7,const  char * arg8,const  char * arg9);


#define D_COMPARE ("\
#########################################################\r\n\
# 函数说明:\r\n\
#   表示用于目的字符串的值与源字符串的值进行对比  \r\n\
#   \r\n\
# 参数说明:\r\n\
#   arg1: 目的字符串的值                             ----------必选参数\r\n\
#   arg2: 是源字符串的值                             ----------必选参数\r\n\
#   arg3: 第一个参数截取子字符串的索引号  ----------可选参数\r\n\
#   arg4: 截取子字符串的长度                      ----------可选参数\r\n\
#  示例:                                                 \r\n\
#    compare = %StrNew%,%ssid%,2,3                 \r\n\
#                                                        \r\n\
#########################################################")
bool compare (int test_id,const  char * arg1,const char * arg2,const  char * arg3,const  char * arg4, const  char * arg5,const  char * arg6,const char * arg7,const  char * arg8,const  char * arg9);


#define D_ADD ("\
#########################################################\r\n\
# 函数说明:\r\n\
#   将源字符串的内容加/减一个数,然后按指定格式保存到目的字符串的变 \r\n\
#   量里  \r\n\
#   \r\n\
# 参数说明:\r\n\
#   arg1: 目的字符串的值                                  ----------必选参数\r\n\
#   arg2: 是源字符串的值                                  ----------必选参数\r\n\
#   arg3: 被加/减数[整数]:表示要减或加的参数   ----------必选参数\r\n\
#   arg4: 目的字符串格式[整数]:参数表示指定的间隔符(0:无,1: \":\",2: \"-\",3: \".\",4: \"%3A\",5:\" \")----------必选参数\r\n\
#   arg5: 目的串截取段[整数]:间隔符之间的字符数   ----------必选参数\r\n\
#   arg6: 加/减[0/1]:1为递增，0为递减。                  ----------必选参数\r\n\
#  示例:                                                 \r\n\
#    add = %mac1%,%mac%,8,0,2,1                 \r\n\
#                                                        \r\n\
#########################################################")
bool add (int test_id,const  char * arg1,const char * arg2,const  char * arg3,const  char * arg4, const  char * arg5,const  char * arg6,const char * arg7,const  char * arg8,const  char * arg9);


#define D_LOOPFIND ("\
#########################################################\r\n\
# 函数说明:\r\n\
#   在一定的时间内下一串指令然后通过正则表达式查找是否有返回所需要的值  \r\n\
#   \r\n\
# 参数说明:\r\n\
#   arg1: 发送的指令 ----------必选参数\r\n\
#   arg2: 总时间        ----------必选参数\r\n\
#   arg3: 单步循环时间 ----------必选参数\r\n\
#   arg4: 需要查找的值 ----------必选参数\r\n\
#   arg5: 目标通讯，无为匿名通讯 通讯命名[任意名字] ---可选\r\n\
#  示例:                                                 \r\n\
#    loopfind = NULL, 40000,10000,rstBtn_pressed                  \r\n\
#                                                        \r\n\
#########################################################")
bool loopfind (int test_id,const  char * arg1,const char * arg2,const  char * arg3,const  char * arg4, const  char * arg5,const  char * arg6,const char * arg7,const  char * arg8,const  char * arg9);



#define D_CHANGE ("\
#########################################################\r\n\
# 函数说明:\r\n\
#  从[源字符串名称]获取的数据,用[正则式]匹配出来的子字符串,根据[改变的方式]对子字符串进行格式上的转换并保存到[目标字符名称]  \r\n\
#   \r\n\
# 参数说明:\r\n\
#   arg1: 目标字符串名 ----------必选参数\r\n\
#   arg2: 源字符名称        ----------必选参数\r\n\
#   arg3: 正则式 ----------必选参数\r\n\
#   arg4: 表达式所在组 ----------必选参数\r\n\
#   arg5: 改变的方式 0:无改变;1:变为小写,2,变为大写---必选参数\r\n\
#  示例:                                                 \r\n\
#    loopfind = NULL, 40000,10000,rstBtn_pressed                  \r\n\
#                                                        \r\n\
#########################################################")
bool change (int test_id,const  char * arg1,const char * arg2,const  char * arg3,const  char * arg4, const  char * arg5,const  char * arg6,const char * arg7,const  char * arg8,const  char * arg9);

#endif // CMU_H
