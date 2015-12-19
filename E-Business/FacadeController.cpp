#include "FacadeController.h"

FacadeController::FacadeController() :
	dbCon(DATABASE_NAME),
	userCon(dbCon),
	typeCon(dbCon),
	goodCon(dbCon),
	saleCon(dbCon),
	cartCon(dbCon),
	orderCon(dbCon),
	cardCon(dbCon)
{
}

void FacadeController::CreateUser(const string username, const string password)
{
	UserVec uv = userCon.getUsers();
	UserID id;
	if (uv.size() > 0) {
		id = uv.at(uv.size() - 1).get_ID() + 1;
	}
	else id = 1;

	if (userCon.newUser(id, username.c_str(), password.c_str())) {
		printf("User record added successfully!\n");
	}
	else {
		printf("User record exists!\n");
	}
}

bool FacadeController::FetchUser(const string username)
{
	if (GetUserID(username) > 0)
		return true;
	else
		return false;
}

UserID FacadeController::GetUserID(const string username)
{
	return userCon.getUserID(username.c_str());
}

bool FacadeController::ChangePassword(const UserID id, const string newpassword)
{
	return userCon.updatePassword(id, newpassword.c_str());
}

bool FacadeController::VerifyPassword(const UserID id, const string inpass)
{
	return userCon.checkPassword(id, inpass);
}

void FacadeController::CreateGoodType(const string name)
{
	TypeVec tv = typeCon.getTypes();
	TypeID id;
	if (tv.size() > 0) {
		id = tv.at(tv.size() - 1).get_ID() + 1;
	}
	else id = 1;

	if (typeCon.newType(id, name.c_str(), 1)) {
		printf("Goods Type record added successfully!\n");
	}
	else {
		printf("Good Type ID exists!\n");
	}
}

void FacadeController::ListGoodTypes()
{
	TypeVec tv = typeCon.getTypes();
	for each (GoodType type in tv) {
		printf(" %d.%28s\tdiscount : %.02f%%\n", type.get_ID(), type.get_TypeName().c_str(), type.get_Discount() * 100);
	}
}

bool FacadeController::ChangeTypeDiscount(const TypeID id, const double discount)
{
	return typeCon.updateDiscount(id, discount);
}

void FacadeController::CreateSale(const unsigned int up, const unsigned int down)
{
	SaleVec sv = saleCon.getSales();
	SaleID id;
	if (sv.size() > 0) {
		id = sv.at(sv.size() - 1).get_ID() + 1;
	}
	else id = 1;

	if (saleCon.newSale(id, up, down)) {
		printf("Sales record added successfully!\n");
	}
	else {
		printf("Sales ID exists!\n");
	}
}

void FacadeController::ListSales()
{
	SaleVec sv = saleCon.getSales();
	for each (Sale sale in sv) {
		printf(" %d.\t满 %d￥\t减 %d￥\n", sale.get_ID(), sale.getUp(), sale.getDown());
	}
}

void FacadeController::DeleteSale(const SaleID id)
{
	saleCon.deleteSale(id);
}

void FacadeController::CreateGood(const string name, const TypeID typeID, const double price, const unsigned int stock, const string describe)
{
	GoodsVec gv = goodCon.getGoods();
	GoodsID id;
	if (gv.size() > 0) {
		id = gv.at(gv.size() - 1).get_ID() + 1;
	}
	else id = 1;

	if (goodCon.newGood(id, name.c_str(), typeID, price, stock, describe)) {
		printf("Goods record added successfully!\n");
	}
	else {
		printf("Good ID exists!\n");
	}
}

bool FacadeController::ChangeGood(const GoodsID ID, const double price, const unsigned int stock)
{
	return goodCon.updateGood(ID, price, stock);
}

bool FacadeController::DecreaseGood(const GoodsID ID, const unsigned int decNum)
{
	int num = goodCon.getAmount(ID) - decNum;
	if (num >= 0) {
		goodCon.updateGood(ID, num);
		return true;
	}
	else
		return false;
	
}

bool FacadeController::FetchGood(const GoodsID ID)
{
	return goodCon.fetchGood(ID);
}

unsigned int FacadeController::GetGoodStock(const GoodsID ID)
{
	return goodCon.getAmount(ID);
}

void FacadeController::ListGoods()
{
	GoodsVec gv = goodCon.getGoods();
	for each (Goods good in gv) {
		printf(">>  %d. \t%s\n\t\t%s \t￥%.02f\t %d in stock\n\t\t%s\n\n", 
			good.get_ID(), good.get_Name().c_str(), typeCon.getTypeName(good.get_TypeID()).c_str(),
			good.get_Price(), good.get_Amount(), good.get_Describe().c_str());
	}
}

void FacadeController::AddCard(const UserID userID, const string cardNo)
{
	CardVec cv = cardCon.getCards();
	CardID id;
	if (cv.size() > 0) {
		id = cv.at(cv.size() - 1).get_ID() + 1;
	}
	else id = 1;

	if (cardCon.newCard(id, userID, cardNo)) {
		printf("Card added successfully!\n");
	}
	else {
		printf("Card ID exists!\n");
	}
}

bool FacadeController::ListCards(const UserID userID)
{
	CardVec cv = cardCon.getCards(userID);
	if (cv.size() == 0) {
		printf("No card founded!\n");
		return false;
	}
	for each (Card card in cv) {
		printf(">>  %d. \t%s\n", card.get_ID(), card.get_CardNo().c_str());
	}
	return true;
}

void FacadeController::ListCarts(const UserID userid)
{
	for each (Cart cart in cartCon.getCarts(userid)) {
		printf(">>  %d. \t%s \t￥%.02f\n\t\t%d in cart\n\n",
			cart.get_ID(), goodCon.getGoodName(cart.getGoodID()).c_str(),
			goodCon.getPrice(cart.getGoodID()), cart.getNumber());
	}
}

double FacadeController::GetCartsPrice(const UserID userid)
{
	double price = 0;
	for each (Cart cart in cartCon.getCarts(userid)) {
		GoodsID goodid = cart.getGoodID();
		price += goodCon.getPrice(goodid) * cart.getNumber();
	}
	return price;
}

void FacadeController::AddCart(const UserID userID, const GoodsID goodID, const unsigned int number = 0)
{
	CartVec cv = cartCon.getCarts();
	CartID id;
	if (cv.size() > 0) {
		id = cv.at(cv.size() - 1).get_ID() + 1;
	}
	else id = 1;

	if (cartCon.newCart(id, userID, goodID, number)) {
		printf("Cart record added successfully!\n");
	}
	else {
		printf("Cart ID exists!\n");
	}
}

unsigned int FacadeController::GetGoodStockInCart(const CartID cartID)
{
	GoodsID goodID = cartCon.getGoodsID(cartID);
	return goodCon.getAmount(goodID);
}

bool FacadeController::CheckCart(const UserID userid)
{
	bool ans = true;
	for each (Cart cart in cartCon.getCarts(userid)) {
		int incart = cart.getNumber();
		int remain = goodCon.getAmount(cart.getGoodID());
		GoodsID goodID = cart.getGoodID();
		if (incart > remain) {	// number illegal
			printf(" 序号 %d 商品数量超过 库存量 %d\n", cart.get_ID(), remain);
			ans = false;
		}
	}
	return ans;
}

bool FacadeController::UpdateCart(const CartID cartID, const unsigned int number)
{
	return cartCon.updateCart(cartID, number);
}

void FacadeController::DeleteCart(const CartID ID)
{
	cartCon.deleteCart(ID);
}

void FacadeController::ClearCart(const UserID userid)
{
	for each (Cart cart in cartCon.getCarts(userid)) {
		cartCon.deleteCart(cart.get_ID());
	}
}

OrderID FacadeController::CreateOrder(const UserID userid)
{
	OrdersVec ov = orderCon.getOrders();
	OrderID id = 0;
	if (ov.size() > 0) {
		id = ov.at(ov.size() - 1).get_ID() + 1;
	}
	else id = 1;

	if (orderCon.newOrder(id, userid, GetCartsPrice(userid))) {
		// 生成订单
		for each (Cart cart in cartCon.getCarts(userid)) {
			// 减少库存数量
			DecreaseGood(cart.getGoodID(), cart.getNumber());
		}
		printf("Order record added successfully!\n");
		ClearCart(userid);	// 清空购物车
	}
	else {
		printf("Order ID exists!\n");
	}
	return id;
}

double FacadeController::GetOrderPrice(const OrderID id)
{
	return orderCon.getPrice(id);
}

void FacadeController::PaidOrder(const OrderID id)
{
	orderCon.updateOrder(id, true);
}

void FacadeController::ListOrders(const UserID userid)
{
	OrdersVec ov = orderCon.getOrders(userid);
	for each (Order order in ov) {
		printf(" %d. 订单金额：￥%.02f %s\n", order.get_ID(), order.getPrice(), order.isPaid() ? "已支付" : "未付款");
	}
}
