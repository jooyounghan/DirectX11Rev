
#include "UAVOption.h"

void UAVOption::InitializeByOption(ID3D11Resource* resource, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	device->CreateUnorderedAccessView(resource, NULL, &m_uav);
}
