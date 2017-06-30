#include <fstream>
#include <vector>
#include "quote.h"
#include "order.h"
#include "book.h"
#include <map>
#include <iostream>
#include <iomanip>
#include <string>
#include <ostream>
#include <sstream>
#include <windows.h>
#include <thread>


using namespace std;
extern vector<order *> orders; // from order.cpp
extern bool output_switch;

bool aggresive(quote & current);
bool passive(quote & current);
void quote_read(fstream &file);
void show_allbook();
void write_allbook();
void write_order();

map<string, map<string, ex> > stockaskbook;
map<string, map<string, ex> > stockbidbook;
extern ofstream fout;	// file out
extern int sleeptime;	// for simulation
extern bool running;

void quote_read(fstream &file)
{
	while (file.get() != '\n'); // remove first line
	while (!file.eof())
	{
		while (!running)
			Sleep(1000);
		char * exchange = new char[8];
		file.getline(exchange, 8, ',');

		bool side;
		if (file.get() == 'B') // 1 represents bid, 0 -> ask
			side = 1;
		else
			side = 0;
		while (file.get() != ',');

		char * symbol = new char[8];
		file.getline(&symbol[0], 8, ',');

		int size;
		file >> size;
		file.get();

		double price;
		file >> price;
		file.get();

		if (side == 1) // 1 represents bid, 0 -> ask
			stockbidbook[string(symbol)][string(exchange)]._allsize += size;
		else
			stockaskbook[string(symbol)][string(exchange)]._allsize += size;
		aggresive(quote(exchange, side, symbol, size, price));

		Sleep(sleeptime);	
	}
}

bool aggresive(quote & current)  // try all orders
{
	for (auto one : orders)		// take a order
	{
		if (current._side == one->_side && (strcmp(current._symbol, one->_symbol) == 0))
		{
			if ((current._side == true && current._price >= one->_price) || 
				(current._side == false && current._price <= one->_price) ) // 'true' is bid, 'false' is ask
			{																// fill orders
				int temp = one->_quantity - one->finishnum;	// temp is the quantity we need
				if (temp == 0)	// skip
					continue;
				if (current._size >= temp)	
				{
					one->finishnum = one->_quantity;	// fill all
					one->exfillnum[current._exchange].num += temp;
					one->exfillnum[current._exchange].priceall += current._price * temp;
					current._size -= temp;
					if (output_switch)
						write_order();
				}
				else
				{
					one->finishnum += current._size;	// plus
					one->exfillnum[current._exchange].num += current._size;
					one->exfillnum[current._exchange].priceall += (current._price * current._size);
					current._size = 0;
					if (output_switch)
						write_order();
					return 0;	// skip next
				}
			}
		}
	}
	passive(current);	// if current.size > 0, do passive
	return 1;
}

bool passive(quote & current)	// stockbook map : stockname->stockbook
{								// second map: exchange name -> stock in a special exchange
	if (current._side == false)
	{
		stockaskbook[string(current._symbol)][string(current._exchange)]._price = current._price;
		stockaskbook[string(current._symbol)][string(current._exchange)]._size = current._size;
	}
	else
	{
		stockbidbook[string(current._symbol)][string(current._exchange)]._price = current._price;
		stockbidbook[string(current._symbol)][string(current._exchange)]._size = current._size;
	}
	if (output_switch)
		write_allbook();
	return 0;
}

void show_allbook()
{
	cout << "askBook:\n";
	for (auto b : stockaskbook)
	{
		cout << "Stock:" << b.first << endl;
		for (auto c : b.second)
			cout << c.first << ":" << c.second._size << "@" << c.second._price << ",";
		cout << endl;
	}
	cout << "bidBook:\n";
	for (auto b : stockbidbook)
	{
		cout << "Stock:" << b.first << endl;
		for (auto c : b.second)
			cout << c.first << ":" << c.second._size << "@" << c.second._price << ",";
		cout << endl;
	}
}

void write_allbook()
{
	fout << "askBook:\n";
	for (auto b : stockaskbook)	// in a book is a map<exchangename, ...>
	{
		fout << "Stock:" << b.first << " ";
		for (auto c : b.second)
			fout << c.first << ":" << c.second._size << "@" << c.second._price << ",";
		fout << "\n";
	}
	fout << "bidBook:\n";

	for (auto b : stockbidbook) // in a book is a map<exchangename, ...>
	{
		fout << "Stock:" << b.first << " ";
		for (auto c : b.second)
			fout << c.first << ":" << c.second._size << "@" << c.second._price << ",";
		fout << "\n";
	}
	fout << "\n";
}


