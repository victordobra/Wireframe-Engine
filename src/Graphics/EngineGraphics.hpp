#pragma once

#include "Core/Types/Defines.hpp"
#include "Main/Instance/MainPipeline.hpp"
#include "Main/Resources/Material.hpp"
#include "Skybox/Instance/SkyboxPipeline.hpp"
#include "Skybox/Resources/Skybox.hpp"

namespace wfe {
    class Program;

    /// @brief A class implementing the engine's graphics functionality.
    class EngineGraphics {
    public:
        /// @brief Creates the engine's graphics components.
        /// @param program The program to create the graphics components for.
        EngineGraphics(Program* program);
        EngineGraphics(const EngineGraphics&) = delete;
        EngineGraphics(EngineGraphics&&) = delete;

        EngineGraphics& operator=(const EngineGraphics&) = delete;
        EngineGraphics& operator=(EngineGraphics&&) = delete;

        /// @brief Gets the program that owns the engine's graphics components.
        /// @return The program that owns the engine's graphics components.
        Program* GetProgram() const {
            return program;
        }
        /// @brief Gets the material manager used by the engine.
        /// @return The material manager used by the engine.
        MaterialManager* GetMaterialManager() const {
            return materialManager;
        }
        /// @brief Gets the main graphics pipeline used by the engine.
        /// @return The main graphics pipeline used by the engine.
        MainPipeline* GetMainPipeline() const {
            return mainPipeline;
        }
        /// @brief Gets the skybox manager used by the engine.
        /// @return The skybox manager used by the engine.
        SkyboxManager* GetSkyboxManager() const {
            return skyboxManager;
        }
        /// @brief Gets the skybox graphics pipeline used by the engine.
        /// @return The skybox graphics pipeline used by the engine.
        SkyboxPipeline* GetSkyboxPipeline() const {
            return skyboxPipeline;
        }

        /// @brief Destroys the engine's graphics components.
        ~EngineGraphics();
    private:
        Program* program;

        MaterialManager* materialManager;
        MainPipeline* mainPipeline;
        SkyboxManager* skyboxManager;
        SkyboxPipeline* skyboxPipeline;
    };
}