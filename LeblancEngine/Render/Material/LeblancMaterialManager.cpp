#include "LeblancEngine/Render/Material/LeblancMaterialManager.h"
#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"
#include "LeblancEngine/Render/Material/LeblancShaders.h"
#include "LeblancEngine/Global/LeblancGlobalContext.h"
#include "LeblancEngine/Render/Device/LeblancDevice.h"

namespace Leblanc
{
	MaterialManager::MaterialManager()
	{

	}

	MaterialManager::~MaterialManager()
	{
		release();
	}

	void MaterialManager::initialize()
	{
		WIN32_FIND_DATA shader_file;
		wstring path(L"Content\\Shader\\");
		wstring partern(L"*.fx");
		HANDLE h_find = FindFirstFileEx((path + L"\\" + partern).c_str(),
			FindExInfoStandard,
			&shader_file,
			FindExSearchNameMatch,
			NULL,
			0
		);

		if (h_find == INVALID_HANDLE_VALUE)
		{
			DWORD error = GetLastError();

			if (error != ERROR_FILE_NOT_FOUND)
			{
				//LOG("Invalid File Handle, err code: " << error);
				return;
			}
		}

		vector<wstring> files;
		if (h_find != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (shader_file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					continue;

				files.push_back(path + shader_file.cFileName);

			} while (FindNextFile(h_find, &shader_file));
			FindClose(h_find);
		}

		for (int i = 0; i < files.size(); i++)
		{
			IDevice* device = g_global_context.m_device_manager.getDevice();
			IShader* new_shader = device->createShader(string(files[i].begin(), files[i].end()));

			if (new_shader)
				m_shaders.insert(make_pair(string(files[i].begin(), files[i].end()), new_shader));
		}
	}

	void MaterialManager::release()
	{
		for (map<string, IShader*>::iterator i = m_shaders.begin(); i != m_shaders.end(); i++)
		{
			safe_delete(i->second);
		}
		m_shaders.clear();
	}

	IShader* MaterialManager::getShaderByFilePath(string filename)
	{
		map<string, IShader*>::iterator i = m_shaders.find(filename);
		if (i != m_shaders.end())
		{
			return i->second;
		}

		return nullptr;
	}
}