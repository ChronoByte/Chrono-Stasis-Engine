#pragma once
#include "csComponent.h"
#include "csGlobals.h"

enum class BillboardType
{
	SCREEN = 0,
	WORLD,
	AXIS,

	// ----

	NONE
};

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

	void AlignToScreen(ComponentCamera * camera);
	void AlignToWorld(ComponentCamera * camera);
	void AlignToAxis(ComponentCamera * camera, int axisLocked = 0);

private: 

	BillboardType bbType = BillboardType::NONE;
	int currentSelected = (int)bbType;
	int axisLocked = 0;

};