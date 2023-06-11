#ifndef _DBGK_H_
#define _DBGK_H_
#include <printk.h>
#define DEBUGK // 可注释
#ifdef DEBUGK
#define DEBUGK(fmt, ...) do { printk("debugk::" fmt, ##__VA_ARGS__); } while (0)
#else
#define DEBUGK(...)
#endif
#endif // !_DBGK_H_


