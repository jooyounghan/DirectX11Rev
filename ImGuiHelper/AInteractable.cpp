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

void AInteractable::BringFrontToInteractionManager(InteractionManager* interactionManager)
{
	interactionManager->BringInteractableToFront(this);
}
