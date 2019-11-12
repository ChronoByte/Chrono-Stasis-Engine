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
	STRING_VALUE,
	NUMBER_VALUE,
	FALSE_VALUE,
	TRUE_VALUE,
};

struct RJSON_Value {
public:
	RJSON_Value(rapidjson::Document::AllocatorType* allocator, rapidjson::Type type = rapidjson::kObjectType);
	virtual ~RJSON_Value();

	void TransformTo(rapidjson::Type type);

	//RJSON_Value* CreateValue(JSONValueType type);
	RJSON_Value* CreateValue(rapidjson::Type type = rapidjson::kObjectType);
	void AddValue(const char* name, RJSON_Value* value);
	RJSON_Value* GetValue(const char* name);
	void SetValue(const char* name, RJSON_Value* value);
	rapidjson::Value* GetJSONValue();

private:
	rapidjson::Document::AllocatorType* allocator = nullptr;
	rapidjson::Value* value = nullptr;
	std::vector<RJSON_Value*> values;
};

/*
struct RJSON_Object : public RJSON_Value
{
public:

	RJSON_Object(rapidjson::Document::AllocatorType* allocator);
	virtual ~RJSON_Object();

	void AddObject(const char* name, RJSON_Object* value);
	RJSON_Object* CreateObject();
	RJSON_Object* GetObj(const char* name);
	void SetObject(const char* name, RJSON_Object* value);
	rapidjson::Value* GetObjectValue();

private:

	rapidjson::Value* value = nullptr;
	rapidjson::Document::AllocatorType* allocator = nullptr;
	std::vector<RJSON_Object*> values;
};

struct RJSON_Array : public RJSON_Value
{
public:
	RJSON_Array(rapidjson::Document::AllocatorType* allocator);
	virtual ~RJSON_Array();

	void AddArray(const char* name, RJSON_Array* value);
	RJSON_Array* CreateArray();
	RJSON_Array* GetArray(const char* name);
	void SetArray(const char* name, RJSON_Array* value);
	rapidjson::Value* GetArrayValue();

private:
	rapidjson::Value* value = nullptr;
	rapidjson::Document::AllocatorType* allocator = nullptr;
	std::vector<RJSON_Array*> values;
};
*/  

struct RJSON_File
{
public:
	RJSON_File(rapidjson::FileWriteStream* os, FILE* fp);
	RJSON_File(rapidjson::FileReadStream* is, FILE* fp);

	virtual ~RJSON_File();

	bool WriteFile();

	RJSON_Value* CreateValue(rapidjson::Type type = rapidjson::kObjectType);
	void AddValue(const char* name, RJSON_Value* value);
	RJSON_Value* GetValue(const char* name);
	void SetValue(const char* name, RJSON_Value* value);
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
	Serializer();
	virtual ~Serializer();

	RJSON_File* JSONRead(const char* path);
	RJSON_File* JSONWrite(const char* path);
	void JSONClose(RJSON_File* file);
};
