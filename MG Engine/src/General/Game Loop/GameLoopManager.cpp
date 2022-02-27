#include "GameLoopManager.h"
#include "InputInternal.h"
#include "EngineTimeInternal.h"
#include "Node.h"

//Testing
#include "ModelRenderer.h"
#include "Camera.h"
#include "CameraController.h"
#include "DirectionalLight.h"
#include "PointLight.h"

#include <future>

namespace mge {
	//Helper functions
	static void RunStart(Node* node) {
		node->Start();
	}
	static void RunFrame(Node* node) {
		node->Frame();
	}
	static void RunRender(Node* node) {
		node->Render();
	}
	static void RunStartEverything(Node* start) {
		//Create the array of futures
		size_t childCount = start->GetChildren().size();
		std::future<void>* futures = new std::future<void>[childCount + 1];

		/// launch the async functions
		futures[0] = std::async(std::launch::deferred | std::launch::async, RunStart, start);

		for (size_t i = 1; i < childCount + 1; i++)
			futures[i] = std::async(std::launch::deferred | std::launch::async, RunStartEverything, start->GetChildren()[i - 1]);

		//Wait for the functions to finish
		for (size_t i = 0; i < childCount + 1; i++)
			futures[i].wait();

		//Delete the futures
		delete[] futures;
	}
	static void RunFrameEverything(Node* start) {
		//Create the array of futures
		size_t childCount = start->GetChildren().size();
		std::future<void>* futures = new std::future<void>[childCount + 1];

		/// launch the async functions
		futures[0] = std::async(std::launch::deferred | std::launch::async, RunFrame, start);

		for (size_t i = 1; i < childCount + 1; i++)
			futures[i] = std::async(std::launch::deferred | std::launch::async, RunFrameEverything, start->GetChildren()[i - 1]);

		//Wait for the functions to finish
		for (size_t i = 0; i < childCount + 1; i++)
			futures[i].wait();

		//Delete the futures
		delete[] futures;
	}
	static void RunRenderEverything(Node* start) {
		//Create the array of futures
		size_t childCount = start->GetChildren().size();
		std::future<void>* futures = new std::future<void>[childCount + 1];

		/// launch the async functions
		futures[0] = std::async(std::launch::deferred | std::launch::async, RunRender, start);

		for (size_t i = 1; i < childCount + 1; i++)
			futures[i] = std::async(std::launch::deferred | std::launch::async, RunRenderEverything, start->GetChildren()[i - 1]);

		//Wait for the functions to finish
		for (size_t i = 0; i < childCount + 1; i++)
			futures[i].wait();

		//Delete the futures
		delete[] futures;
	}

	Camera* camera;
	CameraController* cameraController;
	ModelRenderer* renderer1;
	ModelRenderer* renderer2;
	DirectionalLight* dirLight1;
	DirectionalLight* dirLight2;

	float32_t yRot = 0.f;

	//Main functions
	void StartGameLoop() {
		StartInput();
		LockCursor();

		camera = new Camera();
		camera->SetParent(Node::scene);
		camera->position = { 0.f, 0.f, 5.f };
		camera->clearMode = ClearMode::COLOR;
		camera->clearColor = { 0.01f, 0.01f, 0.01f };

		Model* model = new Model("Assets\\Teapot.obj");

		Material* material1 = new Material();
		material1->color = { 1.f, 1.f, 1.f, 1.f };
		Material* material2 = new Material();
		material2->color = { 0.5f, 0.5f, 1.f, 1.f };

		renderer1 = new ModelRenderer();
		renderer1->position = { 0.f, 0.f, 0.f };
		renderer1->material = material1;
		renderer1->SetParent(Node::scene);
		renderer1->SetModel(model);

		renderer2 = new ModelRenderer();
		renderer2->position = { 0.f, 0.f, -10.f };
		renderer2->rotation = Quaternion::AroundAxis({ 0.f, 1.f, 0.f }, 180.f * DEG_TO_RAD_MULTIPLIER);
		renderer2->material = material2;
		renderer2->SetParent(Node::scene);
		renderer2->SetModel(model);

		dirLight1 = new DirectionalLight();
		dirLight1->SetParent(Node::scene);
		dirLight1->direction = { 1.f, 0.f, 0.f };
		dirLight1->color = { 1.f, 1.f, 1.f, 1.f };

		dirLight2 = new DirectionalLight();
		dirLight2->SetParent(Node::scene);
		dirLight2->direction = { -1.f, 0.f, 0.f };
		dirLight2->color = { 1.f, 0.f, 0.f, 1.f };
		
		cameraController = new CameraController();
		cameraController->SetParent(Node::scene);

		//Run the start function on every object
		RunStartEverything(Node::scene);
	}
	void UpdateGameLoop() {
		//Start the frame
		StartFrame();

		//Update the input
		UpdateInput();

		yRot += 90.f * GetDeltaTime();
		if (yRot > 360.f)
			yRot -= 360.f;

		dirLight1->direction = { Cos(yRot * DEG_TO_RAD_MULTIPLIER), 0.f, -Sin(yRot * DEG_TO_RAD_MULTIPLIER) };
		dirLight2->direction = { Cos((yRot + 180.f) * DEG_TO_RAD_MULTIPLIER), 0.f, -Sin((yRot + 180.f) * DEG_TO_RAD_MULTIPLIER) };

		//Run the frame and render functions on every object
		RunFrameEverything(Node::scene);
		RunRenderEverything(Node::scene);

		//End the frame
		EndFrame();
	}
	void EndGameLoop() {
		//Delete the scene object; this will cause a chain reaction
		delete Node::scene;
	}
}