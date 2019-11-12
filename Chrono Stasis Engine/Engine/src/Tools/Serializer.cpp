#include "Serializer.h"
#include "csGlobals.h"




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
	for (auto& value : values)
	{
		RELEASE(value);
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


RJSON_Value* RJSON_File::CreateValue(rapidjson::Type type)
{
	RJSON_Value* ret = new RJSON_Value(allocator, type);
	values.push_back(ret);
	return ret;
	
}

void RJSON_File::AddValue(const char* name, const RJSON_Value& newValue)
{
	std::string str = name;
	rapidjson::Value genetic(str.c_str(), str.size(), *allocator);
	document->AddMember(genetic, *newValue.GetJSONValue(), *allocator);
}

RJSON_Value* RJSON_File::GetValue(const char* name)
{
	if (document->IsObject() && document->HasMember(name))
	{
		RJSON_Value* ret = new RJSON_Value(allocator);
		ret->GetJSONValue()->CopyFrom(document->operator[](name), *allocator, false);
		values.push_back(ret);

		return ret;
	}

	return nullptr;
}

void RJSON_File::SetValue(const char* name, RJSON_Value& newValue)
{
	if (this->document->HasMember(name))
		this->document->operator[](name) = *newValue.GetJSONValue(); 
	else
		this->AddValue(name, newValue); 

}

void RJSON_File::CloseFile()
{
	fclose(fp);
	RELEASE(document);
	RELEASE(allocator);
	RELEASE(is);
	RELEASE(os);
}

// ---------------- JSON VALUE ----------------- //
RJSON_Value::RJSON_Value(rapidjson::Document::AllocatorType* allocator, rapidjson::Type type) : allocator(allocator)
{
	value = new rapidjson::Value(type);
}

RJSON_Value::~RJSON_Value()
{
	
	for (auto& value : values)
	{
		RELEASE(value);
	}
	values.clear();

	RELEASE(value);
}

RJSON_Value* RJSON_Value::CreateValue(rapidjson::Type type)
{
	RJSON_Value* ret = new RJSON_Value(allocator,type);
	values.push_back(ret);
	return ret;
}

void RJSON_Value::TransformTo(rapidjson::Type type)
{
	switch (type)
	{
	case rapidjson::Type::kObjectType:
			RELEASE(this->value);
			this->value = new rapidjson::Value(rapidjson::kObjectType);
			break;
	case rapidjson::Type::kArrayType:
			RELEASE(this->value);
			this->value = new rapidjson::Value(rapidjson::kArrayType);
			break;
	case rapidjson::Type::kStringType:
			RELEASE(this->value);
			this->value = new rapidjson::Value(rapidjson::kStringType);
			break;
		case rapidjson::Type::kNumberType:
			RELEASE(this->value);
			this->value = new rapidjson::Value(rapidjson::kNumberType);
			break;
		case rapidjson::Type::kTrueType:
			RELEASE(this->value);
			value = new rapidjson::Value(rapidjson::kTrueType);
			break;
		case rapidjson::Type::kFalseType:
			RELEASE(this->value);
			value = new rapidjson::Value(rapidjson::kFalseType);
			break;

	}
}


void RJSON_Value::AddValue(const char* name,const RJSON_Value& newValue)
{

	switch (this->value->GetType())
	{
		case rapidjson::kObjectType:
		{
			std::string str = name;
			rapidjson::Value generic(str.c_str(), str.size(), *allocator);
			this->value->AddMember(generic, *newValue.value, *allocator);
			break;
		}
		case rapidjson::kArrayType:
			this->value->PushBack(*newValue.value, *allocator);
			break;
	}


}

RJSON_Value* RJSON_Value::GetValue(const char* name)
{
	if (value->IsObject() && value->HasMember(name))
	{
		RJSON_Value* ret = new RJSON_Value(allocator);
		ret->value->CopyFrom(value->operator[](name), *allocator, false);
		values.push_back(ret);

		return ret;
	}

	return nullptr;
}

void RJSON_Value::SetValue(const char* name, RJSON_Value& newValue)
{
	if (this->value->HasMember(name))
		this->value->operator[](name) = *newValue.value; //If it exists modify it
	else
		this->AddValue(name, newValue); //if not, set a new one
}

rapidjson::Value* RJSON_Value::GetJSONValue() const
{
	return value;
}

// ---------------- JSON SETS ----------------- //
void RJSON_Value::SetInt(const char* name, int value)
{
	std::string str = name;
	rapidjson::Value index(str.c_str(), str.size(), *allocator);
	this->value->AddMember(index, value, *allocator);
}

void RJSON_Value::SetUint(const char* name, uint value)
{
	std::string str = name;
	rapidjson::Value index(str.c_str(), str.size(), *allocator);
	this->value->AddMember(index, value, *allocator);
}

void RJSON_Value::SetFloat(const char* name, float value)
{
	std::string str = name;
	rapidjson::Value index(str.c_str(), str.size(), *allocator);
	this->value->AddMember(index, value, *allocator);
}

void RJSON_Value::SetString(const char* name, const char* value)
{
	std::string str = name;
	rapidjson::Value index(str.c_str(), str.size(), *allocator);
	std::string str2 = value;
	rapidjson::Value val(str2.c_str(), str2.size(), *allocator);
	this->value->AddMember(index, val, *allocator);
}

void RJSON_Value::SetBoolean(const char* name, bool value)
{
	std::string str = name;
	rapidjson::Value index(str.c_str(), str.size(), *allocator);
	this->value->AddMember(index, value, *allocator);
}

void RJSON_Value::SetVector3(const char* name, float3 vec)
{
	std::string str = name;
	rapidjson::Value index(str.c_str(), str.size(), *allocator);

	rapidjson::Value a(rapidjson::kArrayType);
	a.PushBack(vec.x, *allocator);
	a.PushBack(vec.y, *allocator);
	a.PushBack(vec.z, *allocator);

	this->value->AddMember(index, a, *allocator);
}

void RJSON_Value::SetQuaternion(const char* name, Quat quat)
{
	std::string str = name;
	rapidjson::Value index(str.c_str(), str.size(), *allocator);

	rapidjson::Value a(rapidjson::kArrayType);
	a.PushBack(quat.x, *allocator);
	a.PushBack(quat.y, *allocator);
	a.PushBack(quat.z, *allocator);
	a.PushBack(quat.w, *allocator);

	this->value->AddMember(index, a, *allocator);

}
