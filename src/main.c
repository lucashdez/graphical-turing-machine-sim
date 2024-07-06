// HEADERS
#include "base/base_include.h"
#include <stdlib.h>
#include <stdio.h>

//####################
//  PLATFORM WIN32
#ifdef _WIN32
#  include "win32.h"
#endif
//  PLATFORM LINUX 
#ifdef __linux__
#  include "linux.h"
#endif

// DEFINITIONS


//####################
//  PLATFORM WIN32
#ifdef _WIN32
#  include "win32.c"
#endif

//  PLATFORM LINUX
#ifdef __linux__
#  include "linux.c"
#endif
