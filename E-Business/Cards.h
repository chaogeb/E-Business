#pragma once

#include "typedef.h"
#include "Database.h"

class CardController
{
private:
	SQLite::CardDB db;
public:
	CardController(SQLite::Database &apdb) : db(apdb) {};

};