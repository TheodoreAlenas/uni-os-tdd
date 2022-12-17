#ifndef DEV_MODE_H
#define DEV_MODE_H

/* for README, dev-mode */
#ifdef DEV

#include <stdio.h>
#include <unistd.h>

#define PRINT_DEV_STATUS() printf("\x1b[%dm[pid %d %s > %s]\x1b[0m ", \
    getpid() % 6 + 32, getpid(), __FILE__, __FUNCTION__)
#define WELL(str) PRINT_DEV_STATUS(); printf("%s\n", str)
#define WELLL(printf_expr) PRINT_DEV_STATUS(); printf_expr; printf("\n"); fflush(stdout)

#else

#define PRINT_DEV_STATUS()
#define WELL(str)
#define WELLL(printf_args)

#endif
/* end of snippet */


#endif
