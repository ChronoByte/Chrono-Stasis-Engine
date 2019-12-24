#pragma once
#include "csComponent.h"
#include "Color.h"
#include "csGlobals.h"

enum class BillboardType
{
	SCREEN = 0,
	WORLD,
	AXIS,

	// ----

	NONE
};
class ComponentBillboard : public Component
{

public:

	ComponentBillboard(GameObject* parent);
	~ComponentBillboard();

	void Update(float dt) override;

	void InspectorInfo() override;

	
	// Parser
	void Save(JSON_Object* object, std::string name, bool saveScene, uint& countResources) const;
	void Load(const JSON_Object* object, std::string name);

private: 

	BillboardType bbtype = BillboardType::NONE;
};