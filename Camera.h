#pragma once
#include "Object3D.h"

struct ConstBufferDataVProj {
    Matrix vproj;
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
    Camera();
    void UpdateShake();
    void SetRenderSize(UINT w, UINT h);

    Float3 target = {0.0f, 0.0f, 0.0f};

    Float3 actualPosition;

    float fov;

    UINT renderWidth;
    UINT renderHeight;

    float nearZ;
    float farZ;

    int shakeTimer;

    wConstBuffer<ConstBufferDataVProj> cameraViewProjMatrixCB;

    CameraTargetMode targetMode = CameraTargetMode::LookTo;

    static void Set(Camera& camera);
    void Shake();

    ProjectionMode projectionMode = ProjectionMode::Perspective;
};

