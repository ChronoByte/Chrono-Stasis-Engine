#include "Serializer.h"
#include "csGlobals.h"


Serializer::Serializer()
{
}

Serializer::~Serializer()
{
}

//void Serializer::CreateJSONType(JSONType type)  //TODO: make JSON parent to optimize class methods from JSON types (overwrite) & develop switch
//{
//	switch (type) {
//	case JSONType::OBJECT:
//
//
//	}
//}

RAPIDJSON_FILE* Serializer::JSONRead(const char* path)
{
	FILE* fp = fopen(path, "rb");
	char readBuffer[65536];

	if (fp != nullptr)
		return new RAPIDJSON_FILE(new rapidjson::FileReadStream(fp, readBuffer, sizeof(readBuffer)), fp);
	else
		return nullptr;
}

RAPIDJSON_FILE* Serializer::JSONWrite(const char* path)
{
	FILE* fp = fopen(path, "wb");
	char writeBuffer[65536];

	if (fp != nullptr)
		return new RAPIDJSON_FILE(new rapidjson::FileWriteStream(fp, writeBuffer, sizeof(writeBuffer)), fp);
	else
		return nullptr;
}

void Serializer::JSONClose(RAPIDJSON_FILE* file)
{
	if (file != nullptr)
		file->CloseFile();

	RELEASE(file);
	file = nullptr;
}

// ---------------- JSON FILE  ----------------- //
RAPIDJSON_FILE::RAPIDJSON_FILE(rapidjson::FileWriteStream* os, FILE* fp) : os(os), fp(fp)
{
	document = new rapidjson::Document();
	document->SetObject();

	allocator = &document->GetAllocator();
}

RAPIDJSON_FILE::RAPIDJSON_FILE(rapidjson::FileReadStream* is, FILE* fp) : is(is), fp(fp)
{
	document = new rapidjson::Document();
	document->ParseStream(*is);

	allocator = &document->GetAllocator();
}

RAPIDJSON_FILE::~RAPIDJSON_FILE()
{
	RELEASE(document);
	RELEASE(allocator);
	RELEASE(is);
	RELEASE(os);
}

bool RAPIDJSON_FILE::WriteFile()
{
	if (os != nullptr)
	{
		rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(*os);
		document->Accept(writer);

		return true;
	}
	else
		return false;
}

RAPIDJSON_Object* RAPIDJSON_FILE::PushValue()
{
	RAPIDJSON_Object* ret = new RAPIDJSON_Object(allocator);
	values.push_back(ret);
	return ret;
}

void RAPIDJSON_FILE::AddValue(const char* name, RAPIDJSON_Object* newValue)
{
	std::string str = name;
	rapidjson::Value genetic(str.c_str(), str.size(), *allocator);
	document->AddMember(genetic, *newValue->GetObjectValue(), *allocator);
}

RAPIDJSON_Object* RAPIDJSON_FILE::GetValue(const char* name)
{
	if (document->IsObject() && document->HasMember(name))
	{
		rapidjson::Value& value = document->operator[](name);
		RAPIDJSON_Object* ret = new RAPIDJSON_Object(allocator);
		values.push_back(ret);
		ret->GetObjectValue()->CopyFrom(value, *allocator, false);

		return ret;
	}

	return nullptr;
}

void RAPIDJSON_FILE::SetValue(const char* name, RAPIDJSON_Object* newValue)
{
	if (this->document->HasMember(name))
		this->document->operator[](name) = *newValue->GetObjectValue(); //If it exists modify it
	else
		this->AddValue(name, newValue); //if not, set a new one
}

void RAPIDJSON_FILE::CloseFile()
{
	fclose(fp);
}

// ---------------- JSON VALUE: OBJECT  ----------------- //

RAPIDJSON_Object::RAPIDJSON_Object(rapidjson::Document::AllocatorType* allocator) : allocator(allocator)
{
	value = new rapidjson::Value(rapidjson::kObjectType);
}

RAPIDJSON_Object::~RAPIDJSON_Object()
{
	int size = values.size();
	for (int i = 0; i < size; i++)
	{
		RELEASE(values[i]);
	}
	values.clear();

	RELEASE(value);
}

//void RAPIDJSON_Object::ToArray()
//{
//	RELEASE(value);
//	value = new rapidjson::Value(rapidjson::kArrayType);
//}

RAPIDJSON_Object* RAPIDJSON_Object::CreateObject()
{
	RAPIDJSON_Object* ret = new RAPIDJSON_Object(allocator);
	values.push_back(ret);
	return ret;
}

RAPIDJSON_Object* RAPIDJSON_Object::GetObj(const char* name)
{
	if (value->IsObject() && value->HasMember(name))
	{
		rapidjson::Value& trueValue = value->operator[](name);
		RAPIDJSON_Object* ret = new RAPIDJSON_Object(allocator);
		values.push_back(ret);
		ret->value->CopyFrom(trueValue, *allocator, false);

		return ret;
	}

	return nullptr;
}

void RAPIDJSON_Object::SetObject(const char* name, RAPIDJSON_Object* newValue)
{
	if (this->value->HasMember(name))
		this->value->operator[](name) = *newValue->value; //If it exists modify it
	else
		this->AddObject(name, newValue); //if not, set a new one
}

void RAPIDJSON_Object::AddObject(const char* name, RAPIDJSON_Object* newValue)
{
	if (this->value->GetType() == rapidjson::kObjectType)
	{
		std::string str = name;
		rapidjson::Value generic(str.c_str(), str.size(), *allocator);
		this->value->AddMember(generic, *newValue->value, *allocator);
	}
	else if (this->value->GetType() == rapidjson::kArrayType)
	{
		this->value->PushBack(*newValue->value, *allocator);
	}
}

rapidjson::Value* RAPIDJSON_Object::GetObjectValue()
{
	return value; //Private variable to access
}


// ---------------- JSON VALUE: ARRAY  ----------------- //
RAPIDJSON_Array::RAPIDJSON_Array(rapidjson::Document::AllocatorType* allocator) : allocator(allocator)
{
	value = new rapidjson::Value(rapidjson::kArrayType);
}

RAPIDJSON_Array::~RAPIDJSON_Array()
{
	int size = values.size();
	for (int i = 0; i < size; i++)
	{
		RELEASE(values[i]);
	}
	values.clear();

	RELEASE(value);
}

void RAPIDJSON_Array::AddArray(const char* name, RAPIDJSON_Array* newValue)
{
	if (this->value->GetType() == rapidjson::kArrayType)
	{
		this->value->PushBack(*newValue->value, *allocator);
	}
}

RAPIDJSON_Array* RAPIDJSON_Array::CreateArray()
{
	RAPIDJSON_Array* ret = new RAPIDJSON_Array(allocator);
	values.push_back(ret);
	return ret;
}

RAPIDJSON_Array* RAPIDJSON_Array::GetArray(const char* name)
{
	if (value->IsArray() && value->HasMember(name))
	{
		rapidjson::Value& trueValue = value->operator[](name);
		RAPIDJSON_Array* ret = new RAPIDJSON_Array(allocator);
		values.push_back(ret);
		ret->value->CopyFrom(trueValue, *allocator, false);

		return ret;
	}
	return nullptr;
}

void RAPIDJSON_Array::SetArray(const char* name, RAPIDJSON_Array* newValue)
{
	if (this->value->HasMember(name))
		this->value->operator[](name) = *newValue->value; //If it exists modify it
	else
		this->AddArray(name, newValue); //if not, set a new one
}

rapidjson::Value* RAPIDJSON_Array::GetArrayValue()
{
	return value;
}
