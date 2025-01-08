
#include "UAVOption.h"

void UAVOption::InitializeByOption(ID3D11Resource* resource, ID3D11Device* device)
{
	device->CreateUnorderedAccessView(resource, NULL, &m_uav);
}
