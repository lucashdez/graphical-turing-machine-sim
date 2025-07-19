int main(int argc, char** argv) {
    GLOBAL_BASE_ALLOCATOR = mm_create_malloc_base_memory();
    printf("a");
    
    Display *d;
    Window w;
	String8 message = string_u8_litexpr("Hello World");
	int s;
	d = XOpenDisplay(0);
	if (!d) {
        fprintf(stderr, "Cannot open display");
        exit(1);
	}
	s = DefaultScreen(d);
	w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, 100, 100, 1, BlackPixel(d, s), WhitePixel(d,s));
	XSelectInput(d, w, ExposureMask | KeyPressMask);
	XMapWindow(d, w);
    
    XEvent e;
	while(1) {
        XNextEvent(d, &e);
        if (e.type == Expose) {
            XFillRectangle(d, w, DefaultGC(d, s), 20, 20, 10, 10);
            XDrawString(d, w, DefaultGC(d, s), 10, 50, (char*)message.str, message.size);
        }
        
        if (e.type == KeyPress) {
            break;
        }
        
	}
    
	XCloseDisplay(d);
	return 0;
}
