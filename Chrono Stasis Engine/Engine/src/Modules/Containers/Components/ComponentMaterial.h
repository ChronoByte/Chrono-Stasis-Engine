#pragma once
#include "csComponent.h"
#include "Color.h"
#include "csGlobals.h"


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
	virtual ~ComponentMaterial();

	void SetColor(float r, float g, float b, float a);
	Color GetColor() const;

	const TextureInfo* GetTexture()const;
	void SetTexture(TextureInfo* texture);

	void SetMaterial(TextureInfo* texture, float r, float g, float b, float a);

	void InspectorInfo();
private:
	Color color;
	TextureInfo* texture = nullptr; 
	
};