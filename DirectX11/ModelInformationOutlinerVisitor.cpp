#include "ModelInformationOutlinerVisitor.h"
#include "MapOutlinerWindow.h"

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


ModelInformationOutlinerVisitor::ModelInformationOutlinerVisitor(MapOutlinerWindow* MapOutlinerInstance)
	: MapOutlinerCached(MapOutlinerInstance)
{
}

void ModelInformationOutlinerVisitor::Visit(MeshObject* MeshObjectInsatnce)
{
    PushID(MeshObjectInsatnce->GetObjectID().c_str());

    TransformationInformationDrawer TransformDrawer(
        MeshObjectInsatnce, MeshObjectInsatnce->GetParentObject(),
        false, false,
        false, false,
        true, false
    );
    TransformDrawer.DrawInformation();

    MeshObjectInformationDrawer MeshObjectDrawer(MeshObjectInsatnce);
    MeshObjectDrawer.DrawInformation();

    PopID();
}

void ModelInformationOutlinerVisitor::Visit(BoundingSphere* BoundingSphereInstance)
{
    APlaceable* ParentObject = BoundingSphereInstance->GetParentObject();

    PushID(BoundingSphereInstance->GetObjectID().c_str());

    TransformationInformationDrawer TransformDrawer(
        BoundingSphereInstance, BoundingSphereInstance->GetParentObject(),
        false, false,
        false, false,
        true, false
    );
    TransformDrawer.DrawInformation();

    BoundingSphereInformationDrawer BSDrawer(BoundingSphereInstance);
    BSDrawer.DrawInformation();

    PopID();
}

void ModelInformationOutlinerVisitor::Visit(OrientedBoundingBox* OBBInstance)
{
    PushID(OBBInstance->GetObjectID().c_str());

    TransformationInformationDrawer TransformDrawer(
        OBBInstance, OBBInstance->GetParentObject(),
        false, false,
        false, false,
        true, false
    );
    TransformDrawer.DrawInformation();

    BoundingOBBInformationDrawer OBBDrawer(OBBInstance);
    OBBDrawer.DrawInformation();

    PopID();
}

void ModelInformationOutlinerVisitor::Visit(BoundingFrustum* BoundingFrustumInstance)
{
    PushID(BoundingFrustumInstance->GetObjectID().c_str());

    TransformationInformationDrawer TransformDrawer(
        BoundingFrustumInstance, BoundingFrustumInstance->GetParentObject(),
        false, false,
        false, false,
        true, false
    );
    TransformDrawer.DrawInformation();

    PopID();
}

void ModelInformationOutlinerVisitor::Visit(Viewable* ViewableInstance)
{
    PushID(ViewableInstance->GetObjectID().c_str());

    TransformationInformationDrawer TransformDrawer(
        ViewableInstance, ViewableInstance->GetParentObject(),
        false, false,
        false, false,
        true, false
    );
    TransformDrawer.DrawInformation();

    ViewableInformationDrawer ViewableDrawer(ViewableInstance);
    ViewableDrawer.DrawInformation();

    PopID();
}


//void ModelInformationOutlinerVisitor::DrawTransformationForAttachable(AttachableObject* Attachable)
//{
//    APlaceable* ParentObject = Attachable->GetParentObject();
//
//    static SPosition4D DummyPosition;
//    static SAngle DummyAngle;
//    static SVector3D DummyScale;
//
//    DrawTransformation(
//        true, true, DummyPosition, ParentObject != nullptr ? ParentObject->Position : SPosition4D(),
//        true, true, DummyAngle, ParentObject != nullptr ? ParentObject->Angle : SAngle(),
//        true, true, DummyScale, SVector3D()
//    );
//}
//
//void ModelInformationOutlinerVisitor::DrawTransformationForPlaceables(PlaceableObject* Placeable)
//{
//    DrawTransformation(
//        true, false, Placeable->Position, SPosition4D(),
//        true, false, Placeable->Angle, SAngle(),
//        true, false, Placeable->Scale, SVector3D()
//    );
//}

