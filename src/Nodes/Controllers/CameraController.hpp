#pragma once

#include "Base/Node.hpp"
#include "Nodes/Rendering/Camera.hpp"
#include "Systems/Input.hpp"
#include "Systems/Time.hpp"

namespace mge {
    class CameraController : public Node {
    public:
        Camera* camera;
        Vector3 center;
        float32_t sensitivity{.1f};

        inline void Start() override {
            camera = dynamic_cast<Camera*>(GetParent());

            assert(camera && "A camera controller's parent must be a camera!");
        }
        inline void Update() override {
            if(KeyDown(KEY_LBUTTON)) {
                MouseMovement movement = GetMouseMovement();

                Vector2 movementVec = { movement.x * sensitivity * DEG_TO_RAD_MULTIPLIER, movement.y * sensitivity * DEG_TO_RAD_MULTIPLIER };

                Quaternion newRotation = Quaternion::EulerAngles({movementVec.y, movementVec.x, 0});
                camera->rotation *= newRotation;
                
                Vector3 relativePos = camera->position - center;
                relativePos = Matrix4x4::Rotation(newRotation.Inverted()) * relativePos;
                camera->position = relativePos + center;
            }
        }
    };
}