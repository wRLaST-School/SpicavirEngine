#pragma once
#include "Object3D.h"

struct ConstBufferDataVProj {
    Matrix vproj;
    Matrix billboardMat;
    Float3 cameraPos;
};

enum class CameraTargetMode {
    LookAt,
    LookTo
};

enum class ProjectionMode {
    Orthographic,
    Perspective
};

class Camera :
    public Object3D
{
public:
    ComponentFactoryRegister(Camera)

    Camera();
    void SetRenderSize(float w, float h);
    void UseDefaultParams();

    Float3 GetWorldPosFromScreen(const Float2& screen, float depth);
    Ray GetScreenPosRay(const Float2& screen);

    //UseCurrentより後に呼ばれる場合のみ動作保証
    Matrix GetViewMat();
    //UseCurrentより後に呼ばれる場合のみ動作保証
    Matrix GetProjMat();

    Matrix GetBillboardMat();

    void DrawParams() override;

    Float3 target = { 0.0f, 0.0f, 0.0f };

    //レンダーサイズ
    float renderWidth;
    float renderHeight;

    bool useWindowSize = false;

    float nearZ;
    float farZ;
    float fov;

    SpConstBuffer<ConstBufferDataVProj> cameraViewProjMatrixCB;

    CameraTargetMode targetMode = CameraTargetMode::LookTo;

    static void Set(Camera& camera);
    static Camera* sCurrent;
    static void UseCurrent();

    static Matrix GetCurrentCameraBillboardMat();

    ProjectionMode projectionMode = ProjectionMode::Perspective;

    void ReadParamJson([[maybe_unused]] const nlohmann::json& jsonObject) override;
    void WriteParamJson([[maybe_unused]] nlohmann::json& jsonObject) override;

private:
    Matrix view;
    Matrix proj;
};