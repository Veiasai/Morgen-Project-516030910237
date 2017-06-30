#pragma once

#include <string>
#include <vector>
#include <map>

using namespace std;

struct exfill
{
	int num;
	double priceall;
	exfill() : num(0), priceall(0) {}
};

struct order
{
	int _order_id;
	bool _side;			// 1 represents sell, 0 -> buy
	char * _symbol;
	double _price;
	int _quantity;
	int finishnum;		// the stock fill quantity
	vector<int> split;	//   proportion in every exchange, like 1000:2000:3000
	map<string, exfill> exfillnum;	// fill quantity and price in every exchange
	order(int order_id, bool side, char * symbol, double price, int quantity)
		: _order_id(order_id), _side(side), _symbol(symbol), _price(price), _quantity(quantity), finishnum(0) {}
	~order()
	{
		delete[] _symbol;
	}
};


