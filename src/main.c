#include "base/base.h"
#include "platform/platform.h"
#include "renderer/api/vulkan/lhvk.h"


#include "base/base.c"
#include "renderer/api/vulkan/lhvk.c"

#include <wayland-client.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <string.h>

//~ This will be moved to some wayland thingy


typedef struct WaylandState {
    struct wl_compositor *compositor;
} WaylandState;



internal void
registry_handle_global(void *data, struct wl_registry *registry,
                       uint32_t name, const char *interface, uint32_t version)
{
	DEBUG("interface: '%s', version: %d, name: %d",
          interface, version, name);
    if (data) 
    {
        struct WaylandState *state = (WaylandState*)data;
        if (strcmp(interface, wl_compositor_interface.name) == 0)
        {
            INFOMSG("Asigned compositor version 4");
            state->compositor = wl_registry_bind(registry, name, &wl_compositor_interface, 4);
        }
    }
}

internal void
registry_handle_global_remove(void *data, struct wl_registry *registry,
                              uint32_t name)
{
	// This space deliberately left blank
}

static const struct wl_registry_listener
registry_listener = {
	.global = registry_handle_global,
	.global_remove = registry_handle_global_remove,
};


//~ MAIN
int main(int argc, char *argv[])
{
    GLOBAL_BASE_ALLOCATOR = mm_create_malloc_base_memory();
#ifdef _WIN32
    putenv("VK_INSTANCE_LAYERS=VK_LAYER_KHRONOS_validation;VK_LAYER_KHRONOS_profiles");
#else
    putenv("VK_INSTANCE_LAYERS=VK_LAYER_KHRONOS_validation:VK_LAYER_KHRONOS_profiles");
#endif
    
    PlatformWindow window = {0};
    window.width = 800;
    window.height = 600;
    
    Arena renderer_arena = mm_make_arena_reserve(GLOBAL_BASE_ALLOCATOR, KB(256));
    
    struct wl_display *display = wl_display_connect(0);
    s32 wl_fd = wl_display_get_fd(display);
    INFOMSG("display connected with fd: %d", wl_fd);
    WaylandState state = {0};
    
    struct wl_registry *registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, &state);
    wl_display_roundtrip(display);
    
    
    
    
    /* 
        while(wl_display_dispatch(display) != -1) 
        {
            // This is blank because yes;
        }
     */
    
    wl_display_disconnect(display);
    INFOMSG("display disconnected");
    
    
    return 0;
}