use crate::vulkan::raw::VulkanErrors;
use winit::window::Window;

#[derive(Clone, Debug)]
pub struct App {}

impl App {
    pub fn create(window: &Window) -> Result<Self, VulkanErrors> {
        Ok(Self {})
    }

    pub fn render(&mut self, window: &Window) -> Result<(), VulkanErrors> {
        Ok(())
    }

    pub fn destroy(&mut self) {}
}

#[derive(Clone, Debug, Default)]
struct AppData {}
