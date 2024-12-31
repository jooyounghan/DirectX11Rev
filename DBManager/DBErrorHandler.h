#pragma once
#include <functional>
#include <string>

class DBErrorHandler
{
public:
	virtual ~DBErrorHandler() = default;

public:
	std::function<void(const std::string&)> OnErrorOccurs = [&](const std::string&) {};
};

