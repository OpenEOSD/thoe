#ifndef __OS_ERR__
# define __OS_ERR__
# include <iostream>

# define Sys_Err(msg) std::cerr << msg << std::endl
# define Sys_Log(msg) std::cout << msg

# define Sys_ExitFail() exit( EXIT_FAILURE )
# define Sys_ExitOK()   exit( EXIT_SUCCESS )

# ifndef DEBUG
	#undef  Sys_Log
	#define Sys_Log(msg) (void)msg
# endif
#endif //__OS_ERR__