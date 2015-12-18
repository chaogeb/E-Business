#pragma once

#include <stdexcept>

#include "sqlite3.h"
#include "typedef.h"

using namespace USER;
using namespace GOODS;
using namespace ONSALE;
using namespace SHOPPINGCART;
using namespace ORDER;
using namespace BANKCARD;

namespace SQLite
{
	class Exception : public std::runtime_error
	{
	public:
		explicit Exception(const std::string& aErrorMessage) :
			std::runtime_error(aErrorMessage) {}
	};

	class Database
	{
	public:
		Database();
		Database(const char* apFilename);
		~Database() noexcept; // nothrow
	protected:
		// Shortcut to execute one or multiple SQL statements without results (UPDATE, INSERT, ALTER, COMMIT, CREATE...).
		int exec(const char* apQueries);
		inline int exec(const std::string& aQueries)
		{
			return exec(aQueries.c_str());
		}
		inline void check(const int aRet) const
		{
			if (SQLITE_OK != aRet)
			{
				throw SQLite::Exception(sqlite3_errstr(aRet));
			}
		}
	protected:
		sqlite3 * mpSQLite;
		std::string mFilename;
	};

	class UserDB : public Database
	{
	public:
		UserDB(Database &apdb);
		~UserDB();

		User		getUser(const UserID id);
		User		getUser(const char* username);
		UserVec		getUsers();
		void		updateUser(const UserID ID, const string password);
		void		createUser(const UserID ID, const string username, const string password);
	};

	class GoodTypeDB : public Database
	{
	public:
		GoodTypeDB(Database &apdb);
		~GoodTypeDB();

		GoodType	getGoodType(const UserID id);
		TypeVec		getGoodTypes();
		void		updateGoodType(const TypeID typeID, const double discount);
		void		createGoodType(const TypeID typeID, const string typeName, const double discount);
	};

	class GoodsDB : public GoodTypeDB
	{
	public:
		GoodsDB(Database &apdb);
		~GoodsDB();

		Goods		getGood(const GoodsID id);
		GoodsVec	getGoods();
		void		updateGood(const GoodsID ID, const double price, const unsigned int stock);
		void		updateGood(const GoodsID ID, const unsigned int stock);
		void		createGood(const GoodsID ID, const string name, const TypeID typeID, const double price, const unsigned int stock, const string describe);
		void		deleteGood(const GoodsID ID);
	};

	class SaleDB : public Database
	{
	public:
		SaleDB(Database &apdb);
		~SaleDB();

		Sale		getSale(const UserID id);
		SaleVec		getSales();
		void		createSale(const SaleID ID, const int up, const int down);
		void		deleteSale(const SaleID ID);
	};

	class CartDB : public Database
	{
	public:
		CartDB(Database &apdb);
		~CartDB();

		Cart		getCart(const CartID ID);
		CartVec		getCarts();
		CartVec		getCarts(const UserID userID);
		void		updateCart(const CartID ID, const unsigned int number);
		void		createCart(const CartID ID, const UserID userID, const GoodsID goodID, const unsigned int number);
		void		deleteCart(const CartID ID);
	};

	class OrderDB : public Database
	{
	public:
		OrderDB(Database &apdb);
		~OrderDB();

		Order		getOrder(const OrderID ID);
		OrdersVec	getOrders();
		OrdersVec	getOrders(const UserID userID);
		void		updateOrder(const OrderID ID, const bool paid);
		void		createOrder(const OrderID ID, const UserID userID, const double price, const bool paid);
		void		deleteOrder(const OrderID ID);
	};

	class CardDB : public Database
	{
	public:
		CardDB(Database &apdb);
		~CardDB();

		Card		getCard(const UserID id);
		CardVec		getCards(const UserID userID);
		void		createCard(const CardID ID, const UserID userID, const string cardNo, const string bankName);
		void		deleteCard(const CardID ID);
	};
}	// namespace SQLite