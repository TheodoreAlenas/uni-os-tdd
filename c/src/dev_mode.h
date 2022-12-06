#ifndef DEV_MODE_H
#define DEV_MODE_H

#ifdef DEV

#include <stdio.h>
char *WHO;
#define GREEN   "\x1b[32m"
#define RESET   "\x1b[0m"

#define PRINT_DEV_STATUS() printf("%s[%s]%s ", GREEN, WHO, RESET)
#define WELL(str) PRINT_DEV_STATUS(); printf("%s\n", str)
#define WELLL(printf_expr) PRINT_DEV_STATUS(); printf_expr; printf("\n");
#define I_AM(who) WHO = who

#else

#define PRINT_DEV_STATUS()
#define WELL(str)
#define WELLL(printf_args)
#define I_AM(who)

#endif


#endif
