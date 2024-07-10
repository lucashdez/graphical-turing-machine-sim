// HEADERS
#include "base/base_include.h"
#include <stdlib.h>
#include <stdio.h>


//####################
//    Graphics
#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR 1
#endif
#include "vulkan/vulkan.h"

#include "lh_vulkan.h"

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
//    Graphics
#include "lh_vulkan.c"

//####################
//  PLATFORM WIN32
#ifdef _WIN32
#  include "win32.c"
#endif

//  PLATFORM LINUX
#ifdef __linux__
#  include "linux.c"
#endif
