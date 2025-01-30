#include "SessionManager.h"

using namespace std;
using namespace mysqlx;

SessionManager::SessionManager(
	const std::string& host, 
	unsigned port, 
	const std::string& user, 
	const char* pwd
)
	: Session(host, port, user, pwd)
{

}

SessionManager::~SessionManager()
{
	this->close();
}