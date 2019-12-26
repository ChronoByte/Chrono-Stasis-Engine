#pragma once
#include "csComponent.h"
#include "csGlobals.h"
#include "Billboard.h"

class ComponentCamera;

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

	BillboardType bbType = BillboardType::NONE;
	int currentSelected = (int)bbType;
	int axisLocked = 0;

};