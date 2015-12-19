#include <iostream>
#include <iomanip>
#include <cstdio>
#include "Database.h"
#include "main.h"

#define CLS			system("cls");
#define PAUSE		system("pause");
#define ERR_MSG		cout << "输入格式错误！请重新输入！\n";
#define FUNC_MSG	cout << "请选择一个功能:\n";

using namespace std;

int main()
{
	while (1)
	{
		welcome();
	}
	return 0;
}

void welcome()
{
	CLS;
	cout << "欢迎使用电商交易平台！" << endl;
	FUNC_MSG;
	cout << "1.已有账户登录\t2.注册新账户\t3.后台管理\t0.退出\n" << ">>";
	char c;
	cin >> c;
	switch (c) {
	case '1':
		sign_in();
		break;
	case '2':
		sign_up();
		break;
	case '3':
		background();
		break;
	case '0':
		exit(0);
	default:
		ERR_MSG;
		PAUSE;
	}
	userID = 0;
}

// 
bool sign_in()
{
	cout << "请输入用户名：\n" << ">>";
	cin >> str;
	if (!fc.FetchUser(str)) {
		cout << "用户不存在，请注册！\n";
		PAUSE;
		return 1;
	}
	cout << "请输入密码：\n" << ">>";
	string pass;
	cin >> pass;
	if (!fc.VerifyPassword(fc.GetUserID(str), pass)) {
		cout << "密码错误，请重新输入！\n";
		PAUSE;
		return 0;
	}
	userID = fc.GetUserID(str);
	cout << "欢迎，" << str << endl;
	PAUSE;
	mainwindow();
	return 1;
}

bool sign_up()
{
	cout << "请输入注册用户名：\n" << ">>";
	cin >> str;
	if (fc.FetchUser(str)) {
		cout << "该用户名已被注册，请重新输入!\n";
		return 0;
	}
	string username = str;
	string password;
	do {
		cout << "请输入密码：" << endl << ">>";
		cin >> password;
		cout << "请再次输入密码：" << endl << ">>";
		cin >> str;
	} while (password != str);
	fc.CreateUser(username, password);
	userID = fc.GetUserID(username);
	cout << "注册成功！" << endl;
	PAUSE;

	mainwindow();

	return 0;
}

void mainwindow()
{
	do {
		CLS;
		FUNC_MSG;
		cout << " 1.购物\t\t2.查看购物车\t3.查看订单\n 0.退出登录\n" << ">>";
		cin >> choice;
		switch (choice) {
		case 1:	// 购物
			cout << "=== 网上商城 ===" << endl;
			fc.ListGoods();
			cout << "请选择商品加入购物车：\n" << ">>";
			cin >> goodID;
			if (!fc.FetchGood(goodID)) {
				cout << "找不到所选商品，请重新输入！\n";
			}
			else {
				/* 检查商品库存，添加至购物车 */
				cout << "商品库存：" << fc.GetGoodStock(goodID) << endl;
				cout << "请输入数量：\n" << ">>";
				unsigned int amount;
				cin >> amount;
				if (amount > fc.GetGoodStock(goodID)) {
					cout << "输入数量过大！\n";
				}
				else {
					fc.AddCart(userID, goodID, amount);
					cout << "成功添加至购物车\n";
				}
			}
			PAUSE;
			break;
		case 2:	// 查看购物车
		{
			int choice;				// 购物车选项
			unsigned int amount;	// 修改购物车数量
			cout << "=== 购物车 ===" << endl;
			fc.ListCarts(userID);
			FUNC_MSG;
			cout << " 1.下单\t2.修改数量\t0.返回\n" << ">>";
			cin >> choice;
			switch (choice) {
			case 1:
				placeorder();
				PAUSE;
				break;
			case 2:
				cout << "请输入购物车记录编号:\n" << ">>";
				cin >> cartID;
				cout << "商品库存：" << fc.GetGoodStockInCart(cartID) << endl;
				cout << "请输入数量(输入0删除记录)：\n" << ">>";
				cin >> amount;
				if (amount > fc.GetGoodStockInCart(cartID)) {
					cout << "输入数量过大！\n";
				}
				else if (amount == 0) {
					fc.DeleteCart(cartID);
					cout << "购物车记录已删除！\n";
				}
				else {
					if (fc.UpdateCart(cartID, amount)) {
						cout << "购物车修改成功！\n";
					}
					else {
						cout << "购物车记录不存在！\n";
					}
				}
				PAUSE;
				break;
			case 0:
				break;
			default:
				ERR_MSG;
				PAUSE;
			}
		}
			break;
		case 3:	// 查看订单
			fc.ListOrders(userID);
			PAUSE;
			break;
		case 0:	// 返回
			break;
		default:
			ERR_MSG;
			PAUSE;
		}
	} while (choice != 0);
}

void background()
{
	string goodname;
	double price;
	unsigned int amount;
	string describe;
	double discount;

	do {
		CLS;
		FUNC_MSG;
		cout << " 1.展示所有商品\t2.修改商品\t3.增加商品\n 4.显示商品种类\t5.增加商品种类\t6.促销管理\n 0.返回\n" << ">>";
		cin >> choice;
		switch (choice) {
		case 1:	// 展示所有商品
			cout << "商品列表：\n";
			fc.ListGoods();
			PAUSE;
			break;
		case 2:	// 修改商品
			fc.ListSales();
			cout << "输入待修改的编号：\n" << ">>";
			cin >> goodID;
			if (!fc.FetchGood(goodID)) {
				cout << "没有所选记录！\n";
			}
			else {
				cout << "价格：\n" << ">>";
				cin >> price;
				cout << "库存量：\n" << ">>";
				cin >> amount;
				fc.ChangeGood(goodID, price, amount);
				cout << "修改成功!\n";
			}
			PAUSE;
			break;
		case 3:	// 增加商品
			cout << "\n增加商品\n" << "请选择商品种类：\n";
			fc.ListGoodTypes();
			cout << ">>";
			cin >> typeID; getchar();
			cout << "请输入产品信息：\n";
			cout << "产品名称：\n" << ">>";
			getline(cin, goodname);
			cout << "价格：\n" << ">>";
			cin >> price;
			cout << "库存量：\n" << ">>";
			cin >> amount; getchar();
			cout << "产品描述：\n" << ">>";
			getline(cin, describe);
			fc.CreateGood(goodname, typeID, price, amount, describe);
			cout << "操作成功！" << endl;
			PAUSE;
			break;
		case 4:	// 显示商品种类
			cout << "商品种类：\n";
			fc.ListGoodTypes();
			PAUSE;
			break;
		case 5:	// 增加商品种类
			cout << "\n增加商品种类\n" << "种类名称：\n" << ">>";
			cin >> str;
			fc.CreateGoodType(str);
			cout << "操作成功！" << endl;
			PAUSE;
			break;
		case 6:	// 促销管理
		{
			unsigned int choice;	// 促销管理选项
			FUNC_MSG;
			cout << " 1.品类折扣\t2.订单满减\t0.返回\n" << ">>";
			cin >> choice;
			switch (choice) {
			case 1:	// 品类折扣
				cout << "品类折扣\n" << "请选择品类：\n";
				fc.ListGoodTypes();
				cout << ">>";
				cin >> typeID;
				cout << "折扣率（小数）：\n" << ">>";
				cin >> discount;
				if (!fc.ChangeTypeDiscount(typeID, discount)) {
					cout << "输入不合法，请检查输入！" << endl;
				}
				else {
					cout << "操作成功！" << endl;
				}
				break;
			case 2:	// 全场满减
				cout << "订单满减列表：\n";
				fc.ListSales();
				cout << " 1.增加订单满减选项\t2.删除记录\t0.返回\n" << ">>";
				cin >> choice;
				if (choice == 1) {
					unsigned int up, down;
					cout << "增加满减：\n 请输入满、减金额：\n" << ">>满";
					cin >> up;
					cout << ">>减";
					cin >> down;
					fc.CreateSale(up, down);
				}
				else if (choice == 2) {
					SaleID id;
					cout << " 请输入待删除的序号：\n" << ">>";
					cin >> id;
					fc.DeleteSale(id);
					cout << "删除成功！\n";
				}
				else if (choice == 0) {
					;	// return
				}
				else {
					ERR_MSG;
				}
				break;
			case 0:	// 返回后台管理
				break;
			default:
				ERR_MSG;
			}
		}
			PAUSE;
			break;
		case 0:	// 返回主界面
			break;
		default:
			ERR_MSG;
			PAUSE;
		}
	} while (choice != 0);
	
}

void placeorder()
{
	CLS;
	cout << "=== 订单 ===" << endl;
	fc.ListCarts(userID);
	cout << "订单总额：￥" << fixed << setprecision(2) << (float)fc.GetCartsPrice(userID) << endl;
	FUNC_MSG;
	cout << " 1.下单\t0.返回\n" << ">>";
	int choice;
	cin >> choice;
	if (choice == 1) {
		if (fc.CheckCart(userID)) {
			orderID = fc.CreateOrder(userID);
			cout << "下单成功！" << endl;
			string cardNo, psw;
			bool paid = false;
			while (!paid) {
				int choice;
				cout << "请选择支付方式：\n 1.选择银行卡支付\t2.绑定新卡支付\n" << ">>";
				cin >> choice;
				if (choice == 1) {	// 选择银行卡支付
					if (fc.ListCards(userID)) {
						do {
							cout << "请输入卡号支付：\n" << ">>";
							cin >> cardNo;
							cout << "请输入密码：\n" << ">>";
							cin >> psw;
						} while (!fc.Client(cardNo, psw, fc.GetOrderPrice(orderID)));
						paid = true;
					}
				}
				else if (choice == 2) {	// 绑定新卡支付
					do {
						cout << "绑定新卡支付 请输入卡号：\n" << ">>";
						cin >> cardNo;
						cout << "请输入密码：\n" << ">>";
						cin >> psw;
					} while (!fc.Client(cardNo, psw));
					fc.AddCard(userID, cardNo);
					if (fc.Client(cardNo, psw, fc.GetOrderPrice(orderID))) {
						paid = true;
					}
					else {
						cout << "支付失败！\n";
						PAUSE;
					}
				}
				else {
					ERR_MSG;
					PAUSE;
				}
			}
			cout << "支付成功！\n";
		}
		else {	// 数量不合法
			cout << "请修改购物车数量！" << endl;
		}
	}
	else if (choice == 0) {
		;	// return
	}
	else {
		ERR_MSG;
	}
}