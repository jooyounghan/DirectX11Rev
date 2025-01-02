#include "VariablePort.h"

std::string GetBaseTypeName(std::string typeName)
{
	size_t pos = typeName.find("class ");
	if (pos != std::string::npos)
	{
		typeName = typeName.substr(pos + 6);
	}

	pos = typeName.find('<');
	if (pos != std::string::npos)
	{
		typeName = typeName.substr(0, pos);
	}

	return typeName;
}
