#pragma once

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/filereadstream.h"

struct JSON_File
{
public:
	JSON_File(rapidjson::FileWriteStream* os, FILE* fp);
	JSON_File(rapidjson::FileReadStream* is, FILE* fp);
	
	virtual ~JSON_File();
	void CloseFile();

private:

	FILE* fp = nullptr;
	rapidjson::Document* document = nullptr;
	rapidjson::FileWriteStream* os = nullptr;
	rapidjson::FileReadStream* is = nullptr;
	rapidjson::Document::AllocatorType* allocator = nullptr;
};

class JSONManager
{
public:
	JSONManager();
	virtual ~JSONManager();

	JSON_File* JSONRead(const char* path);
	JSON_File* JSONWrite(const char* path);
	void JSONClose(JSON_File* file);
};
