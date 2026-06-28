#pragma once

#include "../include/glad/glad.h"

#include "Shader.hpp"
#include "Texture.hpp"

class FramebufferCompleteExcepton{};

class Framebuffer
{
    protected:
    unsigned int FBO;
    //Texture2D Tex;
    void CheckComplete();
    
    public:
    unsigned int width, height;
    Framebuffer();
    virtual void Bind();
    void Unbind(unsigned int windowWidth, unsigned int windowHeight); //parameters = new viewport size
    void Unbind(); //leaves the viewport on framebuffer's size
    
    virtual void Resize(unsigned int newWidth, unsigned int newHeight);
    virtual ~Framebuffer();
};

class ScreenFramebuffer : public Framebuffer
{
    private:
    unsigned int RBO;
    unsigned int VAO, VBO, EBO;
    void InitVAO();
    Shader* shader; //to print data (on screen)
    void InitShader();

    public:
    ScreenFramebuffer();
    ScreenFramebuffer(unsigned int width, unsigned int height);

    void Bind() override; //handles viewport
    //void DrawOnVAO
    void DrawToScreen(unsigned int screenWidth, unsigned int screenHeight);
    void Resize(unsigned int newWidth, unsigned int newHeight) override;
    ~ScreenFramebuffer() override;
};

class DepthFramebuffer : public Framebuffer
{
    private:
    unsigned int VAO, VBO, EBO;
    void InitVAO(); //debugging
    Shader* shader;
    void InitShaders();
    
    public:
    Shader* renderShader; //how data is written on the texture
    DepthFramebuffer();
    DepthFramebuffer(unsigned int width, unsigned int height);

    void Bind() override; //handles viewport + depth clearing

    void DrawToScreen(unsigned int screenWidth, unsigned int screenHeight); //debugging 
    void Resize(unsigned int newWidth, unsigned int newHeight) override;
    ~DepthFramebuffer() override;
};