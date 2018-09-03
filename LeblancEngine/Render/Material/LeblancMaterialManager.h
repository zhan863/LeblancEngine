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

		IShader* getShaderByFilePath(string filename);

	private:
		void release();

		// data
		map<string, IShader*> m_shaders;
	};
}
#endif