#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "components/simple_scene.h"

#include "lab_m1/Tema2/tema_camera.h"
#include "lab_m1/Tema2/transform3D.h"


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
        Tema2();
        ~Tema2();

        void Init() override;

        Mesh *CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned int> &indices);

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3& color);
        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        int GenerateRandomNumber(int min, int max);
        struct building {
            glm::vec3 position;
            float scale;
        };

        struct tank {
            glm::vec3 position;
        };

        float translateX, translateY, translateZ, angularStepOX = 0;
        //float tun_translateX, tun_translateY, tun_translateZ, tun_angularStepOX = 0;
        bool trage;
        float timerp;

        struct proiectil{
            float x;
            float z;
            float angle;
            bool ok;
            bool back;
            bool front;
        };

        int numtanks;
        bool forward;
        bool backward;
        std::vector<proiectil> p;
        std::vector<building> buildings;
        std::vector<tank> tanks;
        std::vector<glm::vec3> traiectorie;


    protected:
        implemented::CameraT* camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;

    };
}   // namespace m1
