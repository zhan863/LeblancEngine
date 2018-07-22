#ifndef LEBLANC_ASSET_MANAGER_H
#define LEBLANC_ASSET_MANAGER_H

#include "ThirdParty/pugi/pugixml.hpp"

namespace Leblanc
{
	class DataStream;
	class AssetManager
	{
	public:
		AssetManager();
		~AssetManager();

		static AssetManager* assetManager();
		static bool			 fileExists(const std::string& filename);
		DataStream*			 openStream(const std::string& streamPathName);
		pugi::xml_document*  openXmlDocument(const std::string& resourcePathName);
	};
}
#endif