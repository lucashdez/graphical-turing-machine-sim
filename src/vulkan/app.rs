use anyhow::{anyhow, Result};
use vulkanalia::{
    loader::{LibloadingLoader, LIBRARY},
    prelude::v1_0::*,
    window as vk_window,
};
use winit::window::Window;

/// This function creates an instance of Vulkan
/// * `window` - The winit window
/// * `entry` - The vulkan entry
unsafe fn create_instance(window: &Window, entry: &Entry) -> Result<Instance> {
    let application_info = vk::ApplicationInfo::builder()
        .application_name(b"Graphical Turing\0")
        .application_version(vk::make_version(0, 0, 1))
        .engine_name(b"No engine")
        .engine_version(vk::make_version(0, 0, 1))
        .api_version(vk::make_version(1, 0, 0));

    // Probably get optional extensions for mac
    let enabled_extension_names = vk_window::get_required_instance_extensions(&window)
        .iter()
        .map(|e| e.as_ptr())
        .collect::<Vec<_>>();

    let info = vk::InstanceCreateInfo::builder()
        .application_info(&application_info)
        .enabled_extension_names(&enabled_extension_names);
    Ok(entry.create_instance(&info, None)?)
}

#[derive(Clone, Debug)]
pub struct App {
    entry: Entry,
    instance: Instance,
}

impl App {
    pub unsafe fn create(window: &Window) -> Result<Self> {
        let loader = LibloadingLoader::new(LIBRARY)?;
        let entry = Entry::new(loader).map_err(|b| anyhow!("{}", b))?;
        let instance = create_instance(window, &entry)?;
        Ok(Self { entry, instance })
    }

    pub fn render(&mut self, window: &Window) -> Result<()> {
        Ok(())
    }

    pub fn destroy(&mut self) {
        // SAFETY: Cleaning the instance
        unsafe {
            self.instance.destroy_instance(None);
        }
    }
}

#[derive(Clone, Debug, Default)]
struct AppData {}
