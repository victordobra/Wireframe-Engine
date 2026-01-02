#include "EngineGraphics.hpp"
#include "Main/Program.hpp"

namespace wfe {
	// Public functions
	EngineGraphics::EngineGraphics(Program* program) : program(program) {
		// Set the default camera info
		MainPipeline::CameraInfo cameraInfo {
			.pos = VEC3F_ZERO,
			.rot = QUATF_IDENTITY,
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

	void EngineGraphics::RecordRenderingCommand() {
		// Init the rendering command
		GraphicsSystem* graphicsSystem = program->GetGraphicsSystem();
		if(!graphicsSystem->InitCommand())
			return;
		
		VulkanCommand* command = graphicsSystem->GetRenderingCommands()[graphicsSystem->GetFrameIndex()];

		// Add the command stages
		command->AddCommandStage(mainPipeline->GetSceneInfoTransferStageInfo());
		command->AddCommandStage(mainPipeline->GetRenderStageInfo());
		command->AddCommandStage(skyboxPipeline->GetStageInfo());

		// Add the transition stage
		VulkanCommand::CommandStageInfo transitionStageInfo = graphicsSystem->GetPresentLayoutTransitionStageInfo();
		transitionStageInfo.dependencies = { "MainPipelineRender", "SkyboxPipelineRender" };
		command->AddCommandStage(transitionStageInfo);
	
		// Record and submit the command
		command->RecordCommand(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
		graphicsSystem->SubmitCommand();
	}

	EngineGraphics::~EngineGraphics() {
		// Destroy all graphics components
		delete mainPipeline;
		delete materialManager;
		delete skyboxPipeline;
		delete skyboxManager;
	}
}