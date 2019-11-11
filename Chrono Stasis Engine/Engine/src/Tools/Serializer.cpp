#include "Serializer.h"
#include "csGlobals.h"


Serializer::Serializer()
{
}

Serializer::~Serializer()
{
}

RJSON_File* Serializer::JSONRead(const char* path)
{
	FILE* fp = fopen(path, "rb");
	char readBuffer[65536];

	if (fp != nullptr)
		return new RJSON_File(new rapidjson::FileReadStream(fp, readBuffer, sizeof(readBuffer)), fp);
	else
		return nullptr;
}

RJSON_File* Serializer::JSONWrite(const char* path)
{
	FILE* fp = fopen(path, "wb");
	char writeBuffer[65536];

	if (fp != nullptr)
		return new RJSON_File(new rapidjson::FileWriteStream(fp, writeBuffer, sizeof(writeBuffer)), fp);
	else
		return nullptr;
}

void Serializer::JSONClose(RJSON_File* file)
{
	if (file != nullptr)
		file->CloseFile();

	RELEASE(file);
	file = nullptr;
}

// ---------------- JSON FILE  ----------------- //
RJSON_File::RJSON_File(rapidjson::FileWriteStream* os, FILE* fp) : os(os), fp(fp)
{
	document = new rapidjson::Document();
	document->SetObject();

	allocator = &document->GetAllocator();
}

RJSON_File::RJSON_File(rapidjson::FileReadStream* is, FILE* fp) : is(is), fp(fp)
{
	document = new rapidjson::Document();
	document->ParseStream(*is);

	allocator = &document->GetAllocator();
}

RJSON_File::~RJSON_File()
{
	int size = values.size();
	for (int i = 0; i < size; i++)
	{
		RELEASE(values[i]);
	}
	values.clear();

	RELEASE(document);
	RELEASE(allocator);
	RELEASE(is);
	RELEASE(os);
}


bool RJSON_File::WriteFile()
{
	bool ret = false;
	if (os != nullptr)
	{
		rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(*os);
		ret = document->Accept(writer);

		return ret;
	}
	else
		return ret;
}


void RJSON_File::AddValue(const char* name, RJSON_Value* newValue)
{
	std::string str = name;
	rapidjson::Value genetic(str.c_str(), str.size(), *allocator);
	document->AddMember(genetic, *newValue->GetJSONValue(), *allocator);
}

RJSON_Value* RJSON_File::GetValue(const char* name)
{
	if (document->IsObject() && document->HasMember(name))
	{
		rapidjson::Value& value = document->operator[](name);
		RJSON_Value* ret = new RJSON_Value(allocator);
		values.push_back(ret);
		ret->GetJSONValue()->CopyFrom(value, *allocator, false);

		return ret;
	}

	return nullptr;
}

void RJSON_File::SetValue(const char* name, RJSON_Value* newValue)
{
	if (this->document->HasMember(name))
		this->document->operator[](name) = *newValue->GetJSONValue(); 
	else
		this->AddValue(name, newValue); 

}

void RJSON_File::CloseFile()
{
	fclose(fp);
}

// ---------------- JSON VALUE ----------------- //

RJSON_Value::~RJSON_Value()
{
	int size = values.size();
	for (int i = 0; i < size; i++)
	{
		RELEASE(values[i]);
	}
	values.clear();

	RELEASE(value);
}

void RJSON_Value::TransformTo(JSONValueType type)
{
	switch (type) 
	{
		case JSONValueType::OBJECT_VALUE:
			break;
		case JSONValueType::ARRAY_VALUE:
			RELEASE(value);
			value = new rapidjson::Value(rapidjson::kArrayType);
			break;
		case JSONValueType::STRING_VALUE:
			RELEASE(value);
			value = new rapidjson::Value(rapidjson::kStringType);
			break;
		case JSONValueType::NUMBER_VALUE:
			RELEASE(value);
			value = new rapidjson::Value(rapidjson::kNumberType);
			break;
		case JSONValueType::TRUE_VALUE:
			RELEASE(value);
			value = new rapidjson::Value(rapidjson::kTrueType);
			break;
		case JSONValueType::FALSE_VALUE:
			RELEASE(value);
			value = new rapidjson::Value(rapidjson::kFalseType);
			break;

	}
}

//RJSON_Value* RJSON_Value::CreateValue(JSONValueType type)
//{
//	RJSON_Value* value = nullptr;
//	
//		switch (type) {
//	
//			case JSONValueType::OBJECT_VALUE:
//				value = new RJSON_Object(allocator);
//				values.push_back(value);
//				break;
//	
//			case JSONValueType::ARRAY_VALUE:
//				value = new RJSON_Array(allocator);
//				values.push_back(value);
//				break;
//	
//			default:
//				break;
//	
//		}
//
//}

RJSON_Value* RJSON_Value::CreateValue()
{
	RJSON_Value* ret = new RJSON_Value(allocator);
	values.push_back(ret);
	return ret;
}

void RJSON_Value::AddValue(const char* name, RJSON_Value* newValue)
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

RJSON_Value* RJSON_Value::GetValue(const char* name)
{
	if (value->IsObject() && value->HasMember(name))
	{
		rapidjson::Value& trueValue = value->operator[](name);
		RJSON_Value* ret = new RJSON_Value(allocator);
		values.push_back(ret);
		ret->value->CopyFrom(trueValue, *allocator, false);

		return ret;
	}

	return nullptr;
}

void RJSON_Value::SetValue(const char* name, RJSON_Value* newValue)
{
	if (this->value->HasMember(name))
		this->value->operator[](name) = *newValue->value; //If it exists modify it
	else
		this->AddValue(name, newValue); //if not, set a new one
}

rapidjson::Value* RJSON_Value::GetJSONValue()
{
	return nullptr;
}


/*
// ---------------- JSON VALUE: OBJECT  ----------------- //
RJSON_Object::RJSON_Object(rapidjson::Document::AllocatorType* alloc) : RJSON_Value(alloc)
{
	value = new rapidjson::Value(rapidjson::kObjectType);
	allocator = alloc;
}

RJSON_Object::~RJSON_Object()
{
	int size = values.size();
	for (int i = 0; i < size; i++)
	{
		RELEASE(values[i]);
	}
	values.clear();

	RELEASE(value);
}


RJSON_Object* RJSON_Object::CreateObject()
{
	RJSON_Object* ret = new RJSON_Object(allocator);
	values.push_back(ret);
	return ret;
}

RJSON_Object* RJSON_Object::GetObj(const char* name)
{
	if (value->IsObject() && value->HasMember(name))
	{
		rapidjson::Value& trueValue = value->operator[](name);
		RJSON_Object* ret = new RJSON_Object(allocator);
		values.push_back(ret);
		ret->value->CopyFrom(trueValue, *allocator, false);

		return ret;
	}

	return nullptr;
}

void RJSON_Object::SetObject(const char* name, RJSON_Object* newValue)
{
	if (this->value->HasMember(name))
		this->value->operator[](name) = *newValue->value; //If it exists modify it
	else
		this->AddObject(name, newValue); //if not, set a new one
}

void RJSON_Object::AddObject(const char* name, RJSON_Object* newValue)
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

rapidjson::Value* RJSON_Object::GetObjectValue()
{
	return value; //Private variable to access
}


// ---------------- JSON VALUE: ARRAY  ----------------- //
RJSON_Array::RJSON_Array(rapidjson::Document::AllocatorType* alloc) : RJSON_Value(alloc)
{
	this->value = new rapidjson::Value(rapidjson::kArrayType);
	//this->allocator = alloc;
}

RJSON_Array::~RJSON_Array()
{
	int size = values.size();
	for (int i = 0; i < size; i++)
	{
		RELEASE(values[i]);
	}
	values.clear();

	RELEASE(value);
}

void RJSON_Array::AddArray(const char* name, RJSON_Array* newValue)
{
	if (this->value->GetType() == rapidjson::kArrayType)
	{
		this->value->PushBack(*newValue->value, *allocator);
	}
}

RJSON_Array* RJSON_Array::CreateArray()
{
	RJSON_Array* ret = new RJSON_Array(allocator);
	values.push_back(ret);
	return ret;
}

RJSON_Array* RJSON_Array::GetArray(const char* name)
{
	if (value->IsArray() && value->HasMember(name))
	{
		rapidjson::Value& trueValue = value->operator[](name);
		RJSON_Array* ret = new RJSON_Array(allocator);
		values.push_back(ret);
		ret->value->CopyFrom(trueValue, *allocator, false);

		return ret;
	}
	return nullptr;
}

void RJSON_Array::SetArray(const char* name, RJSON_Array* newValue)
{
	if (this->value->HasMember(name))
		this->value->operator[](name) = *newValue->value; //If it exists modify it
	else
		this->AddArray(name, newValue); //if not, set a new one
}

rapidjson::Value* RJSON_Array::GetArrayValue()
{
	return value;
}

*/


