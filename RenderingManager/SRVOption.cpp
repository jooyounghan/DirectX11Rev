
#include "SRVOption.h"

void SRVOption::InitializeByOption(ID3D11Resource* resource, ID3D11Device* device)
{
	device->CreateShaderResourceView(resource, NULL, &m_srv);
}
