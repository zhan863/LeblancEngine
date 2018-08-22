#include "LeblancEngine/Render/RenderPass/MeshPass/LeblancMeshRenderPass.h"

namespace Leblanc
{
	MeshRenderPass::MeshRenderPass()
	{

	}

	MeshRenderPass::~MeshRenderPass()
	{

	}

	void MeshRenderPass::setScene(const Scene* scene)
	{
		m_scene = scene;
	}
}