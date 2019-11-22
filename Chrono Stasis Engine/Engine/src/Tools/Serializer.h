#pragma once

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/prettywriter.h"
#include <vector>
#include "csGlobals.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/Quat.h"
#include "MathGeoLib/include/Math/float2.h"
#include "MathGeoLib/include/Math/float4.h"

#include "JSON/parson.h"

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

	void SetInt(const char* name, int value);
	void SetUint(const char* name, uint value);
	void SetFloat(const char* name, float value);
	void SetString(const char* name, const char* value);
	void SetBoolean(const char* name, bool value);
	void SetVector3(const char* name, float3 vec);
	void SetQuaternion(const char* name, Quat quat);

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

	JSON_Status json_array_dotset_float3(JSON_Object* object, const char* name, float3 transform);
	JSON_Status json_array_dotset_float2(JSON_Object* object, const char* name, float2 transform);
	JSON_Status json_array_dotset_float4(JSON_Object* object, const char* name, float4 transform);

	RJSON_File* JSONRead(const char* path);
	RJSON_File* JSONWrite(const char* path);
	void JSONClose(RJSON_File* file);
};
