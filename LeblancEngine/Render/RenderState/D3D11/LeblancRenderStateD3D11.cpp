#include "LeblancEngine/Render/Device/LeblancDevice.h"
#include "LeblancEngine/Render/Device/LeblancDeviceContext.h"
#include "LeblancEngine/Render/Device/D3D11/LeblancDeviceD3D11.h"
#include "LeblancEngine/Render/Device/D3D11/LeblancDeviceContextD3D11.h"
#include "LeblancEngine/Render/RenderState/D3D11/LeblancRenderStateD3D11.h"
#include "LeblancEngine/Render/Buffer/D3D11/LeblancBufferD3D11.h"
#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"
#include "LeblancEngine/Render/Include/LeblancDirectInclude.h"

namespace Leblanc
{
	void RasterizerStateD3D11::release()
	{
		safe_Release(m_rasterizer_state);
	}

	void RasterizerStateD3D11::initialize(const RasterizerState* rasterizer_state)
	{
		release();

		D3D11_RASTERIZER_DESC rasterizer_desc;
		rasterizer_desc.CullMode = *rasterizer_state == RasterizerState::NONE ? D3D11_CULL_NONE :
			(*rasterizer_state == RasterizerState::CW_FRONT ? D3D11_CULL_FRONT : D3D11_CULL_BACK);
		rasterizer_desc.FillMode = D3D11_FILL_SOLID;
		rasterizer_desc.ScissorEnable = false;
		rasterizer_desc.DepthBias = false;
		rasterizer_desc.DepthBiasClamp = 0.0f;
		rasterizer_desc.SlopeScaledDepthBias = 0;
		rasterizer_desc.DepthClipEnable = true;

		DeviceD3D11* d3d11_device = dynamic_cast<DeviceD3D11*>(m_device);
		if (d3d11_device)
			d3d11_device->createRasterizerState(&rasterizer_desc, &m_rasterizer_state);
	}

	void DepthStencilStateD3D11::release()
	{
		safe_Release(m_depth_stencil_state);
	}

	void DepthStencilStateD3D11::initialize(const DepthStencilState* depth_stencil_state)
	{
		release();

		D3D11_DEPTH_STENCIL_DESC depth_desc;
		depth_desc.DepthEnable = false;
		depth_desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		depth_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depth_desc.StencilEnable = false;

		DeviceD3D11* d3d11_device = dynamic_cast<DeviceD3D11*>(m_device);
		if (d3d11_device)
			d3d11_device->createDepthStencilState(&depth_desc, &m_depth_stencil_state);
	}

	void BlendStateD3D11::release()
	{
		safe_Release(m_blend_state);
	}

	void BlendStateD3D11::initialize(const BlendState* blend_mode)
	{
		release();

		D3D11_BLEND_DESC blend_desc;
		blend_desc.AlphaToCoverageEnable = false;
		blend_desc.IndependentBlendEnable = false;

		for (int i = 0; i < 8; i++)
		{
			blend_desc.RenderTarget[i].BlendEnable = false;
			blend_desc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
			blend_desc.RenderTarget[i].SrcBlend = D3D11_BLEND_ONE;
			blend_desc.RenderTarget[i].DestBlend = D3D11_BLEND_ONE;
			blend_desc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blend_desc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			blend_desc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
			blend_desc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ONE;
		}

		DeviceD3D11* d3d11_device = dynamic_cast<DeviceD3D11*>(m_device);
		if (d3d11_device)
			d3d11_device->createBlendState(&blend_desc, &m_blend_state);
	}

	InputLayoutCacheD3D11::InputLayoutCacheD3D11(IDevice* device, IDeviceContext* device_context) :
		IInputLayoutCache(device, device_context)
	{
	}

	InputLayoutCacheD3D11::~InputLayoutCacheD3D11()
	{
		release();
	}

	void InputLayoutCacheD3D11::release()
	{
		for (auto it = m_layouts.begin(); it != m_layouts.end(); it++)
		{
			ID3D11InputLayout* layout = it->second;
			safe_Release(layout);
		}

		m_layouts.clear();
	}

	void InputLayoutCacheD3D11::initialize(ID3DX11EffectTechnique* technique, unsigned int pass_index)
	{
		m_technique = technique;
		m_pass_index = pass_index;

		if (ID3DX11EffectPass* pass = m_technique->GetPassByIndex(pass_index))
		{
			pass->GetDesc(&m_pass_desc);
		}
	}

	bool InputLayoutCacheD3D11::bindLayout(const VertexDeclarationD3D11* vertex_declaration) const
	{
		if (!vertex_declaration)
		{
			//LOG("vertex declaration null");
			return false;
		}

		auto it = m_layouts.find(vertex_declaration);

		if (it == m_layouts.end())
		{
			ID3D11InputLayout* inputLayout = 0;
			unsigned int elementCount = vertex_declaration->getElementCount();
			const D3D11_INPUT_ELEMENT_DESC* input_desc = vertex_declaration->getInputLayout();

			ID3D11Device* device = (ID3D11Device*)m_device->getHandle();
			if (device)
			{
				if (FAILED(device->CreateInputLayout(input_desc,
					elementCount,
					m_pass_desc.pIAInputSignature,
					m_pass_desc.IAInputSignatureSize,
					&inputLayout)))
				{
					//LOG("Failed to setup input layout");
					return false;
				}
			}
			else
			{
				//LOG("Failed to find device");
				return false;
			}

			m_layouts.insert(std::make_pair(vertex_declaration, inputLayout));
			it = m_layouts.find(vertex_declaration);
		}

		if (ID3D11InputLayout* layout = it->second)
		{
			ID3D11DeviceContext* device_context = (ID3D11DeviceContext*)m_device_context->getHandle();
			if (device_context)
				device_context->IASetInputLayout(layout);
			return true;
		}
		return false;
	}
}