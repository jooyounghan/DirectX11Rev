#include "ModelDetailedInformationVisitor.h"
#include "ModelDetailWindow.h"

#include "GameWorld.h"

#include "StaticMeshObjectActor.h"
#include "SkeletalMeshObjectActor.h"
#include "EnvironmentActor.h"
#include "EditorPawn.h"

#include "StaticMeshObject.h"
#include "SkeletalMeshObject.h"
#include "BoundingSphereObject.h"
#include "OBBObject.h"
#include "BoundingFrustumObject.h"
#include "Viewable.h"

#include "TransformationInformationDrawer.h"
#include "StaticMeshObjectInformationDrawer.h"
#include "SkeletalMeshObjectInformationDrawer.h"
#include "BoundingSphereInformationDrawer.h"
#include "BoundingOBBInformationDrawer.h"
#include "ViewableInformationDrawer.h"
#include "StaticMeshObjectInformationDrawer.h"

using namespace std;
using namespace ImGui;
using namespace DirectX;


ModelDetailedInformationVisitor::ModelDetailedInformationVisitor(AssetManager* AssetManagerInstanceIn)
	: AssetManagerCached(AssetManagerInstanceIn)
{
}

void ModelDetailedInformationVisitor::Visit(StaticMeshObjectActor* ActorInstance)
{
    PushID(ActorInstance->GetObjectID().c_str());

    TransformationInformationDrawer TransformDrawer(ActorInstance, nullptr);
    TransformDrawer.DrawInformation();

    StaticMeshObjectInformationDrawer MeshObjectDrawer(ActorInstance->GetStaticMeshObjectInstance(), AssetManagerCached);
    MeshObjectDrawer.DrawInformation();

    PopID();
}

void ModelDetailedInformationVisitor::Visit(SkeletalMeshObjectActor* ActorInstance)
{
    PushID(ActorInstance->GetObjectID().c_str());

    TransformationInformationDrawer TransformDrawer(ActorInstance, nullptr);
    TransformDrawer.DrawInformation();

    SkeletalMeshObjectInformationDrawer MeshObjectDrawer(ActorInstance->GetSkeletalMeshObjectInstance(), AssetManagerCached);
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

void ModelDetailedInformationVisitor::Visit(StaticMeshObject* StaticMeshObjectInstance)
{
    PushID(StaticMeshObjectInstance->GetObjectID().c_str());

    TransformationInformationDrawer TransformDrawer(StaticMeshObjectInstance, StaticMeshObjectInstance->GetParentObject());
    TransformDrawer.DrawInformation();

    StaticMeshObjectInformationDrawer MeshObjectDrawer(StaticMeshObjectInstance, AssetManagerCached);
    MeshObjectDrawer.DrawInformation();

    PopID();
}

void ModelDetailedInformationVisitor::Visit(SkeletalMeshObject* SkeletalMeshObjectInstance)
{
    PushID(SkeletalMeshObjectInstance->GetObjectID().c_str());

    TransformationInformationDrawer TransformDrawer(SkeletalMeshObjectInstance, SkeletalMeshObjectInstance->GetParentObject());
    TransformDrawer.DrawInformation();

    SkeletalMeshObjectInformationDrawer MeshObjectDrawer(SkeletalMeshObjectInstance, AssetManagerCached);
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
