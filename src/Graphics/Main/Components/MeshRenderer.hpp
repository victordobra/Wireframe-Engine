#pragma once

#include "Core/ECS/ComponentType.hpp"
#include "Core/Types/Defines.hpp"
#include "Graphics/Main/Resources/Material.hpp"
#include "Graphics/Main/Resources/RenderMesh.hpp"

namespace wfe {
	/// @brief A component indicating a mesh being rendered using the main graphics pipeline.
	struct MeshRenderer {
	public:
		/// @brief The mesh to render.
		RenderMesh* mesh;
		/// @brief The material to use when rendering the mesh.
		Material* material;
	private:
		WFE_COMPONENT_TYPE(MeshRenderer)
	};
}