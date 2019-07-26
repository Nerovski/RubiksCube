#include "MainScene.h"

#include "../cbs/components/FirstPersonController.h"
#include "../cbs/components/Camera.h"
#include "../cbs/components/MeshRenderer/MeshRenderer.h"
#include "../cbs/components/PointLight.h"
#include "../cbs/components/DummyComp.h"

void MainScene::CreateScene() {
    FrameRateLimit(0);
    Skybox("data/skybox/right.jpg",
           "data/skybox/left.jpg",
           "data/skybox/top.jpg",
           "data/skybox/bottom.jpg",
           "data/skybox/back.jpg",
           "data/skybox/front.jpg");

    auto camera = CreateObject("Camera"); {
        camera->Root().Move(camera->Root().Front() * -2.0f);
        auto ccamera = camera->CreateComponent<Camera>(glm::perspective(glm::radians(45.0f), 2880.0f / 1800.0f, 0.1f, 3000.0f));
        auto cpoint_light = camera->CreateComponent<PointLight>(glm::vec3(0.1f),
                                                                glm::vec3(0.8f),
                                                                glm::vec3(0.5f),
                                                                1.0f,
                                                                0.0014f,
                                                                0.000007f);
        auto cfpc = camera->CreateComponent<FirstPersonController>();
        camera->Connect(camera->Root().TransformOut, cfpc->TransformIn);
    }

    glm::vec3 model_scale(1.0f / (976.032f * 2.0f), 1.0f / (976.032f * 2.0f), 1.0f / (986.312f * 2.0f));

    auto first = CreateObject("First"); {
        auto cmesh_renderer = first->CreateComponent<MeshRenderer>("data/earth/13902_Earth_v1_l3.obj",
                                                                   ShaderProgram::Type::PHONG);
        
        auto ctransform = first->CreateComponent<Transform>();
        ctransform->Scale(model_scale);

        auto cdummy = first->CreateComponent<DummyComp>();
        first->Connect(first->Root().TransformOut, ctransform->TransformIn);
        first->Connect(ctransform->TransformOut, cmesh_renderer->TransformIn);
    }
}
