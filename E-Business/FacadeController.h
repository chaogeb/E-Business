#pragma once

#include "User.h"
#include "GoodType.h"
#include "Goods.h"
#include "Sale.h"
#include "Cart.h"
#include "Order.h"
#include "Cards.h"
#include "Database.h"

class FacadeController
{
private:
	SQLite::Database dbCon;
	UserController userCon;
	GoodTypeController typeCon;
	GoodsController goodCon;
	SaleController saleCon;
	CartController cartCon;
	OrderController orderCon;
	CardController cardCon;
public:
	FacadeController();

	void CreateUser(const string username, const string password);
	bool FetchUser(const string username);
	UserID GetUserID(const string username);
	bool ChangePassword(const UserID id, const string newpassword);
	bool VerifyPassword(const UserID id, const string inpass);

	void CreateGoodType(const string name);
	void ListGoodTypes();
	bool ChangeTypeDiscount(const TypeID id, const double discount);

	void CreateSale(const unsigned int up, const unsigned int down);
	void ListSales();
	void DeleteSale(const SaleID id);

	void CreateGood(const string name, const TypeID typeID, const double price, const unsigned int stock, const string describe);
	bool ChangeGood(const GoodsID ID, const double price, const unsigned int stock);
	bool DecreaseGood(const GoodsID ID, const unsigned int decNum);
	bool FetchGood(const GoodsID ID);
	unsigned int GetGoodStock(const GoodsID ID);
	void ListGoods();



	/* 显示用户 userid 的购物车内商品 */
	void ListCarts(const UserID userid);

	/* 返回用户 userid 购物车内商品总价 */
	double GetCartsPrice(const UserID userid);
	
	void AddCart(const UserID userID, const GoodsID goodID, const unsigned int number);
	
	/* 获取购物车内物品的库存量 */
	unsigned int GetGoodStockInCart(const CartID cartID);

	/* 检查购物车物品数量是否合法 */
	bool CheckCart(const UserID userid);
	
	bool UpdateCart(const CartID cartID, const unsigned int number);
	
	void DeleteCart(const CartID ID);

	/* 清空购物车 */
	void ClearCart(const UserID userid);


	/* 生成订单，清空购物车，修改商品余量 */
	void CreateOrder(const UserID userid);

	void ListOrders(const UserID userid);
};