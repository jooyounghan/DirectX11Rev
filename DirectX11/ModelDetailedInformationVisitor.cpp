#include "ModelDetailedInformationVisitor.h"
#include "ModelDetailWindow.h"

#include "MeshObject.h"
#include "BoundingSphere.h"
#include "OrientedBoundingBox.h"
#include "BoundingFrustum.h"

#include "Viewable.h"

#include "TransformationInformationDrawer.h"
#include "BoundingSphereInformationDrawer.h"
#include "BoundingOBBInformationDrawer.h"
#include "ViewableInformationDrawer.h"
#include "MeshObjectInformationDrawer.h"

// 특정 개체 참조 후 삭제 예정
#include "AttachableObject.h"
#include "PlaceableObject.h"

using namespace std;
using namespace ImGui;
using namespace DirectX;


ModelDetailedInformationVisitor::ModelDetailedInformationVisitor(ModelDetailWindow* ModelDetailWindowIn)
	: ModelDetailWindowCached(ModelDetailWindowIn)
{
}

void ModelDetailedInformationVisitor::Visit(PlaceableObject* PlaceableInstance)
{
    PushID(PlaceableInstance->GetObjectID().c_str());

    TransformationInformationDrawer TransformDrawer(
        PlaceableInstance, nullptr, false
    );
    TransformDrawer.DrawInformation();

    PopID();
}

void ModelDetailedInformationVisitor::Visit(MeshObject* MeshObjectInsatnce)
{
    PushID(MeshObjectInsatnce->GetObjectID().c_str());

    TransformationInformationDrawer TransformDrawer(
        MeshObjectInsatnce, MeshObjectInsatnce->GetParentObject(), false
    );
    TransformDrawer.DrawInformation();

    MeshObjectInformationDrawer MeshObjectDrawer(MeshObjectInsatnce);
    MeshObjectDrawer.DrawInformation();

    PopID();
}

void ModelDetailedInformationVisitor::Visit(BoundingSphere* BoundingSphereInstance)
{
    AObject* ParentObject = BoundingSphereInstance->GetParentObject();

    PushID(BoundingSphereInstance->GetObjectID().c_str());

    TransformationInformationDrawer TransformDrawer(
        BoundingSphereInstance, BoundingSphereInstance->GetParentObject(), false
    );
    TransformDrawer.DrawInformation();

    BoundingSphereInformationDrawer BSDrawer(BoundingSphereInstance);
    BSDrawer.DrawInformation();

    PopID();
}

void ModelDetailedInformationVisitor::Visit(OrientedBoundingBox* OBBInstance)
{
    PushID(OBBInstance->GetObjectID().c_str());

    TransformationInformationDrawer TransformDrawer(OBBInstance, OBBInstance->GetParentObject(), false);
    TransformDrawer.DrawInformation();

    BoundingOBBInformationDrawer OBBDrawer(OBBInstance);
    OBBDrawer.DrawInformation();

    PopID();
}

void ModelDetailedInformationVisitor::Visit(BoundingFrustum* BoundingFrustumInstance)
{
    PushID(BoundingFrustumInstance->GetObjectID().c_str());

    TransformationInformationDrawer TransformDrawer(BoundingFrustumInstance, BoundingFrustumInstance->GetParentObject(), false);
    TransformDrawer.DrawInformation();

    PopID();
}

void ModelDetailedInformationVisitor::Visit(Viewable* ViewableInstance)
{
    PushID(ViewableInstance->GetObjectID().c_str());

    TransformationInformationDrawer TransformDrawer(ViewableInstance, ViewableInstance->GetParentObject(), false);
    TransformDrawer.DrawInformation();

    ViewableInformationDrawer ViewableDrawer(ViewableInstance);
    ViewableDrawer.DrawInformation();

    PopID();
}
