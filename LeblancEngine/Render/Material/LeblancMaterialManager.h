#ifndef LEBLANC_MATERIAL_MANAGER_H
#define LEBLANC_MATERIAL_MANAGER_H

#include <map>
#include <string>

using namespace std;

namespace Leblanc
{
	class IShader;
	class MaterialManager
	{
	public:
		MaterialManager();
		~MaterialManager();

		void initialize();

		void release();

		IShader* getShaderByFilePath(string filename);

	private:
		// data
		map<string, IShader*> m_shaders;
	};
}
#endif