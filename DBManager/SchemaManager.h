#pragma once
#include "SessionManager.h"

#include <unordered_map>

class SchemaManager : public mysqlx::Schema, public DBErrorHandler
{
public:
	SchemaManager(SessionManager* sessionManager, const std::string& schemaName);
	virtual ~SchemaManager();

protected:
	SessionManager* m_sessionManager;
};

