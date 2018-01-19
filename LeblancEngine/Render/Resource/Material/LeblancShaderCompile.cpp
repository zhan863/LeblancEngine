#include "LeblancEngine/Render/Resource/Material/LeblancShaderCompile.h"
#include "LeblancEngine/BasicInclude/LeblancPCH.h"
#include "LeblancEngine/Global/LeblancGlobalContext.h"

namespace ShaderCompiler
{
	ID3D10Blob* compileShader(LPCWSTR path,
		LPCSTR function_name,
		LPCSTR profile,
		CONST D3D10_SHADER_MACRO* defines,
		ID3D10Include* includes)
	{
		// Loop until we succeed, or an exception is thrown
		while (true)
		{

			UINT flags = 0;
#ifdef _DEBUG
			flags |= D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

			ID3D10Blob* compiledShader;
			ID3D10Blob* errorMessages;
			HRESULT hr = D3DX11CompileFromFileW(path, defines, includes, function_name, profile,
				flags, 0, NULL, &compiledShader, &errorMessages, NULL);

			if (FAILED(hr))
			{
				if (errorMessages)
				{
					WCHAR message[1024];
					message[0] = NULL;
					char* blobdata = reinterpret_cast<char*>(errorMessages->GetBufferPointer());

					MultiByteToWideChar(CP_ACP, 0, blobdata, static_cast<int>(errorMessages->GetBufferSize()), message, 1024);
					std::wstring fullMessage = L"Error compiling shader file \"";
					fullMessage += path;
					fullMessage += L"\" - ";
					fullMessage += message;

#ifdef _DEBUG
					// Pop up a message box allowing user to retry compilation
					int retVal = MessageBoxW(NULL, fullMessage.c_str(), L"Shader Compilation Error", MB_RETRYCANCEL);
					if (retVal != IDRETRY)
						throw DXException(hr, fullMessage.c_str());
#else
					throw DXException(hr, fullMessage.c_str());
#endif                                                
				}
				else
				{
					_ASSERT(false);
					throw DXException(hr);
				}
			}
			else
				return compiledShader;
		}
	}

	ID3D11VertexShader* compileVSFromFile(ID3D11Device* device,
		LPCWSTR path,
		LPCSTR function_name,
		LPCSTR profile,
		CONST D3D10_SHADER_MACRO* defines,
		ID3D10Include* includes,
		ID3D10Blob** byteCode)
	{
		ID3D10Blob* compiledShader = compileShader(path, function_name, profile, defines, includes);
		ID3D11VertexShader* shader = NULL;
		device->CreateVertexShader(compiledShader->GetBufferPointer(),
			compiledShader->GetBufferSize(),
			NULL,
			&shader);

		return shader;
	}

	ID3D11PixelShader* compilePSFromFile(ID3D11Device* device,
		LPCWSTR path,
		LPCSTR function_name,
		LPCSTR profile,
		CONST D3D10_SHADER_MACRO* defines,
		ID3D10Include* includes)
	{
		ID3D10Blob* compiledShader = compileShader(path, function_name, profile, defines, includes);
		ID3D11PixelShader* shader = NULL;
		device->CreatePixelShader(compiledShader->GetBufferPointer(),
			compiledShader->GetBufferSize(),
			NULL,
			&shader);

		return shader;
	}

	ID3D11GeometryShader* compileGSFromFile(ID3D11Device* device,
		LPCWSTR path,
		LPCSTR function_name,
		LPCSTR profile,
		CONST D3D10_SHADER_MACRO* defines,
		ID3D10Include* includes)
	{
		ID3D10Blob* compiledShader = compileShader(path, function_name, profile, defines, includes);
		ID3D11GeometryShader* shader = NULL;
		device->CreateGeometryShader(compiledShader->GetBufferPointer(),
			compiledShader->GetBufferSize(),
			NULL,
			&shader);

		return shader;
	}

	ID3D11HullShader* compileHSFromFile(ID3D11Device* device,
		LPCWSTR path,
		LPCSTR function_name,
		LPCSTR profile,
		CONST D3D10_SHADER_MACRO* defines,
		ID3D10Include* includes)
	{
		ID3D10Blob* compiledShader = compileShader(path, function_name, profile, defines, includes);
		ID3D11HullShader* shader = NULL;
		device->CreateHullShader(compiledShader->GetBufferPointer(),
			compiledShader->GetBufferSize(),
			NULL,
			&shader);

		return shader;
	}

	ID3D11DomainShader* compileDSFromFile(ID3D11Device* device,
		LPCWSTR path,
		LPCSTR function_name,
		LPCSTR profile,
		CONST D3D10_SHADER_MACRO* defines,
		ID3D10Include* includes)
	{
		ID3D10Blob* compiledShader = compileShader(path, function_name, profile, defines, includes);
		ID3D11DomainShader* shader = NULL;
		device->CreateDomainShader(compiledShader->GetBufferPointer(),
			compiledShader->GetBufferSize(),
			NULL,
			&shader);

		return shader;
	}

	ID3D11ComputeShader* compileCSFromFile(ID3D11Device* device,
		LPCWSTR path,
		LPCSTR function_name,
		LPCSTR profile,
		CONST D3D10_SHADER_MACRO* defines,
		ID3D10Include* includes)
	{
		ID3D10Blob* compiledShader = compileShader(path, function_name, profile, defines, includes);
		ID3D11ComputeShader* shader = NULL;
		device->CreateComputeShader(compiledShader->GetBufferPointer(),
			compiledShader->GetBufferSize(),
			NULL,
			&shader);

		return shader;
	}

	void compileMaterial(LPCWSTR material_file_name,
		LPCSTR vs,
		LPCSTR ps,
		Material& material)
	{
		material.m_vertex_shader.setShader(compileVSFromFile(g_global_context.m_device_manager.getCurrentDevice().getD3D11Device(), material_file_name, vs, "vs_4_0"));
		material.m_pixel_shader.setShader(compilePSFromFile(g_global_context.m_device_manager.getCurrentDevice().getD3D11Device(), material_file_name, ps, "ps_4_0"));
	}
}
