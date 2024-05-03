use crate::vulkan::color::*;
use cgmath::{vec2, vec3, vec4};
use std::mem::size_of;
use vulkanalia::vk::{self, HasBuilder};
pub type Vec2 = cgmath::Vector2<f32>;
pub type Vec3 = cgmath::Vector3<f32>;
pub type Vec4 = cgmath::Vector4<f32>;

pub static VERTICES: [Vertex; 4] = [
    Vertex::new(vec2(-0.5, -0.5), 0x00FF00FF),
    Vertex::new(vec2(0.5, -0.5), 0xFF0000FF),
    Vertex::new(vec2(0.5, 0.5), 0x0000FFFF),
    Vertex::new(vec2(-0.5, 0.5), 0xFFFFFFFF),
];

pub const INDICES: &[u16] = &[0, 1, 2, 2, 3, 0];

#[repr(C)]
#[derive(Clone, Copy, Debug)]
pub struct Vertex {
    pos: Vec2,
    color: u32,
}

impl Vertex {
    pub const fn new(pos: Vec2, color: u32) -> Self {
        Self { pos, color }
    }

    pub fn binding_description() -> vk::VertexInputBindingDescription {
        vk::VertexInputBindingDescription::builder()
            .binding(0)
            .stride(size_of::<Vertex>() as u32)
            .input_rate(vk::VertexInputRate::VERTEX)
            .build()
    }

    pub fn attribute_description() -> [vk::VertexInputAttributeDescription; 2] {
        let pos = vk::VertexInputAttributeDescription::builder()
            .binding(0)
            .location(0)
            .format(vk::Format::R32G32_SFLOAT)
            .offset(0)
            .build();

        let color = vk::VertexInputAttributeDescription::builder()
            .binding(0)
            .location(1)
            .format(vk::Format::R32G32B32A32_UINT)
            .offset(size_of::<Vec2>() as u32)
            .build();
        [pos, color]
    }
}
