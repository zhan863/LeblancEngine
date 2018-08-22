#ifndef LEBLANC_RENDER_COMMAND_H
#define LEBLANC_RENDER_COMMAND_H

#include "LeblancEngine/Render/RenderState/LeblancRenderPipelineState.h"

namespace Leblanc
{
	class Material;
	class IShader;
	class Mesh;

	class RenderCommand
	{
	public:
		RenderCommand(PipelineState state, const Material* material, const IShader* shader, const Mesh* mesh) :
			m_state_cache(state), m_material(material), m_shader(shader), m_mesh(mesh){}
		
		~RenderCommand() {}

		void execute();

	protected:
		// reference
		PipelineState m_state_cache;
		const Material* m_material = nullptr;
		const IShader* m_shader = nullptr;
		const Mesh* m_mesh = nullptr;
	};
}
#endif