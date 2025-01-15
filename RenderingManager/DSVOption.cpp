#include "DSVOption.h"

void DSVOption::InitializeByOption(ID3D11Resource* resource, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	device->CreateDepthStencilView(resource, NULL, &m_dsv);
}
