#pragma once
#include "../include/glad/glad.h"

#include "Shader.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"

//is it scree/depthframebuffer or screen/depthbuffer?
class ScreenframebufferException{};
class DepthframebufferException{};

class Framebuffer
{
    protected:
    unsigned int FBO;
    Mesh canvasMesh;
    Shader canvasShader;
    virtual bool IsComplete();
    
    public:
    Framebuffer(unsigned int w, unsigned int h);
    unsigned int width, height;
    Texture2D tex;
    virtual void Bind();
    void Unbind(unsigned int newViewportW, unsigned int newViewportH);
    void Unbind();
    virtual ~Framebuffer() = 0;
};