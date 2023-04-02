
#include <iostream>
#include <string>

typedef std::string str_t;

#define _eLog(eee) \
  std::cerr << eee << "\n"
#define _eRt(LIT, cstr) \
  std::runtime_error(str_t(LIT" ;; ") + str_t(cstr))
#define _eThrow(LIT,TIL) \
  std::__throw_runtime_error(LIT " ;; " TIL)

#ifdef DEBUG
# define _sLog(sss) std::cout << sss
#else
# define _sLog(sss) (void)sss
#endif
