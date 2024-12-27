#include "pch.h"
#include "SchemaManager.h"

using namespace std;
using namespace mysqlx;

SchemaManager::SchemaManager(SessionManager* sessionManager, const std::string& schemaName)
	: m_sessionManager(sessionManager), Schema(sessionManager->getSchema(schemaName))
{

}

SchemaManager::~SchemaManager()
{
}
