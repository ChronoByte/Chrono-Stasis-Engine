#pragma once
#include "csComponent.h"
#include "csGlobals.h"
#include "MathGeoLib/include/Math/float3.h"

class ComponentCamera;

enum class LightType
{
	L_DIRECTIONAL = 0,
	L_POINT,
	L_SPOT
};

class ComponentLight : public Component
{

public:

	ComponentLight(GameObject* parent);
	~ComponentLight();

	
	void Update(float dt) override;
	void InspectorInfo() override;

	// Parser
	void Save(JSON_Object* object, std::string name, bool saveScene, uint& countResources) const;
	void Load(const JSON_Object* object, std::string name);

private:
	void SetPosition(const float3& pos);
	void SetDirection(const float3& dir);
	void SetColor(const float3& col);
	void SetFading(const float3& fade);

	float3 GetPosition() const;
	float3 GetDirection() const;
	float3 GetColor() const;
	float3 GetFading() const;

	void DrawLight() const;

	void DrawDirectional() const;
	void DrawSpot() const;
	void DrawPoint() const;
	void DrawArea(const Circle& circle) const;
	

private:

	LightType lightType = LightType::L_POINT;
	int currentSelected = (int)lightType;

	float3 position = float3::zero; 
	float3 direction = float3::one;
	float3 color = float3::one;

	float3 fading = float3(0.1f, 0.1f, 0.1f);
	
};