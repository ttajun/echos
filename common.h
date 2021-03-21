#ifndef COMMON_H
#define COMMON_H

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "error_functions.h"

typedef enum { FALSE, TRUE } Boolean;

#define min(m,n) ((m) < (n) ? (m) : (n))
#define max(m,n) ((m) > (n) ? (m) : (n))

/* for debug */
#define ANSI_COLOR_RED "\x1b[31m" 
#define ANSI_COLOR_GREEN "\x1b[32m" 
#define ANSI_COLOR_YELLOW "\x1b[33m" 
#define ANSI_COLOR_BLUE "\x1b[34m" 
#define ANSI_COLOR_MAGENTA "\x1b[35m" 
#define ANSI_COLOR_CYAN "\x1b[36m" 
#define ANSI_COLOR_RESET "\x1b[0m" 

#define DEBUG
#ifdef DEBUG
#define t_debug(fmt, args...) fprintf(stderr, ANSI_COLOR_YELLOW"[%s:%d:%s()]: "ANSI_COLOR_RESET fmt, \
__FILE__, __LINE__, __func__, ##args)
#define t_info(fmt, args...) fprintf(stderr, ANSI_COLOR_CYAN"[%s:%d:%s()]: "ANSI_COLOR_RESET fmt, \
__FILE__, __LINE__, __func__, ##args)
#else
#define t_debug(fmt, args...)
#define t_info(fmt, args...)
#endif

#endif
