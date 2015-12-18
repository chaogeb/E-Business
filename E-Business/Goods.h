#pragma once

#include "typedef.h"
#include "Database.h"

class GoodsController
{
private:
	SQLite::GoodsDB db;
public:
	GoodsController(SQLite::Database &apdb) : db(apdb) {};

	string getGoodName(const GoodsID id) {
		return db.getGood(id).get_Name();
	}

	TypeID getGoodType(const GoodsID id) {
		return db.getGood(id).get_TypeID();
	}

	//string getTypeName(const GoodsID id) {
	//	return db.getGood(id).get_TypeName();
	//}

	//double getDiscount(const GoodsID id) {
	//	return db.getGood(id).get_Discount();
	//}

	bool fetchGood(const GoodsID id) {
		if (db.getGood(id).get_ID() == 0)
			return false;
		else
			return true;
	}

	double getPrice(const GoodsID id) {
		return db.getGood(id).get_Price();
	}

	unsigned int getAmount(const GoodsID id) {
		return db.getGood(id).get_Amount();
	}

	string getDescribe(const GoodsID id) {
		return db.getGood(id).get_Describe();
	}

	GoodsVec getGoods() {
		return db.getGoods();
	}

	/*** return true if success, false if Good exists */
	bool newGood(const GoodsID ID, const string name, const TypeID typeID, const double price, const unsigned int stock, const string describe) {
		if (db.getGood(ID).isvalid()) {
			return false;
		}
		else {
			db.createGood(ID, name, typeID, price, stock, describe);
			return true;
		}
	}

	/*** return true if success, false if GoodsID not exists */
	bool updateGood(const GoodsID ID, const double price, const unsigned int stock) {
		if (db.getGood(ID).isvalid()) {
			db.updateGood(ID, price, stock);
			return true;
		}
		else
			return false;
	}

	bool updateGood(const GoodsID ID, const unsigned int stock) {
		if (db.getGood(ID).isvalid()) {
			db.updateGood(ID, stock);
			return true;
		}
		else
			return false;
	}
};