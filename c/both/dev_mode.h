#ifndef DEV_MODE_H
#define DEV_MODE_H

#ifdef DEV

#include <stdio.h>
#include <unistd.h>

#define PRINT_PREFIX() printf("\x1b[%dm[pid %d %s > %s]\x1b[0m ", \
    getpid() % 6 + 32, getpid(), __FILE__, __FUNCTION__)
/* for README, dev-mode */
#define WELL(str) PRINT_PREFIX(); printf("%s\n", str)
#define WELLL(printf_expr) PRINT_PREFIX(); \
  printf_expr; printf("\n"); fflush(stdout)
/* end of snippet */

#else

#define PRINT_PREFIX()
#define WELL(str)
#define WELLL(printf_args)

#endif


#endif
