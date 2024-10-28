#pragma once
#include "RectangleDrawElement.h"
#include "StaticAssertHelper.h"
#include "InputPort.h"
#include "AOutputPort.h"

#include <list>
#include <string>
#include <memory>
#include <format>

constexpr float NodePaddingSize = 10.f;

class NodeElement : public RectangleDrawElement
{
public:
	NodeElement(
		const ImVec2& CenterPositionIn,
		const ImVec2& RectangleSizeIn,
		const ImU32& RectangleColorIn,
		const ImU32& HilightedColorIn
	);
	virtual ~NodeElement();

protected:
	std::list<std::shared_ptr<AOutputPort>> OutputPorts;
	std::list<std::shared_ptr<InputPort>> InputPorts;
	MakeGetter(OutputPorts);
	MakeGetter(InputPorts)

protected:
	std::string NodeID;

public:
	template<typename T, typename... Args>
	T* AddOutputPort(Args&&... args)
	{
		static_assert(std::is_base_of<AOutputPort, T>::value, DerivedCondition(AOutputPort));
		std::shared_ptr<T> OutputPortInstance = std::make_shared<T>(args...);
		OutputPortInstance->SetParentNodeElement(this);
		OutputPorts.emplace_back(OutputPortInstance);
		const ImVec2 NodePosition = GetPosition();
		SetPosition(NodePosition);
		return OutputPortInstance.get();
	}

	template<typename T, typename... Args>
	T* AddInputPort(Args&&... args)
	{
		static_assert(std::is_base_of<InputPort, T>::value, DerivedCondition(AOutputPort));
		std::shared_ptr<T> InputPortInstance = std::make_shared<T>(args...);
		InputPortInstance->SetParentNodeElement(this);
		InputPorts.emplace_back(InputPortInstance);
		const ImVec2 NodePosition = GetPosition();
		SetPosition(NodePosition);
		return InputPortInstance.get();
	}

public:
	virtual void SetPosition(const ImVec2& CenterPosition) override;
	virtual void AddToDrawList(const ImVec2& OriginPosition, ImDrawList* DrawListIn) override;
};

