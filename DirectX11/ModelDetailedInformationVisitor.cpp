#include "ModelDetailedInformationVisitor.h"
#include "ModelDetailWindow.h"

#include "GameWorld.h"

#include "MeshObject.h"
#include "BoundingSphereObject.h"
#include "OBBObject.h"
#include "BoundingFrustumObject.h"
#include "Viewable.h"

#include "TransformationInformationDrawer.h"
#include "BoundingSphereInformationDrawer.h"
#include "BoundingOBBInformationDrawer.h"
#include "ViewableInformationDrawer.h"
#include "MeshObjectInformationDrawer.h"

// 특정 개체 참조 후 삭제 예정
#include "PlaceableObject.h"

using namespace std;
using namespace ImGui;
using namespace DirectX;


ModelDetailedInformationVisitor::ModelDetailedInformationVisitor(AssetManager* AssetManagerInstanceIn)
	: AssetManagerCached(AssetManagerInstanceIn)
{
}

void ModelDetailedInformationVisitor::Visit(PlaceableObject* PlaceableInstance)
{
    PushID(PlaceableInstance->GetObjectID().c_str());

    TransformationInformationDrawer TransformDrawer(PlaceableInstance, nullptr);
    TransformDrawer.DrawInformation();

    PopID();
}

void ModelDetailedInformationVisitor::Visit(MeshObject* MeshObjectInsatnce)
{
    PushID(MeshObjectInsatnce->GetObjectID().c_str());

    TransformationInformationDrawer TransformDrawer(MeshObjectInsatnce, MeshObjectInsatnce->GetParentObject());
    TransformDrawer.DrawInformation();

    MeshObjectInformationDrawer MeshObjectDrawer(MeshObjectInsatnce, AssetManagerCached);
    MeshObjectDrawer.DrawInformation();

    PopID();
}

void ModelDetailedInformationVisitor::Visit(BoundingSphereObject* BoundingSphereInstance)
{
    AObject* ParentObject = BoundingSphereInstance->GetParentObject();

    PushID(BoundingSphereInstance->GetObjectID().c_str());

    TransformationInformationDrawer TransformDrawer(BoundingSphereInstance, BoundingSphereInstance->GetParentObject());
    TransformDrawer.DrawInformation();

    BoundingSphereInformationDrawer BSDrawer(BoundingSphereInstance);
    BSDrawer.DrawInformation();

    PopID();
}

void ModelDetailedInformationVisitor::Visit(OBBObject* OBBInstance)
{
    PushID(OBBInstance->GetObjectID().c_str());

    TransformationInformationDrawer TransformDrawer(OBBInstance, OBBInstance->GetParentObject());
    TransformDrawer.DrawInformation();

    BoundingOBBInformationDrawer OBBDrawer(OBBInstance);
    OBBDrawer.DrawInformation();

    PopID();
}

void ModelDetailedInformationVisitor::Visit(BoundingFrustumObject* BoundingFrustumInstance)
{
    PushID(BoundingFrustumInstance->GetObjectID().c_str());

    TransformationInformationDrawer TransformDrawer(BoundingFrustumInstance, BoundingFrustumInstance->GetParentObject());
    TransformDrawer.DrawInformation();

    PopID();
}

void ModelDetailedInformationVisitor::Visit(Viewable* ViewableInstance)
{
    PushID(ViewableInstance->GetObjectID().c_str());

    TransformationInformationDrawer TransformDrawer(ViewableInstance, ViewableInstance->GetParentObject());
    TransformDrawer.DrawInformation();

    ViewableInformationDrawer ViewableDrawer(ViewableInstance);
    ViewableDrawer.DrawInformation();

    PopID();
}
