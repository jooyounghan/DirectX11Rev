#pragma once
#include "SessionManager.h"

class SchemaManager : public mysqlx::Schema, public DBErrorHandler
{
public:
	SchemaManager(SessionManager* sessionManager, const std::string& schemaName);
	~SchemaManager() override;

protected:
	SessionManager* m_sessionManager;
};

