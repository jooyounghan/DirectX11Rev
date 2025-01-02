#pragma once
#include "APort.h"
#include <string>

std::string GetBaseTypeName(std::string typeName);

template<typename T>
class VariablePort : public APort
{
public:
	VariablePort(
		Node* parentNode, const bool& isLeft,
		size_t indexCount, size_t portIndex,
		const float& radius, const ImVec2& referencedOrigin
	);
	~VariablePort() override = default;

protected:
	static VariablePort<T>* m_connectingInputPort;
	static VariablePort<T>* m_connectingOutputPort;

protected:
	bool m_isConnecting = false;
	ImVec2 m_mousePositionDuringConnect;

public:
	template<typename U>
	bool IsConnectable(VariablePort<U>* targetPort);

public:
	virtual void OnBeginDrag() override;
	virtual void OnDragging(MouseEventArgs& args) override;
	virtual void OnEndDrag() override;
};

template <typename T>
VariablePort<T>* VariablePort<T>::m_connectingInputPort = nullptr;

template <typename T>
VariablePort<T>* VariablePort<T>::m_connectingOutputPort = nullptr;

template<typename T>
inline VariablePort<T>::VariablePort(
	Node* parentNode, const bool& isLeft, 
	size_t indexCount, size_t portIndex,
	const float& radius, const ImVec2& referencedOrigin
)
	: APort(parentNode, isLeft, indexCount, portIndex, radius, referencedOrigin,
		IM_COL32(0x35, 0xCC, 0x35, 0x88), IM_COL32(0x10, 0xFF, 0x10, 0xFF),
		IM_COL32(0x46, 0x99, 0x46, 0x88), IM_COL32(0x43, 0x66, 0x43, 0xFF)
	)
{
}

template<typename T>
inline void VariablePort<T>::OnBeginDrag()
{
	APort::OnBeginDrag();
	m_isConnecting = true;
	m_mousePositionDuringConnect = m_drawCenter;
}

template<typename T>
inline void VariablePort<T>::OnDragging(MouseEventArgs& args)
{
	APort::OnDragging(args);
	m_mousePositionDuringConnect = ImVec2(args.m_mousePosX, args.m_mousePosY);
}

template<typename T>
inline void VariablePort<T>::OnEndDrag()
{
	APort::OnEndDrag();
	m_isConnecting = false;
}



template<typename T>
template<typename U>
inline bool VariablePort<T>::IsConnectable(VariablePort<U>* targetPort)
{
	return std::is_same_v<U, T> || std::is_base_of_v<U, T>;
}
