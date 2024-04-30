use crate::vulkan::vertex::Vec4;
use cgmath::vec4;

#[repr(C)]
pub struct Color {}

pub fn rgba(rgba: u32) -> Vec4 {
    let r = rgba & 0xff000000;
    let g = rgba & 0x00ff0000;
    let b = rgba & 0x0000ff00;
    let a = rgba & 0x000000ff;
    vec4::<f32>(r as f32, g as f32, b as f32, a as f32)
}
