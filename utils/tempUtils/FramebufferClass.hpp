#pragma once
#include "../include/glad/glad.h"

#include "Shader.hpp"

#include <iostream>

#define FB_TYPE_NORMAL GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT
#define FB_TYPE_DEPTH GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT

class Framebuffer
{
    private:
    enum Type {SCREEN, DEPTH} type;
    struct FB_TYPE
    {
        int component, attachment;
        FB_TYPE(int c, int a):component(c),attachment(a){}
    } fb_type;
    unsigned int FBO, RBO, TexColorBuffer;
    unsigned int width, height; //do I really need these?
    unsigned int canvasVAO, canvasVBO, canvasEBO;
    Shader* canvasShader;
    void InitVAO();
    void InitShader();

    public:
    Framebuffer(unsigned int width, unsigned int height,
                int FB_TYPE_C = GL_DEPTH24_STENCIL8, int FB_TYPE_A = GL_DEPTH_STENCIL_ATTACHMENT);
    void Resize(unsigned int newWidth, unsigned int newHeight);
    unsigned int GetWidth();
    unsigned int GetHeight();
    void BindTexture();
    void Bind();
    void Unbind();
    void Draw();
    void Delete();
};

//I need to somehow make it a 'borrowable' texture