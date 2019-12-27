#pragma once
#include "csComponent.h"
#include "Color.h"
#include "csGlobals.h"

struct RJSON_Value;
//struct Texture {
//	uint id = 0u;
//	uint width = 0u;
//	uint height = 0u;
//	std::string path;
//};

struct TextureInfo;
class GameObject; 

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(GameObject* parent);
	~ComponentMaterial();
	void Update(float dt) override;

	void SetColor(float r, float g, float b, float a);
	Color GetColor() const;

	const TextureInfo* GetTexture()const;
	void SetTexture(TextureInfo* texture);

	void SetMaterial(TextureInfo* texture, float r, float g, float b, float a);

	void InspectorInfo();

	
	// Parser
	void Save(JSON_Object* object, std::string name, bool saveScene, uint& countResources) const;
	void Load(const JSON_Object* object, std::string name);

private:
	Color color;
	TextureInfo* texture = nullptr; 
	TextureInfo* checkersTex = nullptr; 

	bool checkers = false; 
	bool selectMaterial = false;

public:

	bool alpha_test = false;
};