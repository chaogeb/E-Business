#pragma once

#include "typedef.h"
#include "Database.h"

class GoodTypeController
{
private:
	SQLite::GoodTypeDB db;
public:
	GoodTypeController(SQLite::Database &apdb) : db(apdb) {};

	string getTypeName(const TypeID typeID) {
		return db.getGoodType(typeID).get_TypeName();
	}

	double getDiscount(const TypeID typeID) {
		return db.getGoodType(typeID).get_Discount();
	}

	TypeVec getTypes() {
		return db.getGoodTypes();
	}

	/*** return true if success, false if user exists */
	bool newType(const TypeID typeID, const char* typeName, const double discount) {
		if (db.getGoodType(typeID).isAllType()) {
			db.createGoodType(typeID, typeName, discount);
			return true;
		}
		else
			return false;
	}

	/*** return true if success, false if typeID not exists */
	bool updateDiscount(const TypeID typeID, const double discount) {
		if (db.getGoodType(typeID).isAllType()) {
			return false;
		}
		else {
			db.updateGoodType(typeID, discount);
			return true;
		}
	}
};