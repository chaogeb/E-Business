#pragma once

#include "typedef.h"
#include "Database.h"

class UserController
{
private:
	SQLite::UserDB db;
public:
	UserController(SQLite::Database &apdb) : db(apdb) {};

	UserID getUserID(const char* username) {
		return db.getUser(username).get_ID();
	}

	bool checkPassword(const UserID id, const string inpass) {
		return db.getUser(id).checkPassword(inpass);
	}

	UserVec getUsers() {
		return db.getUsers();
	}

	/*** return true if success, false if user exists */
	bool newUser(const UserID id, const char* username, const char* password) {
		if (db.getUser(id).isvalid()) {
			return false;
		}
		else {
			db.createUser(id, username, password);
			return true;
		}
	}

	/*** return true if success, false if CustomerID not exists */
	bool updatePassword(const UserID id, const char* password) {
		if (db.getUser(id).isvalid()) {
			db.updateUser(id, password);
			return true;
		}
		else
			return false;
	}
};