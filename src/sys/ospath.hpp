#ifndef __OS_PATH__
# define __OS_PATH__

# include <string>

# ifdef _WIN32
	#include <windows.h>
	#include <shlobj.h>
	#define OS_PATH_S '\\'
	#define OS_PATH_L MAX_PATH
# else
	#ifdef __APPLE__
		#include <mach-o/dyld.h>
	#endif
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <limits.h>
	#include <unistd.h>
	#include <pwd.h>
	#define OS_PATH_S '/'
	#define OS_PATH_L 1024
# endif

typedef std::string str_t;

#endif //__OS_PATH__

static inline str_t SysRealExecPath(const char *argv0) {

	char out_dir[ OS_PATH_L ];
	ssize_t cnt = OS_PATH_L;

#ifdef _WIN32
	cnt = GetModuleFileName(NULL, out_dir, OS_PATH_L);
#elif defined __APPLE__
	_NSGetExecutablePath(out_dir, &s);
#elif defined __FreeBSD__
	int mib[4] = {CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, -1};
	sysctl(mib, 4, out_dir, &s, NULL, 0);
#elif defined __linux__
	cnt = readlink("/proc/self/exe", out_dir, OS_PATH_L);
#elif defined __NetBSD__
	cnt = readlink("/proc/curproc/exe", out_dir, OS_PATH_L);
#else // other unix
	str_t lproc = str_t("/proc/") + std::to_string(getpid()) + str_t("/exe");
	cnt = readlink(lproc.c_str(), out_dir, OS_PATH_L);
#endif

	do {
		cnt--;
	} while (out_dir[cnt] != OS_PATH_S);

	return str_t(out_dir, cnt+1);
}

static inline str_t SysAcessConfigPath(const char *appname) {

	str_t cfg_dir;

#ifdef _WIN32
	TCHAR wcfg[MAX_PATH];

	if (S_OK == SHGetFolderPath(
		NULL, CSIDL_FLAG_CREATE | CSIDL_LOCAL_APPDATA,
		NULL, 0, wcfg))) {

		PathAppend(wcfg, TEXT(appname));

		if (SHCreateDirectory(NULL, wcfg) == ERROR_SUCCESS)
			/* do nothing */;
		cfg_dir += wcfg,
		cfg_dir += "\\";
	}
#else
	struct passwd *pw = getpwuid(getuid());

	if (pw) {
		cfg_dir += pw->pw_dir,
		cfg_dir += "/.config/",
		cfg_dir += appname,
		cfg_dir += "/";

		if (mkdir(cfg_dir.c_str(), 0777) == 0)
			/* sucess */;
	}
#endif
	return cfg_dir;
}
