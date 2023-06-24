
#include <iostream>
#include <string>

typedef std::string str_t;

#define TH_Err(msg) \
  std::cerr << msg << "\n"
#define TH_Except(LIT, cstr) \
  std::runtime_error(str_t(LIT" ;; ") + str_t(cstr)) 
#define TH_Throw(msg) \
  std::__throw_runtime_error(msg)

#ifdef DEBUG
# define TH_Log(msg) std::cout << msg
#else
# define TH_Log(msg) (void)msg
#endif
