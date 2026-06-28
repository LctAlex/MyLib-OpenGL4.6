#pragma once
#include "../include/glad/glad.h"

#include "Shader.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"

#define CANVAS_SCREEN_FRAMEBUFFER_SHADER "shaders/framebuffer/canvas/basicCanvasVert.glsl", "shaders/framebuffer/canvas/screenCanvasFrag.glsl"

#define CANVAS_DEPTH_FRAMEBUFFER_SHADER "shaders/framebuffer/canvas/basicCanvasVert.glsl", "shaders/framebuffer/canvas/depthCanvasFrag.glsl"
#define DEPTH_RENDER_FRAMEBUFFER_SHADER "shaders/framebuffer/depth/depthRenderVert.glsl", "shaders/framebuffer/depth/depthRenderFrag.glsl"

#define GRASS_DEPTH_RENDER_FRAMEBUFFER_SHADER "shaders/framebuffer/depth/grassDepthRenderVert.glsl", "shaders/framebuffer/depth/depthRenderFrag.glsl"

class ScreenFramebufferException{};
class DepthFramebufferException{};

class Framebuffer
{
    protected:
    unsigned int FBO;
    Mesh canvasMesh;
    Shader* canvasShader; //different accross framebuffers
    bool IsComplete();
    
    public:
    unsigned int width, height;//tex has width and height tho...
    Texture2D* tex; //different accross framebuffers
    
    Framebuffer(unsigned int w, unsigned int h);
    virtual void Bind(); //virtual? Don't think so
    void Unbind(unsigned int newViewportW , unsigned int newViewportH);//automatically resizes glViewport to the given values
    void Unbind();
    virtual void Resize(unsigned int newWidth, unsigned int newHeight); //fbo gets resized & texture I think
    virtual void DrawToScreen(unsigned int screenWidth, unsigned int screenHeight); //shader gets used + set uniforms
    virtual ~Framebuffer() = 0; //Framebuffer is abstract
};

class ScreenFramebuffer : public Framebuffer
{
    private:
    unsigned int RBO;

    public:
    ScreenFramebuffer(unsigned int w, unsigned int h);
    void Bind() override;
    void Resize(unsigned int newWidth, unsigned int newHeight) override;
    void DrawToScreen(unsigned int screenWidth, unsigned int screenHeight) override;
    ~ScreenFramebuffer();
};

class DepthFramebuffer : public Framebuffer
{
    private:
    
    public:
    Shader* depthShader; //(convenience)depth shader for default objects (avoids extra setup)
    DepthFramebuffer(unsigned int w, unsigned int h);
    void Bind() override;
    void Resize(unsigned int newWidth, unsigned int newHeight) override;
    void DrawToScreen(unsigned int screenWidth, unsigned int screenHeight) override;
    ~DepthFramebuffer();
};