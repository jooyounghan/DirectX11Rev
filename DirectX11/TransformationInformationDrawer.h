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
    TransformationInformationDrawer(
        AObject* CurrentPlaceable, AObject* ParentPlaceable,
        const bool& IsTranslationOnlyAbsoluteIn, const bool& IsTranslationDisabledIn,
        const bool& IsRotationOnlyAbsoluteIn, const bool& IsRotationDisabledIn,
        const bool& IsScalingOnlyAbsoluteIn, const bool& IsScalingDisabledIn
    );

protected:
    AObject* ParentObjectCached = nullptr;

protected:
    bool IsTranslationOnlyAbsolute = false; 
    bool IsTranslationDisabled = false;
    bool IsRotationOnlyAbsolute = false; 
    bool IsRotationDisabled = false;
    bool IsScalingOnlyAbsolute = false; 
    bool IsScalingDisabled = false;

public:
    virtual void DrawInformation() override;


private:
    static void DrawTransformationEntitySelection(
        const bool& IsOnlyAbsoluteIn, 
        const char* EntityName, 
        ETransfomationSelect& SelectedIndexOut
    );

    template<typename T>
    static void DrawTransformationEntity(
        const ETransfomationSelect& SelectedIndex,
        T& Entity,
        const T& ParentEntity,
        const bool& DisabledIn
    );

private:
    static const char* TransformationSelect[NumTransformationSelect];
};
