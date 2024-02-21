#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include <cstdlib>  
#include <ctime>    


using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()

{
    int numBuildings = GenerateRandomNumber(5, 10);

    for (int i = 0; i < numBuildings; ++i) {
        float buildingX = static_cast<float>(GenerateRandomNumber(-10, 10));
        float buildingZ = static_cast<float>(GenerateRandomNumber(-10, 10));
        float buildingScale = static_cast<float>(GenerateRandomNumber(0.5, 2));

        buildings.push_back({ glm::vec3(buildingX, 1.5 , buildingZ), buildingScale });
    }

    numtanks = GenerateRandomNumber(5, 10);

    for (int i = 0; i < numtanks; ++i) {
        float tankX = static_cast<float>(GenerateRandomNumber(-10, 10));
        float tankZ = static_cast<float>(GenerateRandomNumber(-10, 10));

        tanks.push_back({ glm::vec3(tankX, 0 , tankZ) });
    }

    timerp = 0;

    renderCameraTarget = false;

    translateX = 0;
    translateY = 0;
    translateZ = 0;
    angularStepOX = 0;

    forward = false;
    backward = false;

   /* tun_translateX = 0;
    tun_translateY = 0;
    tun_translateZ = 0;
    tun_angularStepOX = 0;*/

    trage = false;



    { 
        {
            Mesh* senile = new Mesh("senile");
            senile->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "my_models"), "senile.obj");
            meshes[senile->GetMeshID()] = senile;
        }

        {
            Mesh* corp = new Mesh("corp");
            corp->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "my_models"), "corp.obj");
            meshes[corp->GetMeshID()] = corp;
        }

        {
            Mesh* turela = new Mesh("turela");
            turela->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "my_models"), "turela.obj");
            meshes[turela->GetMeshID()] = turela;
        }

        {
            Mesh* tun = new Mesh("tun");
            tun->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "my_models"), "tun.obj");
            meshes[tun->GetMeshID()] = tun;
        }

        {
            Mesh* proiectil = new Mesh("proiectil");
            proiectil->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "my_models"), "proiectil.obj");
            meshes[proiectil->GetMeshID()] = proiectil;
        }

        {
            Mesh* cladire = new Mesh("cladire");
            cladire->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
            meshes[cladire->GetMeshID()] = cladire;
        }

    }

    // Create a simple cube
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-1, -1,  1), glm::vec3(0, 1, 1), glm::vec3(0.2, 0.8, 0.2)),
            VertexFormat(glm::vec3( 1, -1,  1), glm::vec3(1, 0, 1), glm::vec3(0.9, 0.4, 0.2)),
            VertexFormat(glm::vec3(-1,  1,  1), glm::vec3(1, 0, 0), glm::vec3(0.7, 0.7, 0.1)),
            VertexFormat(glm::vec3( 1,  1,  1), glm::vec3(0, 1, 0), glm::vec3(0.7, 0.3, 0.7)),
            VertexFormat(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1), glm::vec3(0.3, 0.5, 0.4)),
            VertexFormat(glm::vec3( 1, -1, -1), glm::vec3(0, 1, 1), glm::vec3(0.5, 0.2, 0.9)),
            VertexFormat(glm::vec3(-1,  1, -1), glm::vec3(1, 1, 0), glm::vec3(0.7, 0.0, 0.7)),
            VertexFormat(glm::vec3( 1,  1, -1), glm::vec3(0, 0, 1), glm::vec3(0.1, 0.5, 0.8)),
        };

        vector<unsigned int> indices =
        {
            0, 1, 2,        1, 3, 2,
            2, 3, 7,        2, 7, 6,
            1, 7, 3,        1, 5, 7,
            6, 7, 4,        7, 5, 4,
            0, 4, 1,        1, 4, 5,
            2, 6, 4,        0, 2, 4,
        };

        CreateMesh("cube", vertices, indices);
    }

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader *shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}


Mesh* Tema2::CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned int> &indices)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // ========================================================================
    // This section demonstrates how the GPU vertex shader program
    // receives data.

    // TODO(student): If you look closely in the `Init()` and `Update()`
    // functions, you will see that we have three objects which we load
    // and use in three different ways:
    // - LoadMesh   + LabShader (this lab's shader)
    // - CreateMesh + VertexNormal (this shader is already implemented)
    // - CreateMesh + LabShader (this lab's shader)
    // To get an idea about how they're different from one another, do the
    // following experiments. What happens if you switch the color pipe and
    // normal pipe in this function (but not in the shader)? Now, what happens
    // if you do the same thing in the shader (but not in this function)?
    // Finally, what happens if you do the same thing in both places? Why?

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
    return meshes[name];
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{

    for (int i = 0; i < numtanks; ++i) {
        float tankX = static_cast<float>(GenerateRandomNumber(-2, 2));
        float tankZ = static_cast<float>(GenerateRandomNumber(-2, 2));

        traiectorie.push_back(glm::vec3(tankX, 0, tankZ));
    }

    for (const auto& building : buildings) {
        glm::mat4 buildingModelMatrix = glm::mat4(1);
        buildingModelMatrix = glm::translate(buildingModelMatrix, building.position);
        buildingModelMatrix = glm::scale(buildingModelMatrix, glm::vec3(building.scale));

        RenderSimpleMesh(meshes["cladire"], shaders["LabShader"], buildingModelMatrix, glm::vec3(0.204, 0.102, 0));
    }

    for (int i = 0; i < tanks.size(); ++i) {
        tanks[i].position += traiectorie[i] * deltaTimeSeconds;

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, tanks[i].position);
        modelMatrix *= transform3D::RotateOY(RADIANS(-90) + RADIANS(angularStepOX));
        RenderSimpleMesh(meshes["senile"], shaders["LabShader"], modelMatrix, glm::vec3(0.6, 0, 0.6));
        RenderSimpleMesh(meshes["corp"], shaders["LabShader"], modelMatrix, glm::vec3(1, 0.2, 0.6));
        RenderSimpleMesh(meshes["turela"], shaders["LabShader"], modelMatrix, glm::vec3(0.8, 0, 0.6));
        RenderSimpleMesh(meshes["tun"], shaders["LabShader"], modelMatrix, glm::vec3(0.6, 0, 0.6));
    }

        camera = new implemented::CameraT();
        camera->Set(glm::vec3(translateX, translateY, translateZ), glm::vec3(translateX - 10, translateY - 10, translateZ - 10), glm::vec3(0, 1, 0));

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(translateX, translateY, translateZ));
        modelMatrix *= transform3D::RotateOY(RADIANS(-90) + RADIANS(angularStepOX));
        RenderSimpleMesh(meshes["senile"], shaders["LabShader"], modelMatrix, glm::vec3(0.09, 0.561, 0.067));
        RenderSimpleMesh(meshes["corp"], shaders["LabShader"], modelMatrix, glm::vec3(0.91, 0.439, 0.055));
        RenderSimpleMesh(meshes["turela"], shaders["LabShader"], modelMatrix, glm::vec3(0.969, 0.925, 0.102));
        RenderSimpleMesh(meshes["tun"], shaders["LabShader"], modelMatrix, glm::vec3(0.09, 0.561, 0.067));

        for (int i = 0; i < p.size(); i++)
        {
            if (p[i].ok == true)
            {
                glm::vec3 backwardDirection = glm::vec3(0.0f, 0.0f, 1.0f);
                glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1), glm::radians(p[i].angle), glm::vec3(0, 1, 0));
                backwardDirection = glm::vec3(rotationMatrix * glm::vec4(backwardDirection, 0.0f));

                p[i].x -= backwardDirection.x * deltaTimeSeconds * 3;
                p[i].z -= backwardDirection.z * deltaTimeSeconds * 3;
			}
		}
    
        for (int i = 0; i < p.size(); i++)
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(p[i].x, translateY, p[i].z));
            modelMatrix *= transform3D::RotateOY(RADIANS(-90) + RADIANS(angularStepOX));
            RenderSimpleMesh(meshes["proiectil"], shaders["LabShader"], modelMatrix, glm::vec3(0.204, 0.102, 0));
        }

        timerp += deltaTimeSeconds;

    if (Engine::GetElapsedTime() >= 120)
        exit(0);
}


void Tema2::FrameEnd()
{
    DrawCoordinateSystem();
}

int Tema2::GenerateRandomNumber(int min, int max)
{
    return rand() % (max - min + 1) + min;
}


void Tema2::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // TODO(student): Get shader location for uniform mat4 "Model"

    int location1 = glGetUniformLocation(shader->program, "Model");

    // TODO(student): Set shader uniform "Model" to modelMatrix

    glUniformMatrix4fv(location1, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // TODO(student): Get shader location for uniform mat4 "View"

    int location2 = glGetUniformLocation(shader->program, "View");

    // TODO(student): Set shader uniform "View" to viewMatrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    glUniformMatrix4fv(location2, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    int v_color = glGetUniformLocation(shader->program, "v_color");
    glUniform3f(v_color, color.r, color.g, color.b);

    // TODO(student): Get shader location for uniform mat4 "Projection"

    int location3 = glGetUniformLocation(shader->program, "Projection");

    // TODO(student): Set shader uniform "Projection" to projectionMatrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    glUniformMatrix4fv(location3, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    int time = glGetUniformLocation(shader->program, "Time");
    glUniform1f(time, Engine::GetElapsedTime());

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    // Add key press event
    if (window->KeyHold(GLFW_KEY_W))
    {
        glm::vec3 forwardDirection = glm::vec3(0.0f, 0.0f, -1.0f); 

        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1), glm::radians(angularStepOX), glm::vec3(0, 1, 0));
        forwardDirection = glm::vec3(rotationMatrix * glm::vec4(forwardDirection, 0.0f));

        translateX += forwardDirection.x * deltaTime * 5;
        translateZ += forwardDirection.z * deltaTime * 5;
        forward = true;
        backward = false;
       // tun_translateX = translateX;
       // tun_translateZ = translateZ;
    }

    if (window->KeyHold(GLFW_KEY_S))
    {
        glm::vec3 backwardDirection = glm::vec3(0.0f, 0.0f, 1.0f); 

      
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1), glm::radians(angularStepOX), glm::vec3(0, 1, 0));
        backwardDirection = glm::vec3(rotationMatrix * glm::vec4(backwardDirection, 0.0f));

        translateX += backwardDirection.x * deltaTime * 5;
        translateZ += backwardDirection.z * deltaTime * 5;
        forward = false;
        backward = true;
        //tun_translateX = translateX; 
        //tun_translateZ = translateZ;

    }


    if (window->KeyHold(GLFW_KEY_A))
    {
        angularStepOX += deltaTime * 20;
    }
    if (window->KeyHold(GLFW_KEY_D))
    {
        angularStepOX -= deltaTime * 20;
    }
}

void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (button == GLFW_MOUSE_BUTTON_2 && timerp >= 1)
    {
		trage = true;
        if (trage == true)
        {
            proiectil p1;
            p1.x = translateX;
            p1.z = translateZ;
            p1.angle = angularStepOX;
            p1.back = backward;
            p1.front = forward;
            p1.ok = true;
            p.push_back(p1);
            trage = false;
            timerp = 0;
        }
    }
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
