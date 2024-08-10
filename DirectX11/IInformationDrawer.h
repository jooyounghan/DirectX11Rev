#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"

class IInformationDrawerImp
{
public:
	virtual void DrawInformation() = 0;
};

template<typename T>
class AInformationDrawer : public IInformationDrawerImp
{
public:
	AInformationDrawer(T* ObjectIn)
		: ObjectCached(ObjectIn) {}

protected:
	T* ObjectCached = nullptr;
};