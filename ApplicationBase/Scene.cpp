#include "Scene.h"

Scene::~Scene()
{
	for (AComponent* rootComponent : m_rootComponents)
	{
		delete rootComponent;
	}
}
