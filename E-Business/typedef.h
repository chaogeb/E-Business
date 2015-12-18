#pragma once

#include <string>
#include <vector>

#define DATABASE_NAME "E-Business.db"

using namespace std;

typedef unsigned int UserID;
typedef unsigned int GoodsID;
typedef unsigned int SaleID;
typedef unsigned int TypeID;
typedef unsigned int CartID;
typedef unsigned int OrderID;
typedef unsigned int CardID;

namespace USER
{
	class User
	{
	protected:
		UserID ID = 0;
		string username;
		string password;
	public:
		void setRec(char **values) {
			ID			= atoi(values[0]);
			username	= values[1];
			password	= values[2];
		}
		/*** return true if record exists */
		bool isvalid() { return ID > 0; }
		UserID get_ID() { return ID; }
		string get_Username() { return username; }
		bool checkPassword(const string inPass) { return password == inPass; }
	};

	typedef std::vector<User> UserVec;
}	// namespace USER

namespace GOODS
{
	class GoodType
	{
	protected:
		TypeID typeID = 0;
		string typeName;
		double discount;	// 品类打折
	public:
		void setRec(char **values) {
			typeID		= atoi(values[0]);
			typeName	= values[1];
			discount	= atof(values[2]);
		}
		/*** return true if is all type sale */
		bool isAllType() { return typeID == 0; }
		TypeID get_ID() { return typeID; }
		string get_TypeName() { return typeName; }
		double get_Discount() { return discount; }
	};

	typedef std::vector<GoodType> TypeVec;


	class Goods : public GoodType
	{
	protected:
		GoodsID ID = 0;
		string name;
		double price;
		unsigned int stock;	// 数量
		string describe;
	public:
		void setRec(char **values) {
			ID		= atoi(values[0]);
			name	= values[1];
			typeID	= atoi(values[2]);
			price	= atof(values[3]);
			stock	= atoi(values[4]);
			describe = values[5];
		}
		void setType(GoodType goodType) {
			typeName = goodType.get_TypeName();
			discount = goodType.get_Discount();
		}
		/*** return true if record exists */
		bool isvalid() { return ID > 0; }
		GoodsID get_ID() { return ID; }
		string get_Name() { return name; }
		TypeID get_TypeID() { return typeID; }
		double get_Price() { return price * discount; }
		unsigned int get_Amount() { return stock; }
		string get_Describe() { return describe; }
	};

	typedef std::vector<Goods> GoodsVec;
}	// namespace GOODS

namespace ONSALE
{
	class Sale
	{
	protected:
		SaleID ID = 0;
		int up;	// 买‘满’价格
		int down;	// 满‘减’额度
	public:
		void setRec(char **values) {
			ID		= atoi(values[0]);
			up		= atoi(values[1]);
			down	= atoi(values[2]);
		}
		/*** return true if record exists */
		bool isvalid() { return ID > 0; }
		SaleID get_ID() { return ID; }
		int getUp() { return up; }
		int getDown() { return down; }
	};

	typedef std::vector<Sale> SaleVec;
}	// namespace ONSALE

namespace SHOPPINGCART
{
	class Cart
	{
	protected:
		CartID ID = 0;
		UserID userID;
		GoodsID goodID;
		unsigned int number;
	public:
		void setRec(char **values) {
			ID = atoi(values[0]);
			userID = atoi(values[1]);
			goodID = atoi(values[2]);
			number = atoi(values[3]);
		}
		/*** return true if record exists */
		bool isvalid() { return ID > 0; }
		CartID get_ID() { return ID; }
		GoodsID getGoodID() { return goodID; }
		unsigned int getNumber() { return number; }

	};

	typedef std::vector<Cart> CartVec;
}	// namespace SHOPPINGCART

namespace ORDER
{
	class Order
	{
	protected:
		OrderID ID = 0;
		UserID userID;
		double price;
		bool paid = false;
	public:
		void setRec(char **values) {
			ID = atoi(values[0]);
			userID = atoi(values[1]);
			price = atof(values[2]);
			paid = (bool)atoi(values[3]);
		}
		/*** return true if record exists */
		bool isvalid() { return ID > 0; }
		OrderID get_ID() { return ID; }
		UserID getUserID() { return userID; }
		double getPrice() { return price; }
		bool isPaid() { return paid; }
	};

	typedef std::vector<Order> OrdersVec;
}	// namespace ORDER

namespace BANKCARD
{
	class Card
	{
	protected:
		CardID ID = 0;
		UserID userID;
		string cardNo;
		string bankName;
	public:
		void setRec(char **values) {
			ID		= atoi(values[0]);
			userID	= atoi(values[1]);
			cardNo	= values[2];
			bankName = values[3];
		}
		/*** return true if record exists */
		bool isvalid() { return ID > 0; }
		CardID get_ID() { return ID; }
	};

	typedef std::vector<Card> CardVec;
}	// namespace BANKCARD