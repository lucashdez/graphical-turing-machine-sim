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


global_var b32 running = 1;
global_var const struct wl_callback_listener wl_surface_frame_listener;
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


typedef struct ShmBuffer {
    struct wl_buffer *buffer;
    void *data;
    s32 size;
    s32 stride;
    b32 busy;
} ShmBuffer;

typedef struct WaylandState {
    struct wl_display *display;
    struct wl_compositor *compositor;
    struct wl_shm *shm;
    struct xdg_wm_base *x_wm_base;
    
    // Something
    struct wl_surface *surface;
    struct xdg_surface *x_surface;
    struct xdg_toplevel *x_toplevel;
    
    u32 *framebuffer;
    PlatformFrameCallback frame_cb;
    void *frame_user;


    // buffer thingy
    struct wl_buffer *buffer;
    void* data_buffer;

    ShmBuffer buffers[2];
    s32 cur;
    
    b32 running;
    
} WaylandState;
internal ShmBuffer* get_free_buffer(WaylandState *state);
internal void 
wl_buffer_release(void* data, struct wl_buffer *buffer) 
{
    // sent by the compositor
    ShmBuffer *b = data;
    if (b)
        b->busy = 0;
}

global_var const struct wl_buffer_listener buffer_listener = {
    .release = wl_buffer_release,
};



internal s32 
create_shm_buffer(WaylandState *S, ShmBuffer *B, s32 w, s32 h) 
{
    s32 stride = w * 4;
    s32 size = stride * h;
    
    int fd = allocate_shm_file(size);
    if (fd < 0)
    {
        return -1; 
    }
    
    void* mem = mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    
    if (mem == MAP_FAILED)
    {
        close(fd);
        return -1;
    }
    struct wl_shm_pool *pool = wl_shm_create_pool(S->shm, fd, size);
    struct wl_buffer *buffer = wl_shm_pool_create_buffer(pool, 0, w, h, stride, WL_SHM_FORMAT_XRGB8888);
    wl_shm_pool_destroy(pool);
    close(fd);
    
    B->buffer = buffer;
    B->data = mem;
    B->size = size;
    B->stride = stride;
    B->busy = 0;
    
    wl_buffer_add_listener(buffer, &buffer_listener, B);
    return 0;
}

internal void
destroy_shm_buffer(ShmBuffer *B)
{
    if (!B->buffer) return;
    wl_buffer_destroy(B->buffer);
    munmap(B->data, B->size);
    *B = (ShmBuffer){0};
}

internal struct wl_buffer*
create_buffer(struct WaylandState *state)
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
    
    
    mm_memcpy(data, state->framebuffer, size);
    
    state->data_buffer = data;
    //munmap(data, size);
    wl_buffer_add_listener(buffer, &buffer_listener, 0);
    return buffer;
}


internal void wl_frame_done(void *data, struct wl_callback *cb, u32 time);

internal void
xdg_surface_configure(void *data, struct xdg_surface * xsurface, u32 serial)
{
    struct WaylandState *state = data;
    xdg_surface_ack_configure(xsurface, serial);
    
    ShmBuffer *buf = get_free_buffer(state);
    struct wl_callback *first = wl_surface_frame(state->surface);
    static const struct wl_callback_listener listener = {.done = wl_frame_done};
    wl_callback_add_listener(first, &listener, state);
    if (buf) 
    {
        wl_surface_attach(state->surface, buf->buffer, 0, 0);
    }
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

internal void
toplevel_handle_close_action(void *data, struct xdg_toplevel *toplevel)
{
    running = false;
}

internal void
toplevel_handle_configure(void *data
                          , struct xdg_toplevel *toplevel
                          , s32 width, s32 height
                          , struct wl_array *states) 
{
    
}


static u32 color = 0xFF000000;

internal void wl_frame_done(void *data, struct wl_callback *cb, u32 time);

global_var const struct wl_callback_listener wl_surface_frame_listener = {
    .done = wl_frame_done,
};

global_var const struct xdg_toplevel_listener toplevel_listener = {
    .configure = toplevel_handle_configure,
    .close = toplevel_handle_close_action,
};



void app_step(PlatformFrame *frame, void *user_ptr) {
    WaylandState *state = user_ptr; 
    ShmBuffer *B = &state->buffers[state->cur];
    u8* Row = (u8*)state->framebuffer;

    local_persist int XOffset = 0;
    local_persist int YOffset = 0;

    for (int Y = 0; Y < 1080; ++Y)
    {
        u8* pixel = Row;
        for (int X = 0; X < 1920; ++X) 
        {
            *pixel = (u8)(X + XOffset);
            ++pixel;
            *pixel = (u8)(Y + YOffset);
            ++pixel;
            *pixel = 0;
            ++pixel;
            *pixel = 0;
            ++pixel;


        }
        Row += (1920 * 4);
    }

    INFO("frame %llu",frame->dt);

    XOffset += 1;
    YOffset += 1;
}

internal
ShmBuffer* get_free_buffer(WaylandState *state) {
    for(int i = 0; i < 2; i++)
    {
        if (!state->buffers[i].busy) {
            state->cur = i;
            return &state->buffers[i];
        }
    }
    return 0;
}

internal void wl_frame_done(void *data, struct wl_callback *cb, u32 time) 
{
    wl_callback_destroy(cb);
    WaylandState *w = data;

    ShmBuffer *buf = get_free_buffer(w);
    if (!buf){ 
        wl_display_dispatch_pending(w->display);
        buf = get_free_buffer(w);
        if(!buf) return;
    }
    
    //wl_display_dispatch_pending(w->display);

    if (w->frame_cb)
    {
        w->frame_cb(&(PlatformFrame){.dt = time, .width = 1920, .height = 1080}, data);
        mm_memcpy(buf->data, w->framebuffer, buf->size);
    }

    buf->busy = 1;

    wl_surface_attach(w->surface, buf->buffer, 0, 0);
    wl_surface_damage_buffer(w->surface, 0, 0, 1920, 1080);


    struct wl_callback *next_cb = wl_surface_frame(w->surface);
    wl_callback_add_listener(next_cb, &wl_surface_frame_listener, w);

    wl_surface_commit(w->surface);
}






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
    
    Arena renderer_arena = mm_make_arena_reserve(GLOBAL_BASE_ALLOCATOR, MB(256));
    
    
    struct wl_display *display = wl_display_connect(0);
    s32 wl_fd = wl_display_get_fd(display);

    INFO("display connected with fd: %d", wl_fd);
    WaylandState state = {0};
    state.display = display;
    state.framebuffer = MMPushArrayZeros(&renderer_arena, u32, 1920*1080);

    struct wl_registry *registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, &state);
    wl_display_roundtrip(state.display);
    
    state.surface = wl_compositor_create_surface(state.compositor);
    state.x_surface = xdg_wm_base_get_xdg_surface(state.x_wm_base, state.surface);
    xdg_surface_add_listener(state.x_surface, &x_surface_listener, &state);
    state.x_toplevel = xdg_surface_get_toplevel(state.x_surface);
    xdg_toplevel_set_title(state.x_toplevel, "example");
    xdg_toplevel_add_listener(state.x_toplevel, &toplevel_listener, &state);
    create_shm_buffer(&state, &state.buffers[0], 1920, 1080);
    create_shm_buffer(&state, &state.buffers[1], 1920, 1080);
    state.cur = 0;
    wl_surface_commit(state.surface);

    //SET frame callback
    state.frame_cb = app_step;
    state.frame_user = &state;
    /////
    
    
    struct wl_callback *cb = wl_surface_frame(state.surface);
    wl_callback_add_listener(cb, &wl_surface_frame_listener, &state);
    while(running == 1) 
    {
        wl_display_dispatch(display);
    }
    
    
    wl_display_disconnect(state.display);
    INFOMSG("display disconnected");
    
    
    return 0;
}
