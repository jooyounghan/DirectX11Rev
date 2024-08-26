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
    TransformationInformationDrawer(AObject* CurrentObject, AObject* ParentObject = nullptr);

protected:
    AObject* ParentObjectCached = nullptr;

public:
    virtual void DrawInformation() override;


private:
    static void DrawEntitySelection(
        const char* EntityName, 
        ETransfomationSelect& SelectedIndexOut
    );

private:
    void DrawPositionEntity(const ETransfomationSelect& PositionSelectedIndex);
    void DrawAngleEntity(const ETransfomationSelect& PositionSelectedIndex);
    void DrawScaleEntity(const ETransfomationSelect& PositionSelectedIndex);

private:
    static const char* TransformationSelect[NumTransformationSelect];
};
