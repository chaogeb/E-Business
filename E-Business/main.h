#pragma once

#include "FacadeController.h"

FacadeController fc;
UserID userID;
GoodsID goodID;
TypeID typeID;
SaleID saleID;
CartID cartID;
string str;
unsigned int choice;

static void welcome();
bool sign_up();
bool sign_in();

void mainwindow();
void background();
void placeorder();
