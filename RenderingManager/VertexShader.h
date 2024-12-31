#pragma once
#include "AShader.h"
#include <vector>

class VertexShader : public AShader
{
public:
	VertexShader(
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElementDescs,
		const D3D11_PRIMITIVE_TOPOLOGY& topology
	);
	~VertexShader() override = default;

protected:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	std::vector<D3D11_INPUT_ELEMENT_DESC> m_inputElementDescs;
	D3D11_PRIMITIVE_TOPOLOGY m_topology;

protected:
	virtual void CreateShaderImpl(
		const std::wstring& shaderPath,
		const std::string& entryPoint,
		const std::string& targetVersion,
		ID3D11Device* device
	) override;

public:
	virtual void SetShader(
		ID3D11DeviceContext* deviceContext
	) const override;
	virtual void SetSamplerState(
		ID3D11DeviceContext* deviceContext,
		const std::vector<ID3D11SamplerState*>& samplerStates
	) override;
	virtual void ResetShader(ID3D11DeviceContext* deviceContext) const override;
	virtual void ResetSamplerState(ID3D11DeviceContext* deviceContext) const override;
};

