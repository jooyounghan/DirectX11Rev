#pragma once
#include "IInformationDrawer.h"

class AObject;

enum ETransfomationSelect : size_t
{
    Absolute,
    Relative,
    NumTransformationSelect
};

class TransformationInformationDrawer : AInformationDrawer<AObject>
{
public:
    TransformationInformationDrawer(AObject* CurrentPlaceable, AObject* ParentPlaceable, const bool& IsPlaceable);

protected:
    AObject* ParentObjectCached = nullptr;

public:
    virtual void DrawInformation() override;


private:
    static void DrawEntitySelection(
        const char* EntityName, 
        ETransfomationSelect& SelectedIndexOut
    );

    template<typename T, typename Operator, typename ROperator>
    static void DrawEntity(
        const ETransfomationSelect& SelectedIndex,
        T& Entity,
        const T& ParentEntity,
        const float& SpeedIn
    );

private:
    static const char* TransformationSelect[NumTransformationSelect];
};
