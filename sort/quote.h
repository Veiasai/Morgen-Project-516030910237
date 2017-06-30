#pragma once

struct quote
{
	char * _exchange;
	bool _side;			// 1 represents bid, 0 -> ask
	char * _symbol;
	int _size;
	double _price;
	quote(char * exchange, bool side, char * symbol, int size, double price)
		: _exchange(exchange), _side(side), _symbol(symbol), _size(size), _price(price) {}
	~quote()
	{
		delete[] _exchange;
		delete[] _symbol;
	}
};
