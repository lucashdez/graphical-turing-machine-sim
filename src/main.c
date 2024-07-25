//####################
//  PLATFORM WIN32
#ifdef _WIN32
#  include "include/win32.h"
#endif
//  PLATFORM LINUX 
#ifdef __linux__
#  include "linux.h"
#endif

// HEADERS
#include "include/base/base_include.h"
#include <stdlib.h>
#include <stdio.h>

//####################
//    Graphics
#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include "vulkan/vulkan.h"

#include "include/lh_vulkan.h"


// DEFINITIONS


//####################
//    Graphics
#include "include/lh_vulkan.c"

//####################
//  PLATFORM WIN32
#ifdef _WIN32
#  include "win32.c"
#endif

//  PLATFORM LINUX
#ifdef __linux__
#  include "linux.c"
#endif
