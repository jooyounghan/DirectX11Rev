#pragma once
#include <functional>
#include <string>

class DBErrorHandler
{
public:
	std::function<void(const std::string&)> OnErrorOccurs = [&](const std::string&) {};
};

