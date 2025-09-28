#include "base/base.h"
#include "platform/linux/wayland/wayland.h"
#include "platform/platform.h"

#include "base/base.c"
#include "platform/linux/wayland/wayland.c"

#include "renderer/api/software/software_renderer.c"
/*  THIS GOES TO WAYLAND */
#include "platform/linux/wayland/xdg-shell-protocol.c"
#include "platform/linux/wayland/xdg-shell-protocol.h"
#include "renderer/renderer.h"
#include <stdint.h>
#include <wayland-client-protocol.h>
#include <wayland-client.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

/* ~ This will be moved to some wayland thingy */

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
randname(char *buf)
{
    struct timespec ts = {0};
    clock_gettime(CLOCK_REALTIME, &ts);
    long r = ts.tv_nsec;
    int i;
    for (i = 0; i < 6; ++i)
    {
        buf[i] = 'A' + (r & 15) + (r & 16) * 2;
        r >>= 5;
    }
}

internal int
create_shm_file(void)
{
    int retries = 100;
    do
    {
        char name[] =
            "/wl_shm-XXXXXX"; /* This is some ANSI C thingy lel wayland <3  */
        randname(name + sizeof(name) - 7);
        --retries;
        int fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0600);
        if (fd >= 0)
        {
            shm_unlink(name);
            return fd;
        }
    } while (retries > 0 && errno == EEXIST);
    return -1;
}

int
allocate_shm_file(size_t size)
{
    int fd = create_shm_file();
    if (fd < 0)
        return -1;
    int ret;
    do
    {
        ret = ftruncate(fd, size);
    } while (ret < 0 && errno == EINTR);
    if (ret < 0)
    {
        close(fd);
        return -1;
    }
    return fd;
}

internal void
wl_buffer_release(void *data, struct wl_buffer *buffer)
{
    /*  sent by the compositor */
    ShmBuffer *b = data;
    if (b)
    {
        b->busy = 0;
    }
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

    void *mem = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (mem == MAP_FAILED)
    {
        close(fd);
        return -1;
    }
    struct wl_shm_pool *pool = wl_shm_create_pool(S->shm, fd, size);
    struct wl_buffer *buffer = wl_shm_pool_create_buffer(
        pool, 0, w, h, stride, WL_SHM_FORMAT_XRGB8888);
    wl_shm_pool_destroy(pool);
    close(fd);

    B->buffer = buffer;
    B->data = mem;
    B->size = size;
    B->stride = stride;
    B->busy = false;

    wl_buffer_add_listener(buffer, &buffer_listener, B);
    return 0;
}

internal void
destroy_shm_buffer(ShmBuffer *B)
{
    if (!B->buffer)
        return;
    wl_buffer_destroy(B->buffer);
    munmap(B->data, B->size);
    *B = (ShmBuffer){0};
}

internal void
wl_frame_done(void *data, struct wl_callback *cb, u32 time);

internal void
xdg_surface_configure(void *data, struct xdg_surface *xsurface, u32 serial)
{
    struct PlatformWindow *w = data;
    struct WaylandState *state = w->os_window;
    xdg_surface_ack_configure(xsurface, serial);

    if (!state->configured)
    {
        state->configured = true;

        b32 found = lwl_get_free_buffer(state);
        if (found)
        {
            ShmBuffer *buf = &state->buffers[state->cur];
            wl_surface_attach(state->surface, buf->buffer, 0, 0);
        }

        wl_surface_commit(state->surface);
    }

    if (w->width > 0 && w->height > 0)
    {
        s32 i;
        for (i = 0; i < 3; ++i)
        {
            if (state->buffers[i].data)
            {
                munmap(state->buffers[i].data, state->buffers[i].size);
                wl_buffer_destroy(state->buffers[i].buffer);
            }
            create_shm_buffer(state, &state->buffers[i], w->width, w->height);
        }
        state->cur = 0;
    }
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

void
pointer_handle_enter(void *data, struct wl_pointer *pointer, uint32_t serial,
                     struct wl_surface *surface, wl_fixed_t sx, wl_fixed_t sy)
{
    /*  El puntero ha entrado en una superficie */
}

void
pointer_handle_leave(void *data, struct wl_pointer *pointer, uint32_t serial,
                     struct wl_surface *surface)
{
    /*  El puntero ha salido de la superficie */
}

void
pointer_handle_motion(void *data, struct wl_pointer *pointer, uint32_t time,
                      wl_fixed_t sx, wl_fixed_t sy)
{
    WaylandState *state = data;
    state->pointer_x = wl_fixed_to_int(sx);
    state->pointer_y = wl_fixed_to_int(sy);
}

void
pointer_handle_button(void *data, struct wl_pointer *pointer, uint32_t serial,
                      uint32_t time, uint32_t button, uint32_t state)
{
    /*  state = WL_POINTER_BUTTON_STATE_PRESSED o RELEASED */
}

void
pointer_handle_axis(void *data, struct wl_pointer *pointer, uint32_t time,
                    uint32_t axis, wl_fixed_t value)
{
    /*  Scroll */
}

/* Here we should handle all the pointer events */
void
pointer_handle_frame(void *data, struct wl_pointer *pointer)
{
    WaylandState *state = data;
    INFO("Pointer (%d, %d)", state->pointer_x, state->pointer_y);
    /*  TODO (lucashdez) */
}

global_var struct wl_pointer_listener pointer_listener = {
    .enter = pointer_handle_enter,
    .leave = pointer_handle_leave,
    .motion = pointer_handle_motion,
    .button = pointer_handle_button,
    .axis = pointer_handle_axis,
    .frame = pointer_handle_frame,
};

internal void
registry_handle_global(void *data, struct wl_registry *registry, uint32_t name,
                       const char *interface, uint32_t version)
{
    DEBUG("interface: '%s', version: %d, name: %d", interface, version, name);
    if (data)
    {
        struct WaylandState *state = (WaylandState *)data;

        if (strcmp(interface, wl_compositor_interface.name) == 0)
        {
            INFOMSG("Asigned compositor version 4");
            state->compositor =
                wl_registry_bind(registry, name, &wl_compositor_interface, 4);
        }
        if (strcmp(interface, wl_shm_interface.name) == 0)
        {
            INFOMSG("Asigned shm interface version 1");
            state->shm = wl_registry_bind(registry, name, &wl_shm_interface, 1);
        }
        if (strcmp(interface, xdg_wm_base_interface.name) == 0)
        {
            state->x_wm_base =
                wl_registry_bind(registry, name, &xdg_wm_base_interface, 1);
            xdg_wm_base_add_listener(state->x_wm_base, &x_wm_base_listener,
                                     state);
        }
        if (strcmp(interface, wl_seat_interface.name) == 0)
        {
            state->seat =
                wl_registry_bind(registry, name, &wl_seat_interface, version);
            state->pointer = wl_seat_get_pointer(state->seat);
            wl_pointer_add_listener(state->pointer, &pointer_listener, state);
        };
    }
}

internal void
registry_handle_global_remove(void *data, struct wl_registry *registry,
                              uint32_t name)
{
    /*  This space deliberately left blank */
}

static const struct wl_registry_listener registry_listener = {
    .global = registry_handle_global,
    .global_remove = registry_handle_global_remove,
};

internal void
toplevel_handle_close_action(void *data, struct xdg_toplevel *toplevel)
{
    running = false;
}

internal void
toplevel_handle_configure(void *data, struct xdg_toplevel *toplevel, s32 width,
                          s32 height, struct wl_array *states)
{
    PlatformWindow *wnd = data;
    WaylandState *state = wnd->os_window;
    INFO("GOT TOPLEVEL CONFIGURE, %d %d", width, height);
    wnd->width = width;
    wnd->height = height;
    xdg_surface_set_window_geometry(state->x_surface, 0, 0, wnd->width,
                                    wnd->height);
}

static u32 color = 0xFF000000;

internal void
wl_frame_done(void *data, struct wl_callback *cb, u32 time);

global_var const struct wl_callback_listener wl_surface_frame_listener = {
    .done = wl_frame_done,
};

global_var const struct xdg_toplevel_listener toplevel_listener = {
    .configure = toplevel_handle_configure,
    .close = toplevel_handle_close_action,
};

static s32 xOffset = 0;
void
app_step(PlatformWindow *w, void *user_ptr)
{
    PlatformFrame *frame = &w->frame_info;
    renderer_begin_section(w);

    Rects32 r1 = {.p = {.x = 0, .y = 0}, .width = VIRTUAL_WIDTH, .height = 50};
    Rects32 r2 = {.p = {.x = 0, .y = 50},
                  .width = VIRTUAL_WIDTH,
                  .height = VIRTUAL_HEIGHT - 50};

    Rects32 r3 = {
        .p = {.x = 10 + xOffset, .y = VIRTUAL_HEIGHT / 2},
        .width = 50,
        .height = 50,
    };

    Rects32 r4 = {
        .p = {.x = 80 + xOffset * 12, .y = VIRTUAL_HEIGHT / 2},
        .width = 50,
        .height = 50,
    };

    renderer_draw_rect(w, r1, 0xFFaa0000, true);
    renderer_draw_rect(w, r2, 0xFF0000aa, true);
    renderer_draw_rect(w, r3, 0xFF00FF00, true);
    renderer_draw_rect(w, r4, 0xFF00FFFF, true);

    renderer_end_section(w);
    xOffset = (xOffset + 1) % 500;
    /*  Present this in the upper corner */
    INFO("dt=%llu ms (%.2f fps)", frame->dt,
         1000.0f / (frame->dt ? frame->dt : 1));
}

internal void
wl_frame_done(void *data, struct wl_callback *cb, u32 time)
{
    wl_callback_destroy(cb);
    PlatformWindow *wnd = data;
    WaylandState *w = wnd->os_window;

    /* wl_display_dispatch_pending(w->display); */

    if (w->frame_cb)
    {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        u32 now_ms = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
        u32 dt = now_ms - w->last_frame;
        wnd->frame_info.dt = dt;
        w->last_frame = now_ms;
        w->frame_cb(wnd, w->frame_user);
    }

    ShmBuffer *buf = &w->buffers[w->cur];
    wl_surface_attach(w->surface, buf->buffer, 0, 0);
    wl_surface_damage_buffer(w->surface, 0, 0, wnd->width, wnd->height);
    wl_surface_commit(w->surface);

    struct wl_callback *next_cb = wl_surface_frame(w->surface);
    wl_callback_add_listener(next_cb, &wl_surface_frame_listener, wnd);
}

/* ~ MAIN */
int
main(int argc, char *argv[])
{
    GLOBAL_BASE_ALLOCATOR = mm_create_malloc_base_memory();
#ifdef _WIN32
    putenv("VK_INSTANCE_LAYERS=VK_LAYER_KHRONOS_validation;VK_LAYER_KHRONOS_"
           "profiles");
#else
    putenv("VK_INSTANCE_LAYERS=VK_LAYER_KHRONOS_validation:VK_LAYER_KHRONOS_"
           "profiles");
#endif

    PlatformWindow window = {0};
    window.width = 1920;
    window.height = 1080;

    Arena renderer_arena =
        mm_make_arena_reserve(GLOBAL_BASE_ALLOCATOR, MB(256));

    struct wl_display *display = wl_display_connect(0);
    s32 wl_fd = wl_display_get_fd(display);

    WaylandState state = {0};
    state.configured = 0;
    state.last_frame = 0;
    state.display = display;
    state.framebuffer = MMPushArrayZeros(&renderer_arena, u32, 1920 * 1080);
    window.os_window = &state;

    struct wl_registry *registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, &state);
    wl_display_roundtrip(state.display);
    state.surface = wl_compositor_create_surface(state.compositor);
    state.x_surface =
        xdg_wm_base_get_xdg_surface(state.x_wm_base, state.surface);
    state.x_toplevel = xdg_surface_get_toplevel(state.x_surface);
    xdg_surface_add_listener(state.x_surface, &x_surface_listener, &window);
    xdg_toplevel_set_title(state.x_toplevel, "example");
    xdg_toplevel_add_listener(state.x_toplevel, &toplevel_listener, &window);
    xdg_surface_set_window_geometry(state.x_surface, 0, 0, window.width,
                                    window.height);
    s32 result = create_shm_buffer(&state, &state.buffers[0], 1920, 1080);
    result = create_shm_buffer(&state, &state.buffers[1], 1920, 1080);
    result = create_shm_buffer(&state, &state.buffers[2], 1920, 1080);
    state.cur = 0;

    struct wl_callback *first = wl_surface_frame(state.surface);
    static const struct wl_callback_listener listener = {.done = wl_frame_done};
    wl_callback_add_listener(first, &listener, &window);

    wl_surface_commit(state.surface);

    /* SET frame callback */
    state.frame_cb = app_step;
    state.frame_user = &state;
    /* - */

    while (running == 1)
    {
        wl_display_dispatch(display);
    }

    wl_display_disconnect(state.display);

    return 0;
}
