#include "EngineGraphics.hpp"
#include "Main/Program.hpp"

namespace wfe {
    // Public functions
    EngineGraphics::EngineGraphics(Program* program) : program(program) {
		// Set the default camera info
		MainPipeline::CameraInfo cameraInfo {
			.pos = Vector3::ZERO,
			.rot = Quaternion::IDENTITY,
			.cameraType = MainPipeline::CAMERA_TYPE_PERSPECTIVE,
			.perspectiveInfo = {
				.fov = (float)M_PI_2,
				.nearPlane = 0.001f,
				.farPlane = 1000.f
			}
		};

        // Create the material manager and the main graphics pipeline
        materialManager = new MaterialManager(program, program->GetProgramSettings().maxMaterialCount);
        mainPipeline = new MainPipeline(this, cameraInfo);

        // Create the skybox manager and the skybox graphics pipeline
        skyboxManager = new SkyboxManager(program, program->GetProgramSettings().maxSkyboxCount);
        skyboxPipeline = new SkyboxPipeline(this);
    }

    EngineGraphics::~EngineGraphics() {
        // Destroy all graphics components
        delete mainPipeline;
        delete materialManager;
        delete skyboxPipeline;
        delete skyboxManager;
    }
}