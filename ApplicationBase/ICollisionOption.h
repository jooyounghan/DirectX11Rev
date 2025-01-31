#pragma once
class AComponent;
class ICollisionAcceptor;
struct ID3D11Device;
struct ID3D11DeviceContext;

class ICollisionOption
{
public:
	virtual void InsertBVHImpl(ID3D11Device* device, ICollisionAcceptor* acceptor) = 0;
	virtual void RemoveBVHImpl(ICollisionAcceptor* acceptor) = 0;
	virtual void UpdateBVHImpl(ID3D11Device* device, ICollisionAcceptor* accpetor) = 0;
	virtual void OnCollideImpl(AComponent* component) = 0;
};