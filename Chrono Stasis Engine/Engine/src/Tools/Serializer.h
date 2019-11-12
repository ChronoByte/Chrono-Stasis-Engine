#pragma once

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/prettywriter.h"
#include <vector>


struct RJSON_Value {

public:
	RJSON_Value(rapidjson::Document::AllocatorType* allocator, rapidjson::Type type = rapidjson::kObjectType);
	virtual ~RJSON_Value();

	void TransformTo(rapidjson::Type type);

	RJSON_Value* CreateValue(rapidjson::Type type = rapidjson::kObjectType);

	void AddValue(const char* name,const RJSON_Value& value);
	RJSON_Value* GetValue(const char* name);
	void SetValue(const char* name, RJSON_Value& value);

	rapidjson::Value* GetJSONValue() const;

private:
	rapidjson::Document::AllocatorType* allocator = nullptr;
	rapidjson::Value* value = nullptr;
	std::vector<RJSON_Value*> values;
};


struct RJSON_File
{
public:
	RJSON_File(rapidjson::FileWriteStream* os, FILE* fp);
	RJSON_File(rapidjson::FileReadStream* is, FILE* fp);

	virtual ~RJSON_File();

	bool WriteFile();

	RJSON_Value* CreateValue(rapidjson::Type type = rapidjson::kObjectType);

	void AddValue(const char* name, const RJSON_Value& value);
	RJSON_Value* GetValue(const char* name);
	void SetValue(const char* name, RJSON_Value& value);

	void CloseFile();


private:

	FILE* fp = nullptr;
	rapidjson::Document* document = nullptr;
	rapidjson::FileWriteStream* os = nullptr;
	rapidjson::FileReadStream* is = nullptr;
	rapidjson::Document::AllocatorType* allocator = nullptr;
	std::vector<RJSON_Value*> values;
};


class Serializer
{
public:
	Serializer(){}
	virtual ~Serializer(){}

	RJSON_File* JSONRead(const char* path);
	RJSON_File* JSONWrite(const char* path);
	void JSONClose(RJSON_File* file);
};
