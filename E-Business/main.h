#pragma once

#include "FacadeController.h"

FacadeController fc;
UserID userID;
GoodsID goodID;
TypeID typeID;
SaleID saleID;
CartID cartID;
OrderID orderID;
string str;
unsigned int choice;

static void welcome();
bool sign_up();
bool sign_in();

void mainwindow();
void background();
void placeorder();
