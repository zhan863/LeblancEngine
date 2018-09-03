#include "LeblancEngine/Render/Material/D3D11/LeblancD3D11Include.h"

using namespace std;

namespace Leblanc
{
	HRESULT __stdcall IncludeD3D11::Open(
		D3D_INCLUDE_TYPE include_type,
		LPCSTR p_file_name,
		LPCVOID p_parent_data,
		LPCVOID *pp_data,
		UINT *p_bytes)
	{
		string final_path;
		switch (include_type)
		{
		case D3D_INCLUDE_LOCAL: // #include "FILE"
			final_path = m_shader_dir + p_file_name;
			break;
		case D3D_INCLUDE_SYSTEM: // #include <FILE>
			final_path = m_system_dir + p_file_name;
			break;
		default:
			assert(0);
		}

		ios::openmode mode = ios::in | ios::binary;
		fstream* rw_stream = new fstream();
		rw_stream->open(final_path.c_str(), mode);
		// Should check ensure open succeeded, in case fail for some reason.
		if (rw_stream->fail())
		{
			//LOG("Cannot open file: " << final_path);
		}
		else
		{
			rw_stream->seekg(0, ios::end);
			size_t length = (size_t)(rw_stream->tellg());

			if (length)
			{
				rw_stream->seekg(0, ios::beg);

				char* buf = new char[length];
				rw_stream->read(buf, length);
				*pp_data = buf;
				*p_bytes = length;
			}
			else
			{
				*pp_data = nullptr;
				*p_bytes = 0;
			}
			rw_stream->close();
		}
		delete rw_stream;
		rw_stream = nullptr;

		return S_OK;
	}

	HRESULT __stdcall IncludeD3D11::Close(LPCVOID pData)
	{
		// Here we must correctly free buffer created in Open.
		char* buf = (char*)pData;
		delete[] buf;
		return S_OK;
	}

}