#ifndef LEBLANC_SHADER_COMPILE_H
#define LEBLANC_SHADER_COMPILE_H
#include "LeblancEngine/BasicInclude/LeblancPCH.h"

// Compiles a shader from file and returns the compiled bytecode
ID3D10Blob* compileShader(LPCWSTR path,
	LPCSTR function_name,
	LPCSTR profile,
	CONST D3D10_SHADER_MACRO* defines = NULL,
	ID3D10Include* includes = NULL);

// Compiles a shader from file and creates the appropriate shader instance
ID3D11VertexShader* compileVSFromFile(ID3D11Device* device,
	LPCWSTR path,
	LPCSTR function_name = "VS",
	LPCSTR profile = "vs_4_0",
	CONST D3D10_SHADER_MACRO* defines = NULL,
	ID3D10Include* includes = NULL,
	ID3D10Blob** byteCode = NULL);

ID3D11PixelShader* compilePSFromFile(ID3D11Device* device,
	LPCWSTR path,
	LPCSTR function_name = "PS",
	LPCSTR profile = "ps_4_0",
	CONST D3D10_SHADER_MACRO* defines = NULL,
	ID3D10Include* includes = NULL);

ID3D11GeometryShader* compileGSFromFile(ID3D11Device* device,
	LPCWSTR path,
	LPCSTR function_name = "GS",
	LPCSTR profile = "gs_4_0",
	CONST D3D10_SHADER_MACRO* defines = NULL,
	ID3D10Include* includes = NULL);

ID3D11HullShader* compileHSFromFile(ID3D11Device* device,
	LPCWSTR path,
	LPCSTR function_name = "HS",
	LPCSTR profile = "hs_5_0",
	CONST D3D10_SHADER_MACRO* defines = NULL,
	ID3D10Include* includes = NULL);

ID3D11DomainShader* compileDSFromFile(ID3D11Device* device,
	LPCWSTR path,
	LPCSTR function_name = "DS",
	LPCSTR profile = "ds_5_0",
	CONST D3D10_SHADER_MACRO* defines = NULL,
	ID3D10Include* includes = NULL);

ID3D11ComputeShader* compileCSFromFile(ID3D11Device* device,
	LPCWSTR path,
	LPCSTR function_name = "CS",
	LPCSTR profile = "cs_4_0",
	CONST D3D10_SHADER_MACRO* defines = NULL,
	ID3D10Include* includes = NULL);
#endif