#pragma once

#include "../include/glad/glad.h"

#include "Shader.hpp"

//things Framebuffer has in common with the rest:
//glGenFramebuffers(1, &FBO);
//glBindFramebuffer(...)

//glDeleteFramebuffers(1, &FBO);
//glDeleteTextures(1, &Tex);

//obs:
//no reason to remember width & height

class Framebuffer
{
    protected:
    unsigned int FBO;
    unsigned int Tex;
    unsigned int width, height;

    Framebuffer(unsigned int width, unsigned int height);
    
    public:
    unsigned int GetTexture(); //idk how to use textures (and what I need from them, so I'll work on this)
    void Bind();
    void Unbind();
    unsigned int GetWidth();
    unsigned int GetHeight();
    virtual void Resize(unsigned int newWidth, unsigned int newHeight) = 0; // may not need to be PURE
    virtual ~Framebuffer() = 0;
};

class ScreenFramebuffer : public Framebuffer
{
    private:
    unsigned int RBO;
    unsigned int VAO, VBO, EBO;
    void InitVAO();
    Shader* shader;
    void InitShader();

    public:
    ScreenFramebuffer(unsigned int width, unsigned int height);

    void Draw();
    void Resize(unsigned int newWidth, unsigned int newHeight) override;
    ~ScreenFramebuffer() override;
};