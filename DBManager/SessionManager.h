#pragma once
#include "mysqlx/xdevapi.h"
#include "DBErrorHandler.h"
#include <string>
#include <shared_mutex>

class SessionManager : public mysqlx::Session, public DBErrorHandler
{
public:
	SessionManager(const std::string& host, unsigned port, const std::string& user, const char* pwd = NULL);
	virtual ~SessionManager();

protected:
	mysqlx::string m_savePoint;
};

