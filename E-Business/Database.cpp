#include "Database.h"

namespace SQLite
{
	Database::Database() : mpSQLite(NULL), mFilename(NULL) {}

	Database::Database(const char* apFilename) :
		mpSQLite(NULL),
		mFilename(apFilename)
	{
		// 打开数据库, 创建连接
		int ret = sqlite3_open(apFilename, &mpSQLite);

		if (SQLITE_OK != ret)
		{
			sqlite3_close(mpSQLite); // close is required even in case of error on opening
			throw SQLite::Exception(sqlite3_errstr(ret));
		}

		exec("CREATE TABLE IF NOT EXISTS USER\
				('id' int, 'account' varchar(20), 'password' varchar(20))");
		exec("CREATE TABLE IF NOT EXISTS GOOD_TYPE\
				('id' int, 'name' varchar(20), 'discount' double)");
		exec("CREATE TABLE IF NOT EXISTS GOODS\
				('id' int, 'name' text, 'type_ID' int, 'price' double, 'amount' int, 'describe' text)");
		exec("CREATE TABLE IF NOT EXISTS SALE\
				('id' int, 'up' int, 'down' int)");
		exec("CREATE TABLE IF NOT EXISTS CART\
				('id' int, 'user_ID' int, 'good_ID' int, 'number' int)");
		exec("CREATE TABLE IF NOT EXISTS ORDERS\
				('id' int, 'user_ID' int, 'price' double, 'paid' bool)");
		exec("CREATE TABLE IF NOT EXISTS CARD\
				('id' int, 'user_ID' int, 'card_NO' varchar(20), 'bank_name' varchar(20))");
	}

	// Close the SQLite database connection.
	Database::~Database() noexcept // nothrow
	{
		const int ret = sqlite3_close(mpSQLite);

		// Only case of error is SQLITE_BUSY: "database is locked" (some statements are not finalized)
		// Never throw an exception in a destructor :
		if (SQLITE_OK != ret)
		{
			printf("database is locked\n");
			getchar(); exit(-1);
		}
	}

	int Database::exec(const char* apQueries)
	{
		const int ret = sqlite3_exec(mpSQLite, apQueries, NULL, NULL, NULL);
		check(ret);

		// Return the number of rows modified by those SQL statements (INSERT, UPDATE or DELETE only)
		return sqlite3_changes(mpSQLite);
	}



	UserDB::UserDB(Database & apdb) : Database(apdb) {}

	UserDB::~UserDB() { mpSQLite = NULL; }

	int getUser_callback(void *user, int nColumn, char **colValues, char **colNames)
	{
		((User*)user)->setRec(colValues);
		return 0;
	}
	User UserDB::getUser(const UserID id)
	{
		User user;
		char sql[100];
		sprintf(sql, "SELECT * FROM USER WHERE id=%d LIMIT 1", id);
		sqlite3_exec(mpSQLite, sql, &getUser_callback, &user, NULL);

		return user;
	}

	User UserDB::getUser(const char * username)
	{
		User user;
		char sql[100];
		sprintf(sql, "SELECT * FROM USER WHERE account='%s' LIMIT 1", username);
		sqlite3_exec(mpSQLite, sql, &getUser_callback, &user, NULL);

		return user;
	}

	int getUsers_callback(void *userlist, int nColumn, char **colValues, char **colNames)
	{
		User user;
		user.setRec(colValues);
		((UserVec*)userlist)->push_back(user);
		return 0;
	}
	UserVec UserDB::getUsers()
	{
		UserVec userlist;
		char sql[100];
		sprintf(sql, "SELECT * FROM USER");
		sqlite3_exec(mpSQLite, sql, &getUsers_callback, &userlist, NULL);

		return userlist;
	}

	void UserDB::updateUser(const UserID ID, const string password)
	{
		char sql[100];
		sprintf(sql, "UPDATE USER SET password='%s' WHERE id=%d", password.c_str(), ID);
		exec(sql);
	}

	void UserDB::createUser(const UserID ID, const string username, const string password)
	{
		char sql[100];
		sprintf(sql, "INSERT INTO USER VALUES (%d, '%s', '%s')",
			ID, username.c_str(), password.c_str());
		exec(sql);
	}



	GoodTypeDB::GoodTypeDB(Database & apdb) : Database(apdb) {}

	GoodTypeDB::~GoodTypeDB() { mpSQLite = NULL; }

	int getGoodType_callback(void *goodtype, int nColumn, char **colValues, char **colNames)
	{
		((GoodType*)goodtype)->setRec(colValues);
		return 0;
	}
	GoodType GoodTypeDB::getGoodType(const UserID id)
	{
		GoodType goodtype;
		char sql[100];
		sprintf(sql, "SELECT * FROM GOOD_TYPE WHERE id=%d LIMIT 1", id);
		sqlite3_exec(mpSQLite, sql, &getGoodType_callback, &goodtype, NULL);

		return goodtype;
	}

	int getGoodTypes_callback(void *typelist, int nColumn, char **colValues, char **colNames)
	{
		GoodType type;
		type.setRec(colValues);
		((TypeVec*)typelist)->push_back(type);
		return 0;
	}
	TypeVec GoodTypeDB::getGoodTypes()
	{
		TypeVec typelist;
		char sql[100];
		sprintf(sql, "SELECT * FROM GOOD_TYPE");
		sqlite3_exec(mpSQLite, sql, &getGoodTypes_callback, &typelist, NULL);

		return typelist;
	}

	void GoodTypeDB::updateGoodType(const TypeID typeID, const double discount)
	{
		char sql[100];
		sprintf(sql, "UPDATE GOOD_TYPE SET discount=%f WHERE id=%d", discount, typeID);
		exec(sql);
	}

	void GoodTypeDB::createGoodType(const TypeID typeID, const string typeName, const double discount)
	{
		char sql[200];
		sprintf(sql, "INSERT INTO GOOD_TYPE VALUES (%d, '%s', %f)",
			typeID, typeName.c_str(), discount);
		exec(sql);
	}



	GoodsDB::GoodsDB(Database & apdb) : GoodTypeDB(apdb) {}

	GoodsDB::~GoodsDB() { mpSQLite = NULL; }

	int getGood_callback(void *good, int nColumn, char **colValues, char **colNames)
	{
		((Goods*)good)->setRec(colValues);
		return 0;
	}
	Goods GoodsDB::getGood(const GoodsID id)
	{
		Goods good;
		GoodType goodtype;

		char sql[100];
		sprintf(sql, "SELECT * FROM GOODS WHERE id=%d LIMIT 1", id);
		sqlite3_exec(mpSQLite, sql, &getGood_callback, &good, NULL);
		
		sprintf(sql, "SELECT * FROM GOOD_TYPE WHERE id=%d LIMIT 1", good.get_TypeID());
		sqlite3_exec(mpSQLite, sql, &getGoodType_callback, &goodtype, NULL);

		good.setType(goodtype);

		return good;
	}

	int getGoods_callback(void *goodslist, int nColumn, char **colValues, char **colNames)
	{
		Goods good;
		good.setRec(colValues);
		((GoodsVec*)goodslist)->push_back(good);
		return 0;
	}
	GoodsVec GoodsDB::getGoods()
	{
		GoodsVec goodslist;
		char sql[100];
		sprintf(sql, "SELECT * FROM GOODS");
		sqlite3_exec(mpSQLite, sql, &getGoods_callback, &goodslist, NULL);

		//for each (Goods &good in goodslist) {
		//	GoodType goodtype;
		//	sprintf(sql, "SELECT * FROM GOOD_TYPE WHERE id=%d LIMIT 1", good.get_TypeID());
		//	sqlite3_exec(mpSQLite, sql, &getGoodType_callback, &goodtype, NULL);

		//	good.setType(goodtype);
		//}
		for (unsigned int i = 0; i < goodslist.size(); i++) {
			GoodType goodtype;
			sprintf(sql, "SELECT * FROM GOOD_TYPE WHERE id=%d LIMIT 1", goodslist.at(i).get_TypeID());
			sqlite3_exec(mpSQLite, sql, &getGoodType_callback, &goodtype, NULL);

			goodslist.at(i).setType(goodtype);
		}
		
		return goodslist;
	}

	void GoodsDB::updateGood(const GoodsID ID, const double price, const unsigned int stock)
	{
		char sql[100];
		sprintf(sql, "UPDATE GOODS SET price=%f,amount=%d WHERE id=%d", price, stock, ID);
		exec(sql);
	}

	void GoodsDB::updateGood(const GoodsID ID, const unsigned int stock)
	{
		char sql[100];
		sprintf(sql, "UPDATE GOODS SET amount=%d WHERE id=%d", stock, ID);
		exec(sql);
	}
	void GoodsDB::createGood(const GoodsID ID, const string name, const TypeID typeID, const double price, const unsigned int stock, const string describe)
	{
		char sql[500];
		sprintf(sql, "INSERT INTO GOODS VALUES (%d, '%s', %d, %f, %d, '%s')",
			ID, name.c_str(), typeID, price, stock, describe.c_str());
		exec(sql);
	}

	void GoodsDB::deleteGood(const GoodsID ID)
	{
		char sql[100];
		sprintf(sql, "DELETE FROM GOODS WHERE id=%d", ID);
		exec(sql);
	}



	SaleDB::SaleDB(Database & apdb) : Database(apdb) {}

	SaleDB::~SaleDB() { mpSQLite = NULL; }

	int getSale_callback(void *sale, int nColumn, char **colValues, char **colNames)
	{
		((Sale*)sale)->setRec(colValues);
		return 0;
	}
	Sale SaleDB::getSale(const UserID id)
	{
		Sale sale;
		char sql[100];
		sprintf(sql, "SELECT * FROM SALE WHERE id=%d LIMIT 1", id);
		sqlite3_exec(mpSQLite, sql, &getSale_callback, &sale, NULL);

		return sale;
	}

	int getSales_callback(void *salelist, int nColumn, char **colValues, char **colNames)
	{
		Sale sale;
		sale.setRec(colValues);
		((SaleVec*)salelist)->push_back(sale);
		return 0;
	}
	SaleVec SaleDB::getSales()
	{
		SaleVec salelist;
		char sql[100];
		sprintf(sql, "SELECT * FROM SALE");
		sqlite3_exec(mpSQLite, sql, &getSales_callback, &salelist, NULL);

		return salelist;
	}

	void SaleDB::createSale(const SaleID ID, const int up, const int down)
	{
		char sql[100];
		sprintf(sql, "INSERT INTO SALE VALUES (%d, %d, %d)",
			ID, up, down);
		exec(sql);
	}

	void SaleDB::deleteSale(const SaleID ID)
	{
		char sql[100];
		sprintf(sql, "DELETE FROM SALE WHERE id=%d", ID);
		exec(sql);
	}



	CartDB::CartDB(Database & apdb) : Database(apdb) {}

	CartDB::~CartDB() { mpSQLite = NULL; }

	int getCart_callback(void *cart, int nColumn, char **colValues, char **colNames)
	{
		((Cart*)cart)->setRec(colValues);
		return 0;
	}
	Cart CartDB::getCart(const CartID id)
	{
		Cart cart;
		char sql[100];
		sprintf(sql, "SELECT * FROM CART WHERE id=%d LIMIT 1", id);
		sqlite3_exec(mpSQLite, sql, &getCart_callback, &cart, NULL);

		return cart;
	}

	int getCarts_callback(void *cartlist, int nColumn, char **colValues, char **colNames)
	{
		Cart cart;
		cart.setRec(colValues);
		((CartVec*)cartlist)->push_back(cart);
		return 0;
	}
	CartVec CartDB::getCarts()
	{
		CartVec cartlist;
		char sql[100];
		sprintf(sql, "SELECT * FROM CART");
		sqlite3_exec(mpSQLite, sql, &getCarts_callback, &cartlist, NULL);

		return cartlist;
	}
	CartVec CartDB::getCarts(const UserID userID)
	{
		CartVec cartlist;
		char sql[100];
		sprintf(sql, "SELECT * FROM CART WHERE user_ID=%d", userID);
		sqlite3_exec(mpSQLite, sql, &getCarts_callback, &cartlist, NULL);

		return cartlist;
	}

	void CartDB::updateCart(const CartID ID, const unsigned int number)
	{
		char sql[100];
		sprintf(sql, "UPDATE CART SET number=%d WHERE id=%d", number, ID);
		exec(sql);
	}

	void CartDB::createCart(const CartID ID, const UserID userID, const GoodsID goodID, const unsigned int number)
	{
		char sql[100];
		sprintf(sql, "INSERT INTO CART VALUES (%d, %d, %d, %d)",
			ID, userID, goodID, number);
		exec(sql);
	}

	void CartDB::deleteCart(const CartID ID)
	{
		char sql[100];
		sprintf(sql, "DELETE FROM CART WHERE id=%d", ID);
		exec(sql);
	}



	CardDB::CardDB(Database & apdb) : Database(apdb) {}

	CardDB::~CardDB() { mpSQLite = NULL; }

	int getCard_callback(void *card, int nColumn, char **colValues, char **colNames)
	{
		((Card*)card)->setRec(colValues);
		return 0;
	}
	Card CardDB::getCard(const CardID id)
	{
		Card card;
		char sql[100];
		sprintf(sql, "SELECT * FROM CARD WHERE id=%d LIMIT 1", id);
		sqlite3_exec(mpSQLite, sql, &getCard_callback, &card, NULL);

		return card;
	}

	int getCards_callback(void *cardlist, int nColumn, char **colValues, char **colNames)
	{
		Card card;
		card.setRec(colValues);
		((CardVec*)cardlist)->push_back(card);
		return 0;
	}
	CardVec CardDB::getCards()
	{
		CardVec cardlist;
		char sql[100];
		sprintf(sql, "SELECT * FROM CARD");
		sqlite3_exec(mpSQLite, sql, &getCards_callback, &cardlist, NULL);

		return cardlist;
	}
	CardVec CardDB::getCards(const UserID userID)
	{
		CardVec cardlist;
		char sql[100];
		sprintf(sql, "SELECT * FROM CARD WHERE user_ID=%d", userID);
		sqlite3_exec(mpSQLite, sql, &getCards_callback, &cardlist, NULL);

		return cardlist;
	}

	void CardDB::createCard(const CardID ID, const UserID userID, const string cardNo, const string bankName)
	{
		char sql[200];
		sprintf(sql, "INSERT INTO CARD VALUES (%d, %d, '%s', '%s')",
			ID, userID, cardNo.c_str(), bankName.c_str());
		exec(sql);
	}

	void CardDB::deleteCard(const CardID ID)
	{
		char sql[100];
		sprintf(sql, "DELETE FROM CARD WHERE id=%d", ID);
		exec(sql);
	}


	OrderDB::OrderDB(Database & apdb) : Database(apdb) {}

	OrderDB::~OrderDB() { mpSQLite = NULL; }

	int getOrder_callback(void *order, int nColumn, char **colValues, char **colNames)
	{
		((Order*)order)->setRec(colValues);
		return 0;
	}
	Order OrderDB::getOrder(const OrderID ID)
	{
		Order order;
		char sql[100];
		sprintf(sql, "SELECT * FROM ORDERS WHERE id=%d LIMIT 1", ID);
		sqlite3_exec(mpSQLite, sql, &getOrder_callback, &order, NULL);

		return order;
	}

	int getOrders_callback(void *orderslist, int nColumn, char **colValues, char **colNames)
	{
		Order order;
		order.setRec(colValues);
		((OrdersVec*)orderslist)->push_back(order);
		return 0;
	}
	OrdersVec OrderDB::getOrders()
	{
		OrdersVec orderslist;
		char sql[100];
		sprintf(sql, "SELECT * FROM ORDERS");
		sqlite3_exec(mpSQLite, sql, &getOrders_callback, &orderslist, NULL);

		return orderslist;
	}
	OrdersVec OrderDB::getOrders(const UserID userID)
	{
		OrdersVec orderslist;
		char sql[100];
		sprintf(sql, "SELECT * FROM ORDERS WHERE user_ID=%d", userID);
		sqlite3_exec(mpSQLite, sql, &getOrders_callback, &orderslist, NULL);

		return orderslist;
	}

	void OrderDB::updateOrder(const OrderID ID, const bool paid)
	{
		char sql[100];
		sprintf(sql, "UPDATE ORDERS SET paid=%d WHERE id=%d", paid, ID);
		exec(sql);
	}

	void OrderDB::createOrder(const OrderID ID, const UserID userID, const double price, const bool paid)
	{
		char sql[200];
		sprintf(sql, "INSERT INTO ORDERS VALUES (%d, %d, %f, %d)",
			ID, userID, price, paid);
		exec(sql);
	}

	void OrderDB::deleteOrder(const OrderID ID)
	{
		char sql[100];
		sprintf(sql, "DELETE FROM ORDERS WHERE id=%d", ID);
		exec(sql);
	}
}