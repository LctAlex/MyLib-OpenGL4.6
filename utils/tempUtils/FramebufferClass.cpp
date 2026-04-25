#include "FramebufferClass.hpp"

void Framebuffer::InitVAO()
{
    float canvasVertices[] = {
        -1.f, -1.f, 0.f, 0.f,
        1.f, 1.f, 1.f, 1.f,
        -1.f, 1.f, 0.f, 1.f,
        1.f, -1.f, 1.f, 0.f};
    unsigned int canvasIndices[] = {
        0, 1, 2,
        0, 3, 1};

    glGenVertexArrays(1, &canvasVAO);
    glBindVertexArray(canvasVAO);

    glGenBuffers(1, &canvasVBO);
    glBindBuffer(GL_ARRAY_BUFFER, canvasVBO);
    glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), canvasVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &canvasEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, canvasEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), canvasIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(0));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Framebuffer::InitShader()
{
    canvasShader = new Shader("shaders/framebuffer/vertFB.glsl", "shaders/framebuffer/fragFB.glsl");
}

Framebuffer::Framebuffer(unsigned int width, unsigned int height, int FB_TYPE_C, int FB_TYPE_A) : fb_type(FB_TYPE_C, FB_TYPE_A) // FB_TYPE_... for solutions
{
    Framebuffer::InitVAO();
    Framebuffer::InitShader();
    this->width = width;
    this->height = height;

    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glGenTextures(1, &TexColorBuffer);
    glBindTexture(GL_TEXTURE_2D, TexColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TexColorBuffer, 0);

    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "ERROR::FRAMEBUFFER::INCOMPLETE";
        exit(EXIT_FAILURE);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Resize(unsigned int newWidth, unsigned int newHeight)
{
    this->width = newWidth;
    this->height = newHeight;

    glBindTexture(GL_TEXTURE_2D, TexColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newWidth, newHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TexColorBuffer, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, newWidth, newHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
}

unsigned int Framebuffer::GetWidth()
{
    return width;
}

unsigned int Framebuffer::GetHeight()
{
    return height;
}

void Framebuffer::BindTexture()
{
    glBindTexture(GL_TEXTURE_2D, TexColorBuffer);
}

void Framebuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void Framebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Draw()
{
    canvasShader->Use();
    canvasShader->SetUniformInt("screenTex", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TexColorBuffer);
    glBindVertexArray(canvasVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Framebuffer::Delete()
{
    glDeleteBuffers(1, &canvasVBO);
    glDeleteBuffers(1, &canvasEBO);
    glDeleteVertexArrays(1, &canvasVAO);
    delete canvasShader;
    glDeleteFramebuffers(1, &FBO);
    glDeleteRenderbuffers(1, &RBO);
    glDeleteTextures(1, &TexColorBuffer);
}