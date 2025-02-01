#pragma once
#include "DBErrorHandler.h"

#include <string>
#include <shared_mutex>
#include <mysqlx/xdevapi.h>

class SessionManager : public mysqlx::Session, public DBErrorHandler
{
public:
	SessionManager(const std::string& host, unsigned port, const std::string& user, const char* pwd = NULL);
	~SessionManager() override;

protected:
	mysqlx::string m_savePoint;
};

