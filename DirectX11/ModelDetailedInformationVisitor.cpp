#include "ModelDetailedInformationVisitor.h"
#include "ModelDetailWindow.h"

#include "GameWorld.h"

#include "MeshObjectActor.h"
#include "EnvironmentActor.h"
#include "EditorPawn.h"

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

using namespace std;
using namespace ImGui;
using namespace DirectX;


ModelDetailedInformationVisitor::ModelDetailedInformationVisitor(AssetManager* AssetManagerInstanceIn)
	: AssetManagerCached(AssetManagerInstanceIn)
{
}

void ModelDetailedInformationVisitor::Visit(MeshObjectActor* ActorInstance)
{
    PushID(ActorInstance->GetObjectID().c_str());

    TransformationInformationDrawer TransformDrawer(ActorInstance, nullptr);
    TransformDrawer.DrawInformation();

    MeshObjectInformationDrawer MeshObjectDrawer(ActorInstance->GetMeshObjectInstance(), AssetManagerCached);
    MeshObjectDrawer.DrawInformation();

    PopID();
}

void ModelDetailedInformationVisitor::Visit(EnvironmentActor* EnvironmentActorInstance)
{
    PushID(EnvironmentActorInstance->GetObjectID().c_str());

    TransformationInformationDrawer TransformDrawer(EnvironmentActorInstance, nullptr);
    TransformDrawer.DrawInformation();

    PopID();
}

void ModelDetailedInformationVisitor::Visit(EditorPawn* EditorActorInstance)
{
    PushID(EditorActorInstance->GetObjectID().c_str());

    TransformationInformationDrawer TransformDrawer(EditorActorInstance, nullptr);
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

    Checkbox("Consume Movement", ViewableInstance->GetPointerbIsConsumeMove());

    PopID();
}
