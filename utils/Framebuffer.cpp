#include "Framebuffer.hpp"

Framebuffer::Framebuffer(unsigned int width, unsigned int height):width(width),height(height){}
unsigned int Framebuffer::GetTexture()
{
    return Tex;
}
void Framebuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}
void Framebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
unsigned int Framebuffer::GetWidth()
{
    return width;
}
unsigned int Framebuffer::GetHeight()
{
    return height;
}
Framebuffer::~Framebuffer(){} //?????

ScreenFramebuffer::ScreenFramebuffer(unsigned int width, unsigned int height) : Framebuffer(width, height)
{
    ScreenFramebuffer::InitVAO();
    ScreenFramebuffer::InitShader();

    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glGenTextures(1, &Tex);
    glBindTexture(GL_TEXTURE_2D, Tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Tex, 0);

    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void ScreenFramebuffer::InitVAO()
{
    float canvasVertices[] = 
    {
        -1.f, -1.f,    0.f, 0.f,
        1.f, 1.f,      1.f, 1.f,
        -1.f, 1.f,     0.f, 1.f,
        1.f, -1.f,     1.f, 0.f
    };
    unsigned int canvasIndices[] = 
    {
        0, 1, 2,
        0, 3, 1
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), canvasVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), canvasIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(0));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void ScreenFramebuffer::InitShader()
{
    shader = new Shader("shaders/framebuffer/vertSFB.glsl", "shaders/framebuffer/fragSFB.glsl");
}
void ScreenFramebuffer::Draw()
{
    shader->Use();
    shader->SetUniformInt("screenTex", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Tex);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
void ScreenFramebuffer::Resize(unsigned int newWidth, unsigned int newHeight)
{
    if(newWidth <= 0 || newHeight <= 0) return; //no reason to check '<' but still
    width = newWidth;
    height = newHeight;

    glBindTexture(GL_TEXTURE_2D, Tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newWidth, newHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Tex, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, newWidth, newHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
}
ScreenFramebuffer::~ScreenFramebuffer()
{
    delete shader;
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glDeleteRenderbuffers(1, &RBO);
    glDeleteFramebuffers(1, &FBO);
    glDeleteTextures(1, &Tex);
}