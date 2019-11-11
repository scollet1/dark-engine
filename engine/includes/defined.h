#ifndef DEFINED_H
#define DEFINED_H

# if __STDC_VERSION__ < 199901L
	# if __GNUC__ >= 2
		# define __func__ __FUNCTION__
	# else
		# define __func__ "<unknown>"
	# endif
# endif

#ifndef WHICH
#define WHICH(var) (#var)
#endif

/*
** FATAL ERRORS
*/

# ifndef FATAL_ERR
# define FATAL_ERR std::string("FATAL ERROR")
# endif

#ifndef ERR_SYS
#define ERR_SYS -1
#endif

#ifndef ERR_PROC
#define ERR_PROC -2
#endif

/*
** NON-FATAL ERRORS
*/

# ifndef NON_FATAL_ERR
# define NON_FATAL_ERR std::string("NON-FATAL ERROR")
# endif

#ifndef ERR_ELSE
#define ERR_ELSE 1
#endif

# ifndef ERR_INVALID
# define ERR_INVALID 2
# endif

# ifndef ERR_EXIT
# define ERR_EXIT 3
# endif

#ifndef SUCCESS_I
#define SUCCESS_I 0
#endif

#ifndef FAILURE_I
#define FAILURE_I -1
#endif

#ifndef SUCCESS
#define SUCCESS true
#endif

#ifndef FAILURE
#define FAILURE false
#endif

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#ifndef TRUE_STR
#define TRUE_STR (ANSI_COLOR_GREEN)
#endif

#ifndef FALSE_STR
#define FALSE_STR (ANSI_COLOR_RED)
#endif

#endif // DEFINED_H