#include "LeblancEngine/Render/RenderPass/MeshPass/LeblancGBufferPass.h"
#include "LeblancEngine/Render/RenderEntity/LeblancMesh.h"
#include "LeblancEngine/Render/Material/LeblancMaterial.h"
#include "LeblancEngine/Render/Scene/LeblancScene.h"
#include "LeblancEngine/Render/RenderCommand/LeblancRenderCommand.h"
#include <vector>
using namespace std;

namespace Leblanc
{
	GBufferPass::GBufferPass()
	{

	}

	GBufferPass::~GBufferPass()
	{

	}

	void GBufferPass::render()
	{
		if (m_scene)
		{
			const vector<Mesh*>& meshes = m_scene->meshForPass("gbuffer");

			for (int i = 0; i < meshes.size(); i++)
			{
				RenderCommand command(PipelineState(), meshes[i]->getMaterial(), meshes[i]->getMaterial()->getShader(), meshes[i]);
				command.execute();
			}
		}
	}
}