#include "AInteractable.h"
#include "InteractionManager.h"

void AInteractable::RegisterToInteractionManager(InteractionManager* interactionManager)
{
	interactionManager->RegisterInteractable(this);
}

void AInteractable::DeregisterToInteractionManager(InteractionManager* interactionManager)
{
	interactionManager->DeregisterInteractable(this);
}
