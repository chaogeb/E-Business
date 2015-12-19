#pragma once

#include "typedef.h"
#include "Database.h"

class CardController
{
private:
	SQLite::CardDB db;
public:
	CardController(SQLite::Database &apdb) : db(apdb) {};

	/* ªÒ»°ø®∫≈ */
	string getCardNO(CardID id) {
		return db.getCard(id).get_CardNo();
	}

	CardVec getCards() {
		return db.getCards();
	}

	CardVec getCards(UserID userID) {
		return db.getCards(userID);
	}
	
	bool newCard(const CardID ID, const UserID userID, const string cardNum) {
		if (db.getCard(ID).isvalid()) {
			return false;
		}
		else {
			db.createCard(ID, userID, cardNum, "");	// bank name not valid
			return true;
		}
	}
};