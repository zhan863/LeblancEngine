#include "LeblancEngine/Asset/LeblancAssetManager.h"
#include "LeblancEngine/Asset/LeblancDataStream.h"
#include <fstream>

AssetManager::AssetManager()
{

}

AssetManager::~AssetManager()
{

}

bool AssetManager::fileExists(const std::string& filename)
{
	struct stat finfo;
	int statResult = stat(filename.c_str(), &finfo);
	if (statResult == 0)
		return true;
	else
		return false;
}

DataStream*	AssetManager::openStream(const std::string& filename)
{
	DataStream* stream = nullptr;
	if (AssetManager::fileExists(filename))
	{
		std::ios::openmode mode = std::ios::in | std::ios::binary;
		std::fstream* rwStream = new std::fstream();
		rwStream->open(filename.c_str(), mode);
		// Should check ensure open succeeded, in case fail for some reason.
		if (rwStream->fail())
		{
			//LOG("Cannot open file: " << filename);
		}
		else
		{
			rwStream->seekg(0, std::ios::end);
			size_t length = (size_t)(rwStream->tellg());
			rwStream->seekg(0, std::ios::beg);

			uint8_t* buffer = (uint8_t*)malloc(sizeof(uint8_t)*length);
			rwStream->read((char*)buffer, length);

			stream = new MemoryDataStream(filename, buffer, length, true);
			rwStream->close();
		}
		delete rwStream;
		rwStream = nullptr;
	}

	if (!stream)
	{
		//LOG("ERROR could not open stream " << filename);
	}

	return stream;
}

pugi::xml_document* AssetManager::openXmlDocument(const std::string& filename)
{
	pugi::xml_document * xmlDocument = nullptr;

	std::unique_ptr<typename DataStream> stream =
		std::unique_ptr<typename DataStream>(openStream(filename));
	if (stream)
	{
		xmlDocument = new pugi::xml_document();
		// Create intermediate buffer;
		char * buffer = (char*)malloc(sizeof(char) * stream->size() + 1);
		memset(buffer, 0, stream->size() + 1);
		if (buffer)
		{
			stream->readBytes(buffer, stream->size());
			if (xmlDocument->load(buffer) == false)
			{
				//LOG("Failed to load xml document from stream: " << filename)
				delete xmlDocument;
				xmlDocument = nullptr;
			}
			free(buffer);
		}
	}

	return xmlDocument;
}