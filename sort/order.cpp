#include <fstream>
#include <string>
#include <vector>
#include "order.h"
#include "quote.h"
#include <iostream>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <map>
#include "book.h"

using namespace std;

vector<order *> orders;

extern ofstream fout;
void refresh_split();

extern map<string, map<string, ex> > stockaskbook;
extern map<string, map<string, ex> > stockbidbook;

void order_read(fstream &file)
{
	while (file.get() != '\n'); // remove first line
	while (!file.eof())
	{
		int order_id;
		if (!(file >> order_id))	// error
			break;
		file.get();

		bool side;
		if (file.get() == 'B')
			side = 0;
		else
			side = 1;
		while (file.get() != ',');

		char * symbol = new char[8];
		file.getline(symbol, 8, ',');

		double price;
		file >> price;
		file.get();

		int quantity;
		file >> quantity;
		file.get();

		orders.push_back(new order(order_id, side, symbol, price, quantity));
	}
}

void show_order()		// show order on command
{
	refresh_split();
	cout << setw(8) << "Order id" << setw(6) << "Side" << setw(8) << "Symbol"
		<< setw(8) << "Price" << setw(12) << "Quantity" << setw(12) << "finishnum" << setw(12) << "split\n";
	for (auto c : orders)
	{
		cout << setw(7) << c->_order_id;
		if (c->_side == true)
			cout << setw(6) << "Sell";
		else
			cout << setw(6) << "Buy";
		cout << setw(8) << c->_symbol << setw(8) << c->_price
			<< setw(12) << c->_quantity << setw(12) << c->finishnum << setw(10);
		for (auto temp : c->split)
		{
			cout << temp;
			if (temp != c->split.back())
				cout << ":";
		}
		cout << "\n";
	}
}

void write_order()		// write all order in file: fout
{
	refresh_split();
	fout << setw(8) << "Order id" << setw(6) << "Side" << setw(8) << "Symbol"
		<< setw(8) << "Price" << setw(12) << "Quantity" << setw(12) << "finishnum" << setw(12) << "split\n";
	for (auto c : orders)
	{
		fout << setw(7) << c->_order_id;
		if (c->_side == true)
			fout << setw(6) << "Sell";
		else
			fout << setw(6) << "Buy";
		fout << setw(8) << c->_symbol << setw(8) << c->_price
			<< setw(12) << c->_quantity << setw(12) << c->finishnum << setw(10);
		for (auto temp : c->split)
		{
			fout << temp;
			if (temp != c->split.back())
				fout << ":";
		}
		fout << "\n";
	}
	fout << '\n';
}

void refresh_split()
{
	for (auto one : orders)
	{
		one->split.clear();
		if (!(stockbidbook[one->_symbol].size() > 0))
			break;
		if (one->_side == true) // 1 represents sell, 0 -> buy
		{
			for (auto b : stockbidbook[one->_symbol])
				one->split.push_back(b.second._allsize);
		}
		else
		{
			for (auto b : stockaskbook[one->_symbol])		// refresh the split
				one->split.push_back(b.second._allsize);
		}
	}
}

void insert_order()
{
	int order_id;
	cin >> order_id;	// error
	cin.get();

	bool side;
	if (cin.get() == 'B')
		side = 0;
	else
		side = 1;
	while (cin.get() != ',');

	char * symbol = new char[8];
	cin.getline(symbol, 8, ',');

	double price;
	cin >> price;
	cin.get();

	int quantity;
	cin >> quantity;
	while (cin.get() != '\n');

	if (cin.fail())
	{
		cin.clear();
		while (cin.get() != '\n');
		cout << "error format\n";
	}
	else
		orders.push_back(new order(order_id, side, symbol, price, quantity));
}

void write_result()
{
	fout << "\nResult :\n";
	fout << setw(8) << "Order id" << setw(6) << "Side" << setw(8) << "Symbol"
		<< setw(12) << "finishnum\n";
	for (auto c : orders)
	{
		fout << setw(7) << c->_order_id;
		if (c->_side == true)
			fout << setw(6) << "Sell";
		else
			fout << setw(6) << "Buy";
		fout << setw(8) << c->_symbol << setw(12) << c->finishnum << "\n";
		double allprice = 0;
		for (auto temp : c->exfillnum)
		{
			fout << temp.first << ":" << temp.second.num << "@" << temp.second.priceall / temp.second.num << ",";
			allprice += temp.second.priceall;
		}
		fout << "\naverage price: " << allprice / c->finishnum << "\n";
	}
}

void show_result()
{
	cout << "\nResult :\n";
	cout << setw(8) << "Order id" << setw(6) << "Side" << setw(8) << "Symbol"
		<< setw(12) << "finishnum\n";
	for (auto c : orders)
	{
		cout << setw(7) << c->_order_id;
		if (c->_side == true)
			cout << setw(6) << "Sell";
		else
			cout << setw(6) << "Buy";
		cout << setw(8) << c->_symbol << setw(12) << c->finishnum << "\n";
		double allprice = 0;
		for (auto temp : c->exfillnum)
		{
			cout << temp.first << ":" << temp.second.num << "@" << temp.second.priceall / temp.second.num << ",";
			allprice += temp.second.priceall;
		}
		cout << "\naverage price: " << allprice / c->finishnum << "\n";
	}
}