use vulkan::app::App;
use winit::{
    event::{Event, WindowEvent},
    event_loop::{ControlFlow, EventLoop},
    window::WindowBuilder,
};

pub mod vulkan;

fn main() {
    let event_loop = EventLoop::new().unwrap();
    let window = WindowBuilder::new().build(&event_loop).unwrap();

    // ControlFlow::Poll continuously runs the event loop, even if the OS hasn't
    // dispatched any events. This is ideal for games and similar applications.
    event_loop.set_control_flow(ControlFlow::Poll);

    // ControlFlow::Wait pauses the event loop if no events are available to process.
    // This is ideal for non-game applications that only update in response to user
    // input, and uses significantly less power/CPU time than ControlFlow::Poll.
    event_loop.set_control_flow(ControlFlow::Wait);

    let mut app: App = unsafe { App::create(&window).unwrap() };
    event_loop
        .run(move |event, elwt| {
            match event {
                Event::WindowEvent {
                    window_id,
                    event: WindowEvent::CloseRequested,
                } => {
                    println!("The close button was pressed in: {:?}; stopping", window_id);
                    elwt.exit();
                    app.destroy();
                }
                Event::AboutToWait => {
                    // Application update code.

                    // Queue a RedrawRequested event.
                    //
                    // You only need to call this if you've determined that you need to redraw in
                    // applications which do not always need to. Applications that redraw continuously
                    // can render here instead.
                    window.request_redraw();
                }
                Event::WindowEvent {
                    window_id,
                    event: WindowEvent::RedrawRequested,
                } => {
                    if !elwt.exiting() {
                        // TODO: Handle unwrap
                        app.render(&window).unwrap()
                    }
                }
                _ => (),
            }
        })
        .expect("couldn't run the event loop.");
}
