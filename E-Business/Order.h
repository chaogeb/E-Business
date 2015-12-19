#pragma once

#include "typedef.h"
#include "Database.h"

class OrderController
{
private:
	SQLite::OrderDB db;
public:
	OrderController(SQLite::Database &apdb) : db(apdb) {};

	double getPrice(const OrderID ID) {
		return db.getOrder(ID).getPrice();
	}

	OrdersVec getOrders() {
		return db.getOrders();
	}
	OrdersVec getOrders(const UserID userID) {
		return db.getOrders(userID);
	}

	/*** return true if success, false if OrderID exists */
	bool newOrder(const OrderID ID, const UserID userID, const double price, const bool paid = false) {
		if (db.getOrder(ID).isvalid()) {
			return false;
		}
		else {
			db.createOrder(ID, userID, price, paid);
			return true;
		}
	}

	/*** return true if success, false if CartID not exists */
	bool updateOrder(const OrderID ID, const bool paid) {
		if (db.getOrder(ID).isvalid()) {
			db.updateOrder(ID, paid);
			return true;
		}
		else
			return false;
	}

	bool deleteOrder(const OrderID ID) {
		db.deleteOrder(ID);
		return true;
	}
};