#ifndef LEBLANC_D3D11_INCLUDE_H
#define LEBLANC_D3D11_INCLUDE_H
#include "LeblancEngine/Render/Include/LeblancDirectInclude.h"

using namespace std;

namespace Leblanc
{
	class IncludeD3D11 : public ID3DInclude
	{
	public:
		// shaderDir: Directory of current shader file, used by #include "FILE"
		// systemDir: Default shader includes directory, used by #include <FILE>
		IncludeD3D11(const char* shader_dir, const char* system_dir) :
			m_shader_dir(shader_dir),
			m_system_dir(system_dir)
		{
		}

		HRESULT __stdcall IncludeD3D11::Open(
			D3D_INCLUDE_TYPE include_type,
			LPCSTR p_file_name,
			LPCVOID p_parent_data,
			LPCVOID *pp_data,
			UINT *p_bytes);

		HRESULT __stdcall Close(LPCVOID pData);

	private:
		string m_shader_dir;
		string m_system_dir;
	};
}
#endif