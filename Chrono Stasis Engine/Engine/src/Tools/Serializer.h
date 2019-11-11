#pragma once

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/prettywriter.h"
#include <vector>

enum JSONValueType {
	NONE_VALUE = 0,

	OBJECT_VALUE,
	ARRAY_VALUE,
	BOOLEAN_VALUE,
	NUMBER_VALUE,
};

struct RAPIDJSON_Array
{
public:
	RAPIDJSON_Array(rapidjson::Document::AllocatorType* allocator);
	virtual ~RAPIDJSON_Array();

	void AddArray(const char* name, RAPIDJSON_Array* value);
	RAPIDJSON_Array* CreateArray();
	RAPIDJSON_Array* GetArray(const char* name);
	void SetArray(const char* name, RAPIDJSON_Array* value);
	rapidjson::Value* GetArrayValue();

private:
	rapidjson::Value* value = nullptr;
	rapidjson::Document::AllocatorType* allocator = nullptr;
	std::vector<RAPIDJSON_Array*> values;
};

struct RAPIDJSON_Object
{
public:

	RAPIDJSON_Object(rapidjson::Document::AllocatorType* allocator);
	virtual ~RAPIDJSON_Object();

	//void ToArray();
	void AddObject(const char* name, RAPIDJSON_Object* value);
	RAPIDJSON_Object* CreateObject();
	RAPIDJSON_Object* GetObj(const char* name);
	void SetObject(const char* name, RAPIDJSON_Object* value);
	rapidjson::Value* GetObjectValue();

private:

	rapidjson::Value* value = nullptr;
	rapidjson::Document::AllocatorType* allocator = nullptr;
	std::vector<RAPIDJSON_Object*> values;
};

struct RAPIDJSON_FILE
{
public:
	RAPIDJSON_FILE(rapidjson::FileWriteStream* os, FILE* fp);
	RAPIDJSON_FILE(rapidjson::FileReadStream* is, FILE* fp);
	
	virtual ~RAPIDJSON_FILE();

	bool WriteFile();
	RAPIDJSON_Object* PushValue();
	void AddValue(const char* name, RAPIDJSON_Object* value);
	RAPIDJSON_Object* GetValue(const char* name);
	void SetValue(const char* name, RAPIDJSON_Object* value);
	void CloseFile();

private:

	FILE* fp = nullptr;
	rapidjson::Document* document = nullptr;
	rapidjson::FileWriteStream* os = nullptr;
	rapidjson::FileReadStream* is = nullptr;
	rapidjson::Document::AllocatorType* allocator = nullptr;
	std::vector<RAPIDJSON_Object*> values;
};

class Serializer
{
public:
	Serializer();
	virtual ~Serializer();

	//void CreateValue(JSONValueType value);

	RAPIDJSON_FILE* JSONRead(const char* path);
	RAPIDJSON_FILE* JSONWrite(const char* path);
	void JSONClose(RAPIDJSON_FILE* file);
};
