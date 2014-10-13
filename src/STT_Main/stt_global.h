#ifndef STT_GLOBAL_H
#define STT_GLOBAL_H

#include "3rd/lua_tinker/lua_tinker.h"
#include "stt_interface/Basic_Module_Interface.h"
/* ############################################################################################################# */
///
/// \brief G_STT_Interpreter
///
extern std::map<int,lua_State *> G_STT_Interpreter;

///
/// \brief G_Test_Info
///
extern std::map<int,TEST_INFO> G_Test_Info;

///
/// \brief G_Test_Fun_Info
///
extern std::map<std::string,TEST_FUN_INFO>  G_Test_Fun_Info;

/* ############################################################################################################# */

void G_STT_Reg_Fun(int test_id,const char * moudle_name,const char * fun_name ,const char * fun_describe,FUN f );

bool G_STT_Run_Fun(const char * fun_name, int test_id,const  char * v1 ="",const  char * v2="",const  char * v3="",const  char * v4="", const  char * v5="",const  char * v6="",const char * v7="",const  char * v8="",const  char * v9="");

/* ############################################################################################################# */

bool stt_print (int test_id,const  char * arg1,const char * arg2,const  char * arg3,const  char * arg4, const  char * arg5,const  char * arg6,const char * arg7,const  char * arg8,const  char * arg9);


/* ############################################################################################################# */


class STT_Global
{
public:
    STT_Global();

   static void Reg_Fun(int test_id);
};

#endif // STT_GLOBAL_H
