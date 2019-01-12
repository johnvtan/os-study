#ifndef __PANIC_H__
#define __PANIC_H__

#include "stdio.h"

#define PANIC(msg) {printf(msg); while (1) {}}
#endif
