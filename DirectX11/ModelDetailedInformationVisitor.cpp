#include "ModelDetailedInformationVisitor.h"
#include "GameWorld.h"

#include "MapAsset.h"

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
#include "EnvironementActorDrawer.h"
#include "MaterialInformationDrawer.h"

using namespace std;
using namespace ImGui;
using namespace DirectX;


ModelDetailedInformationVisitor::ModelDetailedInformationVisitor()
{
}

void ModelDetailedInformationVisitor::Visit(MapAsset* MapAssetInstance)
{
    MapAssetInstance->GetEditorActorInstance()->AcceptGui(this);
    MapAssetInstance->GetEnvironmentActorInstance()->AcceptGui(this);
}

void ModelDetailedInformationVisitor::Visit(StaticMeshObjectActor* ActorInstance)
{
    PushID(ActorInstance->GetObjectID().c_str());

    TransformationInformationDrawer TransformDrawer(ActorInstance, nullptr);
    TransformDrawer.DrawInformation();

    StaticMeshObject* StaticMeshObjectInstance = ActorInstance->GetStaticMeshObjectInstance();
    StaticMeshObjectInformationDrawer MeshObjectDrawer(StaticMeshObjectInstance);
    MeshObjectDrawer.DrawInformation();

    if (StaticMeshObjectInstance != nullptr)
    {
        MaterialInformationDrawer MaterialDrawer(StaticMeshObjectInstance);
        MaterialDrawer.DrawInformation();
    }
    PopID();
}

void ModelDetailedInformationVisitor::Visit(SkeletalMeshObjectActor* ActorInstance)
{
    PushID(ActorInstance->GetObjectID().c_str());

    TransformationInformationDrawer TransformDrawer(ActorInstance, nullptr);
    TransformDrawer.DrawInformation();

    SkeletalMeshObject* SkeletalMeshObjectInstance = ActorInstance->GetSkeletalMeshObjectInstance();
    SkeletalMeshObjectInformationDrawer MeshObjectDrawer(SkeletalMeshObjectInstance);
    MeshObjectDrawer.DrawInformation();

    if (SkeletalMeshObjectInstance != nullptr)
    {
        MaterialInformationDrawer MaterialDrawer(SkeletalMeshObjectInstance);
        MaterialDrawer.DrawInformation();
    }
    PopID();
}

void ModelDetailedInformationVisitor::Visit(EnvironmentActor* EnvironmentActorInstance)
{
    PushID(EnvironmentActorInstance->GetObjectID().c_str());

    EnvironementActorDrawer EnvironmentDrawer(EnvironmentActorInstance);
    EnvironmentDrawer.DrawInformation();

    PopID();
}

void ModelDetailedInformationVisitor::Visit(EditorPawn* EditorActorInstance)
{
    PushID(EditorActorInstance->GetObjectID().c_str());

    PopID();
}

void ModelDetailedInformationVisitor::Visit(StaticMeshObject* StaticMeshObjectInstance)
{
    PushID(StaticMeshObjectInstance->GetObjectID().c_str());

    TransformationInformationDrawer TransformDrawer(StaticMeshObjectInstance, StaticMeshObjectInstance->GetParentObject());
    TransformDrawer.DrawInformation();

    StaticMeshObjectInformationDrawer MeshObjectDrawer(StaticMeshObjectInstance);
    MeshObjectDrawer.DrawInformation();

    PopID();
}

void ModelDetailedInformationVisitor::Visit(SkeletalMeshObject* SkeletalMeshObjectInstance)
{
    PushID(SkeletalMeshObjectInstance->GetObjectID().c_str());

    TransformationInformationDrawer TransformDrawer(SkeletalMeshObjectInstance, SkeletalMeshObjectInstance->GetParentObject());
    TransformDrawer.DrawInformation();

    SkeletalMeshObjectInformationDrawer MeshObjectDrawer(SkeletalMeshObjectInstance);
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
