#include "Framebuffer.hpp"

Framebuffer::Framebuffer(unsigned int w, unsigned int h):
width(w),height(h),canvasMesh(Mesh::createCanvas())
{
    glGenFramebuffers(1, &FBO);
}

bool Framebuffer::IsComplete()
{
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) return true;
    else return false;
}

void Framebuffer::Bind()
{
    glViewport(0, 0, width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void Framebuffer::Unbind(unsigned int newViewportW, unsigned int newViewportH)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, newViewportW, newViewportH);
}

void Framebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
//find things in common here
void Framebuffer::Resize(unsigned int newWidth, unsigned int newHeight) {}
void Framebuffer::DrawToScreen(unsigned int screenWidth, unsigned int screenHeight) {}

Framebuffer::~Framebuffer(){}

ScreenFramebuffer::ScreenFramebuffer(unsigned int w, unsigned int h):Framebuffer(w, h)
{
    canvasShader = new Shader(CANVAS_SCREEN_FRAMEBUFFER_SHADER);
    tex = new Texture2D(w, h);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->GetHandler(), 0);

    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    if(!Framebuffer::IsComplete())
    {
        //throw
        std::cout << "ScreenFramebuffer is incomplete!\n";
        exit(EXIT_FAILURE);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ScreenFramebuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glViewport(0, 0, width, height);
}

void ScreenFramebuffer::Resize(unsigned int newWidth, unsigned int newHeight)
{
    width = newWidth;
    height = newHeight;

    glBindFramebuffer(GL_FRAMEBUFFER, FBO); //need this? OpenGL is magic

    glBindTexture(GL_TEXTURE_2D, tex->GetHandler());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newWidth, newHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->GetHandler(), 0);

    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, newWidth, newHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    glBindFramebuffer(GL_FRAMEBUFFER, 0); //need this?
}

void ScreenFramebuffer::DrawToScreen(unsigned int screenWidth, unsigned int screenHeight)
{
    glViewport(0, 0, screenWidth, screenHeight);
    glDisable(GL_DEPTH_TEST);
    glClearColor(1.f, 1.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    canvasShader->Use();
    canvasShader->SetUniformInt("screenTex", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex->GetHandler());
    canvasMesh.Draw();
}

ScreenFramebuffer::~ScreenFramebuffer()
{
    glDeleteFramebuffers(1, &FBO);
    glDeleteRenderbuffers(1, &RBO);
    delete canvasShader;
    delete tex;
}

DepthFramebuffer::DepthFramebuffer(unsigned int w, unsigned int h):Framebuffer(w, h)
{
    depthShader = new Shader(DEPTH_RENDER_FRAMEBUFFER_SHADER); //this one needs to be external...Vertex shaders may differ
    canvasShader = new Shader(CANVAS_DEPTH_FRAMEBUFFER_SHADER);
    tex = new Texture2D(w, h, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex->GetHandler(), 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    
    if(!Framebuffer::IsComplete())
    {
        // throw
        std::cout << "DepthFramebuffer is incomplete!\n";
        exit(EXIT_FAILURE);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DepthFramebuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void DepthFramebuffer::Resize(unsigned int newWidth, unsigned int newHeight)
{}

void DepthFramebuffer::DrawToScreen(unsigned int screenWidth, unsigned int screenHeight)
{
    glViewport(0, 0, screenWidth, screenHeight);
    glDisable(GL_DEPTH_TEST);
    glClearColor(1.f, 1.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    canvasShader->Use();
    canvasShader->SetUniformInt("depthTex", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex->GetHandler());
    canvasMesh.Draw();
}

DepthFramebuffer::~DepthFramebuffer()
{
    glDeleteFramebuffers(1, &FBO);
    delete canvasShader;
    delete tex;
}