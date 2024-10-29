#pragma once
#include "Delegation.h"

class AManagedByContainer;

typedef std::function<void(AManagedByContainer*)> BeginRemoveHandler;

class AManagedByContainer
{
public:
	Delegation<AManagedByContainer*> BeginRemoveEvent;
};