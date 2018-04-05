#include "LeblancEngine/Render/Resource/Material/LeblancMaterial.h"
#include "LeblancEngine/Global/LeblancGlobalContext.h"
#include "LeblancEngine/Render/Resource/Material/LeblancGpuVariable.h"
#include "LeblancEngine/BasicInclude/LeblancMemoryOperation.h"
#include "LeblancEngine/Math/LeblancMath.h"

#include <sstream>

const static string shader_folder = "Content\\Shader\\";
const static int k_float4_size = 16;

Material::Material()
{

}

Material::~Material()
{
	release();
}

void Material::release()
{
	for each(auto iter in m_parameters)
	{
		safe_delete(iter.second);
	}

	m_parameters.clear();
	m_textures.clear();
}

GpuData* Material::getGpuDataFromString(string type_name, string value)
{
	if (type_name == "Float4")
	{
		stringstream ss(value);
		Vector4f float4;
		ss >> float4.x >> float4.y >> float4.z >> float4.w;

		return new GpuData(k_float4_size, &float4);
	}

	return nullptr;
}

void Material::initialize(string file_name)
{
	if (std::unique_ptr<typename pugi::xml_document> doc =
		std::unique_ptr<typename pugi::xml_document>(g_global_context.m_asset_manager.openXmlDocument(file_name)))
	{
		//LOG("Loaded manifest file " << file_name);

		pugi::xpath_node shader = doc->select_node("/Material/Shader");
		string shader_file_path = shader_folder + shader.node().attribute("name").value();	
		m_shader = g_global_context.m_material_manager.getShaderByFilePath(shader_file_path);

		pugi::xpath_node technique = doc->select_node("/Material/Technique");
		m_technique_name = technique.node().attribute("name").value();

		pugi::xpath_node pass = doc->select_node("/Material/Pass");
		m_pass_name = pass.node().attribute("name").value();

		pugi::xpath_node_set parameters = doc->select_nodes("/Material/Parameters/Parameter");

		for (auto parameter = parameters.begin(); parameter != parameters.end(); ++parameter)
		{
			auto xpath_node = *parameter;
			if (xpath_node)
			{
				if (const char* parameter_type_cstr = xpath_node.node().attribute("type").value())
				{
					string parameter_type = parameter_type_cstr;
					string parameter_name = xpath_node.node().attribute("name").value();

					if (parameter_type != "Texture")
					{
						GpuData* gpu_data = getGpuDataFromString(parameter_type, xpath_node.node().attribute("value").as_string());
						if (gpu_data)
							m_parameters.insert(make_pair(parameter_name, gpu_data));
					}
					else
					{
						string texture_name = xpath_node.node().attribute("value").value();
						m_textures.insert(make_pair(parameter_name, texture_name));
					}
				}
			}
		}
	}
	else
	{
		//THROW("Failed to load material " << file_name);
		return;
	}
}