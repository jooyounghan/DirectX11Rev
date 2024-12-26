#pragma once
#include "DBErrorHandler.h"
#include <unordered_map>

class SessionManager;

class SchemaManager : public mysqlx::Schema, public DBErrorHandler
{
public:
	SchemaManager(SessionManager* sessionManager, const std::string& schemaName);
	virtual ~SchemaManager();

protected:
	SessionManager* m_sessionManager;
};

