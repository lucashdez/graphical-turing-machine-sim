#ifndef WIN32_H
# define WIN32_H

# if !defined(_WIN32) && (defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__))
#  define _WIN32
# endif /* _WIN32 */

#include "windows.h"

#endif