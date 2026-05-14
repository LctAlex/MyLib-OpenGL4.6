// #include "Framebuffer.hpp"

// Framebuffer::Framebuffer(unsigned int w, unsigned int h):
// width(w),height(h),canvasMesh(Mesh::createCanvas()),
// canvasShader("shaders/framebuffer/screen/screenVert.glsl", "shaders/framebuffer/screen/screenFrag.glsl"),
// tex(w, h)
// {
//     glGenFramebuffers(1, &FBO);
//     glBindFramebuffer(GL_FRAMEBUFFER, FBO);
// }

// bool Framebuffer::IsComplete()
// {
//     if(glCheckFramebufferStatus(FBO) == GL_FRAMEBUFFER_COMPLETE) return true;
//     else return false;
// }

