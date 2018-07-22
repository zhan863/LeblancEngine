#ifndef LEBLANC_TEXTURE_D3D11_H
#define LEBLANC_TEXTURE_D3D11_H

#include "LeblancEngine/Render/Texture/LeblancTexture.h"
#include <vector>

using namespace std;

struct ID3D11Resource;
struct ID3D11ShaderResourceView;

namespace Leblanc
{
	class TextureD3D11 : public ITexture
	{
	public:
		TextureD3D11(IDevice* device);

		virtual ~TextureD3D11();

		virtual bool initialize(const TextureParameters* texture_parameters);

		virtual ID3D11Resource* getResource();

		virtual ID3D11ShaderResourceView* getResourceView();

	protected:
		void release();

		virtual bool create();

		// data
		ID3D11Resource* m_resource = nullptr;
		ID3D11ShaderResourceView* m_shader_resource_view = nullptr;
	};
}

#endif