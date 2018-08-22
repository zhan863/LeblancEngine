#ifndef LEBLANC_MATERIAL_H
#define LEBLANC_MATERIAL_H
#include "LeblancEngine/Render/Material/LeblancMaterial.h"
#include "LeblancEngine/Render/Material/LeblancShaders.h"
#include <string>
#include <map>

using namespace std;

namespace Leblanc
{
	class IShader;
	class ITechnique;
	class GpuData;
	class Material
	{
	public:
		Material();
		~Material();

		void release();

		void initialize(string file_name);

		void setParameters() const;

		IShader* getShader() { return m_shader; }

		string getTechniqueName() const { return m_technique_name; }

	private:
		GpuData * getGpuDataFromString(string type_name, string value);

		// Reference
		IShader * m_shader = nullptr;
		string m_technique_name;
		string m_pass_name;

		// Data
		map<string, GpuData*> m_parameters;
		map<string, string> m_textures;
	};
}
#endif