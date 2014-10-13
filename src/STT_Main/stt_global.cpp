#include "stt_global.h"
/* ############################################################################################################# */
// 来自 Basic_Module_Interface.h 文件全局数据的定义
/************************************/

///
/// \brief G_Test_Info
///
std::map<int,TEST_INFO> G_Test_Info;

///
/// \brief G_Test_Fun_Info
///
std::map<std::string,TEST_FUN_INFO>  G_Test_Fun_Info;



/* ############################################################################################################# */


///
/// \brief G_STT_Interpreter [STT解释器]
///
std::map<int,lua_State *> G_STT_Interpreter;


/* ############################################################################################################# */

void G_STT_Reg_Fun(int test_id,const char * moudle_name,const char * fun_name ,const char * fun_describe,FUN f )
{

    if(G_Test_Fun_Info.find(fun_name)==G_Test_Fun_Info.end())
    {
       TEST_FUN_INFO tfi={moudle_name,fun_name,fun_describe};
       G_Test_Fun_Info.insert(std::pair<std::string,TEST_FUN_INFO>(fun_name,tfi));
    }

    lua_tinker::def(G_STT_Interpreter[test_id], fun_name, f);
}

bool G_STT_Run_Fun(const char * fun_name, int test_id,const  char * v1,const  char * v2,const  char * v3,const  char * v4, const  char * v5,const  char * v6,const char * v7,const  char * v8,const  char * v9)
{
   return  lua_tinker::call<bool,int,const char *,const char *,const char *,const char *,const char *,const char *,const char *,const char *,const char *>(G_STT_Interpreter[test_id],fun_name,test_id,v1,v2,v3,v4,v5,v6,v7,v8,v9);
}


/* ############################################################################################################# */
bool stt_print(int test_id, const char *arg1, const char *arg2, const char *arg3, const char *arg4, const char *arg5, const char *arg6, const char *arg7, const char *arg8, const char *arg9)
{
     std::cout<<arg1<<arg2<<arg3<<arg4<<arg5<<arg6<<arg7<<arg8<<arg9<<std::endl;

     return true;
}

/* ############################################################################################################# */


STT_Global::STT_Global()
{
}

void STT_Global::Reg_Fun(int test_id)
{
         G_STT_Reg_Fun(test_id,"STT_MAIN","stt_print","",stt_print);

}



