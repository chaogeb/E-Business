#include <iostream>
#include <iomanip>
#include <cstdio>
#include "Database.h"
#include "main.h"

#define CLS			system("cls");
#define PAUSE		system("pause");
#define ERR_MSG		cout << "�����ʽ�������������룡\n";
#define FUNC_MSG	cout << "��ѡ��һ������:\n";

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
	cout << "��ӭʹ�õ��̽���ƽ̨��" << endl;
	FUNC_MSG;
	cout << "1.�����˻���¼\t2.ע�����˻�\t3.��̨����\t0.�˳�\n" << ">>";
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
	cout << "�������û�����\n" << ">>";
	cin >> str;
	if (!fc.FetchUser(str)) {
		cout << "�û������ڣ���ע�ᣡ\n";
		PAUSE;
		return 1;
	}
	cout << "���������룺\n" << ">>";
	string pass;
	cin >> pass;
	if (!fc.VerifyPassword(fc.GetUserID(str), pass)) {
		cout << "����������������룡\n";
		PAUSE;
		return 0;
	}
	userID = fc.GetUserID(str);
	cout << "��ӭ��" << str << endl;
	PAUSE;
	mainwindow();
	return 1;
}

bool sign_up()
{
	cout << "������ע���û�����\n" << ">>";
	cin >> str;
	if (fc.FetchUser(str)) {
		cout << "���û����ѱ�ע�ᣬ����������!\n";
		return 0;
	}
	string username = str;
	string password;
	do {
		cout << "���������룺" << endl << ">>";
		cin >> password;
		cout << "���ٴ��������룺" << endl << ">>";
		cin >> str;
	} while (password != str);
	fc.CreateUser(username, password);
	userID = fc.GetUserID(username);
	cout << "ע��ɹ���" << endl;
	PAUSE;

	mainwindow();

	return 0;
}

void mainwindow()
{
	do {
		CLS;
		FUNC_MSG;
		cout << " 1.����\t\t2.�鿴���ﳵ\t3.�鿴����\n 0.�˳���¼\n" << ">>";
		cin >> choice;
		switch (choice) {
		case 1:	// ����
			cout << "=== �����̳� ===" << endl;
			fc.ListGoods();
			cout << "��ѡ����Ʒ���빺�ﳵ��\n" << ">>";
			cin >> goodID;
			if (!fc.FetchGood(goodID)) {
				cout << "�Ҳ�����ѡ��Ʒ�����������룡\n";
			}
			else {
				/* �����Ʒ��棬��������ﳵ */
				cout << "��Ʒ��棺" << fc.GetGoodStock(goodID) << endl;
				cout << "������������\n" << ">>";
				unsigned int amount;
				cin >> amount;
				if (amount > fc.GetGoodStock(goodID)) {
					cout << "������������\n";
				}
				else {
					fc.AddCart(userID, goodID, amount);
					cout << "�ɹ���������ﳵ\n";
				}
			}
			PAUSE;
			break;
		case 2:	// �鿴���ﳵ
		{
			int choice;				// ���ﳵѡ��
			unsigned int amount;	// �޸Ĺ��ﳵ����
			cout << "=== ���ﳵ ===" << endl;
			fc.ListCarts(userID);
			FUNC_MSG;
			cout << " 1.�µ�\t2.�޸�����\t0.����\n" << ">>";
			cin >> choice;
			switch (choice) {
			case 1:
				placeorder();
				PAUSE;
				break;
			case 2:
				cout << "�����빺�ﳵ��¼���:\n" << ">>";
				cin >> cartID;
				cout << "��Ʒ��棺" << fc.GetGoodStockInCart(cartID) << endl;
				cout << "����������(����0ɾ����¼)��\n" << ">>";
				cin >> amount;
				if (amount > fc.GetGoodStockInCart(cartID)) {
					cout << "������������\n";
				}
				else if (amount == 0) {
					fc.DeleteCart(cartID);
					cout << "���ﳵ��¼��ɾ����\n";
				}
				else {
					if (fc.UpdateCart(cartID, amount)) {
						cout << "���ﳵ�޸ĳɹ���\n";
					}
					else {
						cout << "���ﳵ��¼�����ڣ�\n";
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
		case 3:	// �鿴����
			fc.ListOrders(userID);
			PAUSE;
			break;
		case 0:	// ����
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
		cout << " 1.չʾ������Ʒ\t2.�޸���Ʒ\t3.������Ʒ\n 4.��ʾ��Ʒ����\t5.������Ʒ����\t6.��������\n 0.����\n" << ">>";
		cin >> choice;
		switch (choice) {
		case 1:	// չʾ������Ʒ
			cout << "��Ʒ�б�\n";
			fc.ListGoods();
			PAUSE;
			break;
		case 2:	// �޸���Ʒ
			fc.ListSales();
			cout << "������޸ĵı�ţ�\n" << ">>";
			cin >> goodID;
			if (!fc.FetchGood(goodID)) {
				cout << "û����ѡ��¼��\n";
			}
			else {
				cout << "�۸�\n" << ">>";
				cin >> price;
				cout << "�������\n" << ">>";
				cin >> amount;
				fc.ChangeGood(goodID, price, amount);
				cout << "�޸ĳɹ�!\n";
			}
			PAUSE;
			break;
		case 3:	// ������Ʒ
			cout << "\n������Ʒ\n" << "��ѡ����Ʒ���ࣺ\n";
			fc.ListGoodTypes();
			cout << ">>";
			cin >> typeID; getchar();
			cout << "�������Ʒ��Ϣ��\n";
			cout << "��Ʒ���ƣ�\n" << ">>";
			getline(cin, goodname);
			cout << "�۸�\n" << ">>";
			cin >> price;
			cout << "�������\n" << ">>";
			cin >> amount; getchar();
			cout << "��Ʒ������\n" << ">>";
			getline(cin, describe);
			fc.CreateGood(goodname, typeID, price, amount, describe);
			cout << "�����ɹ���" << endl;
			PAUSE;
			break;
		case 4:	// ��ʾ��Ʒ����
			cout << "��Ʒ���ࣺ\n";
			fc.ListGoodTypes();
			PAUSE;
			break;
		case 5:	// ������Ʒ����
			cout << "\n������Ʒ����\n" << "�������ƣ�\n" << ">>";
			cin >> str;
			fc.CreateGoodType(str);
			cout << "�����ɹ���" << endl;
			PAUSE;
			break;
		case 6:	// ��������
		{
			unsigned int choice;	// ��������ѡ��
			FUNC_MSG;
			cout << " 1.Ʒ���ۿ�\t2.��������\t0.����\n" << ">>";
			cin >> choice;
			switch (choice) {
			case 1:	// Ʒ���ۿ�
				cout << "Ʒ���ۿ�\n" << "��ѡ��Ʒ�ࣺ\n";
				fc.ListGoodTypes();
				cout << ">>";
				cin >> typeID;
				cout << "�ۿ��ʣ�С������\n" << ">>";
				cin >> discount;
				if (!fc.ChangeTypeDiscount(typeID, discount)) {
					cout << "���벻�Ϸ����������룡" << endl;
				}
				else {
					cout << "�����ɹ���" << endl;
				}
				break;
			case 2:	// ȫ������
				cout << "���������б�\n";
				fc.ListSales();
				cout << " 1.���Ӷ�������ѡ��\t2.ɾ����¼\t0.����\n" << ">>";
				cin >> choice;
				if (choice == 1) {
					unsigned int up, down;
					cout << "����������\n ��������������\n" << ">>��";
					cin >> up;
					cout << ">>��";
					cin >> down;
					fc.CreateSale(up, down);
				}
				else if (choice == 2) {
					SaleID id;
					cout << " �������ɾ������ţ�\n" << ">>";
					cin >> id;
					fc.DeleteSale(id);
					cout << "ɾ���ɹ���\n";
				}
				else if (choice == 0) {
					;	// return
				}
				else {
					ERR_MSG;
				}
				break;
			case 0:	// ���غ�̨����
				break;
			default:
				ERR_MSG;
			}
		}
			PAUSE;
			break;
		case 0:	// ����������
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
	cout << "=== ���� ===" << endl;
	fc.ListCarts(userID);
	cout << "�����ܶ��" << fixed << setprecision(2) << (float)fc.GetCartsPrice(userID) << endl;
	FUNC_MSG;
	cout << " 1.�µ�\t0.����\n" << ">>";
	int choice;
	cin >> choice;
	if (choice == 1) {
		if (fc.CheckCart(userID)) {
			orderID = fc.CreateOrder(userID);
			cout << "�µ��ɹ���" << endl;
			string cardNo, psw;
			bool paid = false;
			while (!paid) {
				int choice;
				cout << "��ѡ��֧����ʽ��\n 1.ѡ�����п�֧��\t2.���¿�֧��\n" << ">>";
				cin >> choice;
				if (choice == 1) {	// ѡ�����п�֧��
					if (fc.ListCards(userID)) {
						do {
							cout << "�����뿨��֧����\n" << ">>";
							cin >> cardNo;
							cout << "���������룺\n" << ">>";
							cin >> psw;
						} while (!fc.Client(cardNo, psw, fc.GetOrderPrice(orderID)));
						paid = true;
					}
				}
				else if (choice == 2) {	// ���¿�֧��
					do {
						cout << "���¿�֧�� �����뿨�ţ�\n" << ">>";
						cin >> cardNo;
						cout << "���������룺\n" << ">>";
						cin >> psw;
					} while (!fc.Client(cardNo, psw));
					fc.AddCard(userID, cardNo);
					if (fc.Client(cardNo, psw, fc.GetOrderPrice(orderID))) {
						paid = true;
					}
					else {
						cout << "֧��ʧ�ܣ�\n";
						PAUSE;
					}
				}
				else {
					ERR_MSG;
					PAUSE;
				}
			}
			cout << "֧���ɹ���\n";
		}
		else {	// �������Ϸ�
			cout << "���޸Ĺ��ﳵ������" << endl;
		}
	}
	else if (choice == 0) {
		;	// return
	}
	else {
		ERR_MSG;
	}
}