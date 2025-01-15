#include "RTVOption.h"

void RTVOption::InitializeByOption(ID3D11Resource* resource, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	device->CreateRenderTargetView(resource, NULL, &m_rtv);
}
