#include "base/base.h"
#include "platform/platform.h"
#include "renderer/api/vulkan/lhvk.h"


#include "base/base.c"
#include "renderer/api/vulkan/lhvk.c"

// THIS GOES TO WAYLAND
#include <wayland-client.h>
#include "platform/linux/wayland/xdg-shell-protocol.h"
#include "platform/linux/wayland/xdg-shell-protocol.c"


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <string.h>

//~ This will be moved to some wayland thingy

#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

internal void 
randname(char* buf)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    long r = ts.tv_nsec;
    for(int i = 0; i < 6; ++i)
    {
        buf[i] = 'A' + (r&15) + (r&16)*2;
        r >>= 5;
    }
}

internal int
create_shm_file(void)
{
    int retries = 100;
    do {
        char name[] = "/wl_shm-XXXXXX"; // This is some ANSI C thingy lel wayland <3 
        randname(name + sizeof(name) - 7);
        --retries;
        int fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0600);
        if (fd >= 0) {
            shm_unlink(name);
            return fd;
        }
    } while(retries > 0 && errno == EEXIST);
    return -1;
}

int 
allocate_shm_file(size_t size)
{
    int fd = create_shm_file();
    if (fd < 0)
        return -1;
    int ret;
    do {
        ret = ftruncate(fd, size);
    } while(ret < 0 && errno == EINTR);
    if (ret < 0)
    {
        close(fd);
        return -1;
    }
    return fd;
}


typedef struct WaylandState {
    struct wl_compositor *compositor;
    struct wl_shm *shm;
    struct xdg_wm_base *x_wm_base;
    
    // Something
    struct wl_surface *surface;
    struct xdg_surface *x_surface;
    struct xdg_toplevel *x_toplevel;
} WaylandState;

internal void 
wl_buffer_release(void* data, struct wl_buffer *buffer) 
{
    // sent by the compositor
    wl_buffer_destroy(buffer);
}

global_var const struct wl_buffer_listener buffer_listener = {
    .release = wl_buffer_release,
};

internal struct wl_buffer*
draw_frame(struct WaylandState *state)
{
    const int width = 640, height = 480;
    int stride = width * 4;
    int size = stride * height;
    
    int fd = allocate_shm_file(size);
    if (fd == -1)
    {
        return 0; 
    }
    u32 *data = mmap(NULL, size, PROT_READ| PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED)
    {
        close(fd);
        return 0;
    }
    struct wl_shm_pool *pool = wl_shm_create_pool(state->shm, fd, size);
    struct wl_buffer *buffer = wl_shm_pool_create_buffer(pool, 0, width, height, stride, WL_SHM_FORMAT_XRGB8888);
    wl_shm_pool_destroy(pool);
    close(fd);
    
    /* Draw checkerboxed background */
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if ((x + y / 8 * 8) % 16 < 8)
                data[y * width + x] = 0xFF666666;
            else
                data[y * width + x] = 0xFFEEEEEE;
        }
    }
    munmap(data, size);
    wl_buffer_add_listener(buffer, &buffer_listener, 0);
    return buffer;
}

internal void
xdg_surface_configure(void *data, struct xdg_surface * xsurface, u32 serial)
{
    struct WaylandState *state = data;
    xdg_surface_ack_configure(xsurface, serial);
    
    struct wl_buffer *buffer = draw_frame(state);
    wl_surface_attach(state->surface, buffer, 0, 0);
    wl_surface_commit(state->surface);
}

global_var const struct xdg_surface_listener x_surface_listener = {
    .configure = xdg_surface_configure,
};

internal void
xdg_wm_base_ping(void *data, struct xdg_wm_base *x_wm_base, u32 serial)
{
    xdg_wm_base_pong(x_wm_base, serial);
}

global_var const struct xdg_wm_base_listener x_wm_base_listener = {
    .ping = xdg_wm_base_ping,
};

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
        if (strcmp(interface, wl_shm_interface.name) == 0) 
        {
            INFOMSG("Asigned shm interface version 1");
            state->shm = wl_registry_bind(registry, name, &wl_shm_interface, 1);
        }
        if (strcmp(interface, xdg_wm_base_interface.name) == 0)
        {
            state->x_wm_base = wl_registry_bind(registry, name, &xdg_wm_base_interface, 1);
            xdg_wm_base_add_listener(state->x_wm_base, &x_wm_base_listener, state);
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
    
    state.surface = wl_compositor_create_surface(state.compositor);
    state.x_surface = xdg_wm_base_get_xdg_surface(state.x_wm_base, state.surface);
    xdg_surface_add_listener(state.x_surface, &x_surface_listener, &state);
    state.x_toplevel = xdg_surface_get_toplevel(state.x_surface);
    xdg_toplevel_set_title(state.x_toplevel, "example");
    wl_surface_commit(state.surface);
    
    
    while(wl_display_dispatch(display) != -1) 
    {
        // This is blank because yes;
    }
    
    wl_display_disconnect(display);
    INFOMSG("display disconnected");
    
    
    return 0;
}