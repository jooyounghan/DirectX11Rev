#include "RTVOption.h"

void RTVOption::InitializeByOption(
    ID3D11Device* device,
    ID3D11DeviceContext* deviceContext,
    ID3D11Resource* resource
)
{
	device->CreateRenderTargetView(resource, NULL, &m_rtv);
}
