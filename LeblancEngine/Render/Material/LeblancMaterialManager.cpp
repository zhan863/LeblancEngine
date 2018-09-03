#include "LeblancEngine/Render/Material/LeblancMaterialManager.h"
#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"
#include "LeblancEngine/Render/Material/LeblancShaders.h"
#include "LeblancEngine/Global/LeblancGlobalContext.h"
#include "LeblancEngine/Render/Device/LeblancDevice.h"

#include <filesystem>
namespace fs = std::experimental::filesystem;

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

		if (!fs::exists(path) || !fs::is_directory(path)) return;

		fs::recursive_directory_iterator it(path);
		fs::recursive_directory_iterator endit;

		vector<wstring> files;
		while (it != endit)
		{
			if (fs::is_regular_file(*it) && it->path().extension() == ".fx") 
				files.push_back(it->path().c_str());
				++it;
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