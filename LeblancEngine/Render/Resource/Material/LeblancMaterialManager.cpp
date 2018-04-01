#include "LeblancEngine/Render/Resource/Material/LeblancMaterialManager.h"
#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"
#include "LeblancEngine/Render/Resource/Material/LeblancShaders.h"
#include "LeblancEngine/Global/LeblancGlobalContext.h"

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
		Shader* new_shader = new Shader(g_global_context.m_device_manager.getCurrentDevice());
		new_shader->initialize(string(files[i].begin(), files[i].end()), "");
		m_shaders.push_back(new_shader);
	}
}

void MaterialManager::release()
{
	for (int i = 0; i < m_shaders.size(); i++)
		safe_release(m_shaders[i]);
	m_shaders.clear();
}