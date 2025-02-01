#pragma once
class SpotLightComponent;
class PointLightComponent;

class ILightVisitor
{
public:
	virtual ~ILightVisitor() = default;

public:
	virtual void Visit(SpotLightComponent* spotLightComponent) = 0;
	virtual void Visit(PointLightComponent* pointLightComponent) = 0;
};

