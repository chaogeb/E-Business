#pragma once

#include "typedef.h"
#include "Database.h"

class SaleController
{
private:
	SQLite::SaleDB db;
public:
	SaleController(SQLite::Database &apdb) : db(apdb) {};

	SaleVec getSales() {
		return db.getSales();
	}

	/*** return true if success, false if SaleID exists */
	bool newSale(const SaleID ID, const int up, const int down) {
		if (db.getSale(ID).isvalid()) {
			return false;
		}
		else {
			db.createSale(ID, up, down);
			return true;
		}
	}

	bool deleteSale(const SaleID ID) {
		db.deleteSale(ID);
		return true;
	}
};