
#include "CollisionVisitor.h"

using namespace std;

ACollisionVisitor::ACollisionVisitor(ICollisionAcceptor* collisionAcceptor)
	: m_collisionAcceptor(collisionAcceptor)
{
}
