#pragma once
class AComponent;
class ICollisionAcceptor;

class ICollisionOption
{
public:
	virtual void InsertBVHImpl(ICollisionAcceptor* acceptor) = 0;
	virtual void RemoveBVHImpl(ICollisionAcceptor* acceptor) = 0;
	virtual void UpdateBVHImpl(ICollisionAcceptor* accpetor) = 0;
	virtual void OnCollideImpl(AComponent* component) = 0;
};