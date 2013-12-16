#ifndef __dbg_h_
#define __dbg_h_

#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifdef NDEBUG
#define debug(M, ...)
#else
// debug("format", arg1, arg2)
#define debug(M, ...) fprintf(stderr, "[Debug]\t%s:%d:\t" M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
// __FILE__ , __LINE__ 将告诉你file:line
// ##__VA_ARGS__ 是说将 ...中的所有内容都放在这里
#endif

// 定义一个可以安全读 errno 的版本
#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define log_err(M, ...) fprintf(stderr, "[Error]\t(%s:%d: errno: %s)\t" M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_warn(M, ...) fprintf(stderr, "[Warn]\t(%s:%d: errno: %s)\t" M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_info(M, ...) fprintf(stderr, "[Info]\t(%s:%d)\t" M "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

// sentinel 放在函数中任何它不应该执行到的地方，你可以把它放在 if 语句或者 switch 语句
// 中不应该发生的分支，像 default
#define sentinel(M, ...)  { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

// check_mem 用以检查指针是合法的
#define check_mem(A) check((A), "Out of memory.")

// 在 NDEBUG定义了的情况下，依然检查，只是不打印信息
#define check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__); errno=0; goto error; }

#endif
