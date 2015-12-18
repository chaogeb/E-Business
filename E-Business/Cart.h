#pragma once

#include "typedef.h"
#include "Database.h"

class CartController
{
private:
	SQLite::CartDB db;
public:
	CartController(SQLite::Database &apdb) : db(apdb) {};

	GoodsID getGoodsID(const CartID ID) {
		return db.getCart(ID).getGoodID();
	}

	unsigned int getNumber(const CartID ID) {
		return db.getCart(ID).getNumber();
	}

	CartVec getCarts() {
		return db.getCarts();
	}
	CartVec getCarts(const UserID userID) {
		return db.getCarts(userID);
	}

	/*** return true if success, false if cartID exists */
	bool newCart(const CartID ID, const UserID userID, const GoodsID goodID, const unsigned int number) {
		if (db.getCart(ID).isvalid()) {
			return false;
		}
		else {
			db.createCart(ID, userID, goodID, number);
			return true;
		}
	}

	/*** return true if success, false if CartID not exists */
	bool updateCart(const CartID ID, const unsigned int number) {
		if (db.getCart(ID).isvalid()) {
			db.updateCart(ID, number);
			return true;
		}
		else
			return false;
	}

	bool deleteCart(const CartID ID) {
		db.deleteCart(ID);
		return true;
	}
};