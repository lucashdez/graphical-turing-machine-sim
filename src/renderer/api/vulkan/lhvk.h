/* date = July 21st 2025 9:31 am */
#ifndef LHVK_H
#define LHVK_H

#include <stdio.h>

/* 
*  ############
*    STRUCTS
  *  ############
*/
typedef struct Lhvk_VulkanState 
{
    Arena arena;
    b32 enable_validation_layers;
    
    VkInstance instance;
    VkDebugUtilsMessengerEXT debug_messenger;
    VkSurfaceKHR surface;
    
    
} Lhvk_VulkanState;

/* 
*  ############
 *      API
  *  ############
*/
internal Lhvk_VulkanState lhvk_initVulkan(PlatformWindow*);
internal void lhvk_createInstance(Lhvk_VulkanState*);


#endif //LHVK_H
