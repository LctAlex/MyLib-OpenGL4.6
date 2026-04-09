#include <iostream>
#include <vector>

#include <../include/glad/glad.h>
#include <../include/GLFW/glfw3.h>

#include "../utils/WINDOW.hpp"
#include "../utils/MESH.hpp"
#include "../utils/SHADER.hpp"
#include "../utils/CAMERA.hpp"
#include "../utils/VAO.hpp"
//FRAMEBUFFER

int main()
{
    SetWindowHint(UNRESIZABLE_WINDOW);
    Window window(800, 800, "Framebuffer");

    Camera camera(window.GetPointer(), glm::vec3(0.f, 0.f, 3.f));
    window.EnableCursor();

    // const float vertices[] =
    //         {
    //             -1.f, 1.f, 0.f,
    //             -1.f, -1.f, 0.f,
    //             1.f, 1.f, 0.f,
    //             1.f, -1.f, 0.f
    //         };

    const float vertices[] =
    {
        -0.5f, 0.5f, 0.f,
        -0.5f, -0.5f, 0.f,
        .5f, .5f, 0.f,
        .5f, -.5f, 0.f
    };

    const unsigned int indices[] =
    {
        0, 1, 2,
        2, 1, 3
    };

    Mesh mesh(vertices, indices, 12, 6, 3, 3);
    Shader shader("shaders/vertMVPpc.glsl", "shaders/fragpc.glsl");

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)window.GetWidth() / (float)window.GetHeight(), 0.1f, 100.f);

    int resScale = 10; //resolution can ONLY go down lol. SetResolution(newWidth, newHeight);
    //new resolution must also have the same...AspectRatio???
    ////Rendering to a texture
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    unsigned int texColorbuffer;
    glGenTextures(1, &texColorbuffer);
    glBindTexture(GL_TEXTURE_2D, texColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window.GetWidth()/resScale, window.GetHeight()/resScale, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); //alloc mem
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //glBindTexture(GL_TEXTURE_2D, 0); //unbind it??
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorbuffer, 0);

    //to make sure OpenGL can also perform depth (and optionally stencil) testing, we add the attachments to the framebuffer:
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window.GetWidth()/resScale, window.GetHeight()/resScale); //allocated memory
    glBindRenderbuffer(GL_RENDERBUFFER, 0); //unbind

    //NOTE: we'll ALSO need to update framebuffer_resize_callback:
    //when the user resizes the window, the main Framebuffer (FBO) and it's renderbuffer (RBO) must also be UPDATED!
    //(therefore we must store the resolution X,Y inside variables in Window class)

    //now we attach the renderbuffer to the framebuffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    //check for completion
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "ERROR::FRAMEBUFFER::INCOMPLETE_FRAMEBUFFER\n";
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return 0;
    }
    else 
    {
        std::cout << "\nFRAMEBUFFER COMPLETED!\n";
        glBindFramebuffer(GL_FRAMEBUFFER, 0); //I NEED TO UNBIND IT AFTER INITIALIZING
    }

    //framebuffer QUAD data:
    const float QUADvertices[] =
    { 
        -1.f, 1.f,      0.f, 1.f,
        -1.f, -1.f,     0.f, 0.f,
        1.f, 1.f,       1.f, 1.f,
        1.f, -1.f,       1.f, 0.f
    };

    const unsigned int QUADindices[] =
    {
        0, 1, 2,
        2, 1, 3
    };

    VAO FBvao(QUADvertices, 16, QUADindices, 6, 2, 4); //we'll have a simple, hardcoded one inside Window. We'll ALWAYS draw over a Framebuffer
    //Mesh FBvao(QUADvertices, QUADindices, 8, 6, 2, 2);
    Shader FBshader("framebufferShaders/vertFB.glsl", "framebufferShaders/fragFB.glsl");
    FBshader.Use();
    //FBshader.SetUniformInt("screenTex", 0); //works without this, don't know how

    while (!window.ShouldClose())
    {
        window.StartUpdate();
        camera.Update(&window);
        
        //NOTE: actual drawing will happen here: after StartUpdate() binds framebuffer, then EndUpdate() will take care of the rest
        //Also after I get Framebuffer to work, I should make functionalities to use it with effects...I WILL need uniforms

        //Pass 1, drawing on the framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer); //from here everything is drawn onto the framebuffer
        // window.ClearBackground();//default clear color DOES get drawn onto the framebuffer
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, window.GetWidth() / resScale, window.GetHeight() / resScale);
        //...but not the rest:
        model = glm::rotate(model, glm::radians(1.f), glm::vec3(0.f, 0.f, 1.f));
        shader.Use();
        shader.SetUniformMat4("view", camera.GetView());
        shader.SetUniformMat4("model", model);
        shader.SetUniformMat4("projection", projection);
        mesh.Draw();

        //Pass 2, the actual drawing on the screen
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        window.ClearBackground(1.0f, 1.0f, 1.0f, 1.0f);

        glDisable(GL_DEPTH_TEST); // A MUST in order to draw the framebuffer over the ClearColor
        glViewport(0, 0, window.GetWidth(), window.GetHeight());
        FBshader.Use();
        FBvao.Bind();
        glBindTexture(GL_TEXTURE_2D, texColorbuffer);
        //glPolygonMode(GL_FRONT, GL_LINE);
        FBvao.Draw(6);
        FBvao.Unbind();
        window.ShowFPS();
        window.EndUpdate();
    }
    window.Close();
    shader.Delete();
    mesh.Delete();

    glDeleteFramebuffers(1, &framebuffer);
    glDeleteRenderbuffers(1, &rbo);
    FBvao.Delete();
    FBshader.Use();
    return 0;
}

//NOTE: when setting resolution/resizing window, the viewport must also be adjusted for the framebuffer, otherwise weird things happen
//TASK: Implement the framebuffer into the Window now. FUCK I NEED 2D and 3D windows / methods!!!