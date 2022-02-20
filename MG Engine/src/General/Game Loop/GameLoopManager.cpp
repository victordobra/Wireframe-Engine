#include "GameLoopManager.h"
#include "InputInternal.h"
#include "EngineTimeInternal.h"
#include "BuildInfo.h"
#include "Node.h"

//Platform specific
#ifdef PLATFORM_WINDOWS
#include "Windows\Windows.h"
#endif

//Testing
#include "ModelRenderer.h"
#include "Camera.h"
#include "CameraController.h"

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
	ModelRenderer* renderer;
	CameraController* cameraController;
	//Main functions
	void StartGameLoop() {
		StartInput();
		LockCursor();

		camera = new Camera();
		camera->SetParent(Node::scene);
		camera->position = { 0.f, 0.f, 5.f };
		camera->clearMode = ClearMode::COLOR;
		camera->clearColor = { 0.01f, 0.01f, 0.01f };

		renderer = new ModelRenderer();
		renderer->SetParent(Node::scene);
		Model* model = new Model("Assets\\Cube.obj");
		renderer->SetModel(model);
		
		cameraController = new CameraController();
		cameraController->SetParent(Node::scene);

		//Run the start function on every object
		RunStartEverything(Node::scene);
	}
	void UpdateGameLoop() {
		//Start the frame
		StartFrame();

		//Get if the window's active
#ifdef PLATFORM_WINDOWS
		bool8_t windowActive = WindowsIsWindowActive();
#endif 
		if (!windowActive) {
			//Wait to reduce processor load and exit the function
			std::this_thread::sleep_for(std::chrono::duration<sint64_t, std::milli>(100));
			EndFrame();
			return;
		}

		//Update the input
		UpdateInput();

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