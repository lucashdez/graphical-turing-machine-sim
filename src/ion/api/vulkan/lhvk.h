/* date = July 21st 2025 9:31 am */
#ifndef LHVK_H
#define LHVK_H

#include <stdio.h>
// This include is because of strcmp
#include <string.h>

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
internal void lhvk_init_vulkan(Lhvk_VulkanState*);
internal void lhvk_create_instance(Lhvk_VulkanState*);

#endif //LHVK_H
