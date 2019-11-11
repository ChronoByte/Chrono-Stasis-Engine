#include "JSONManager.h"
#include "csGlobals.h"


JSONManager::JSONManager()
{
}

JSONManager::~JSONManager()
{
}

JSON_File* JSONManager::JSONRead(const char* path)
{
	FILE* fp = fopen(path, "rb");
	char readBuffer[65536];

	if (fp != nullptr)
		return new JSON_File(new rapidjson::FileReadStream(fp, readBuffer, sizeof(readBuffer)), fp);
	else
		return nullptr;
}

JSON_File* JSONManager::JSONWrite(const char* path)
{
	FILE* fp = fopen(path, "wb");
	char writeBuffer[65536];

	if (fp != nullptr)
		return new JSON_File(new rapidjson::FileWriteStream(fp, writeBuffer, sizeof(writeBuffer)), fp);
	else
		return nullptr;
}

void JSONManager::JSONClose(JSON_File* file)
{
	if (file != nullptr)
		file->CloseFile();

	RELEASE(file);
	file = nullptr;
}

JSON_File::JSON_File(rapidjson::FileWriteStream* os, FILE* fp) : os(os), fp(fp)
{
	document = new rapidjson::Document();
	document->SetObject();

	allocator = &document->GetAllocator();
}

JSON_File::JSON_File(rapidjson::FileReadStream* is, FILE* fp) : is(is), fp(fp)
{
	document = new rapidjson::Document();
	document->ParseStream(*is);

	allocator = &document->GetAllocator();
}

JSON_File::~JSON_File()
{
	RELEASE(document);
	RELEASE(allocator);
	RELEASE(is);
	RELEASE(os);
}

void JSON_File::CloseFile()
{
	fclose(fp);
}