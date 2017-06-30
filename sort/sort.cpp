#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <vector>
#include "order.h"
#include "quote.h"
#include "book.h"
#include <ctime>
#include <ostream>
#include <sstream>
#include <thread>

using namespace std;

void order_read(fstream &file);
void show_order();
void quote_read(fstream &file);
bool aggresive(quote & current);
bool passive(quote & current);
void show_allbook();
void insert_order();
void thread_quote();
void thread_run();
void show_result();
void write_result();

ofstream fout;

int sleeptime;	// for simulation
bool output_switch = true;	// if or not output result to a file
bool running;	// control quote reading
bool _end;
fstream order_file;
fstream quote_file;
clock_t start;

int main()
{
	string filename;

	cout << "Input unit time:";
	cin >> sleeptime;
	cin.get();
	
	cout << "input the orders file name:";
	cin >> filename;
	order_file.open(filename);

	start = clock();	// first start counting
	if (!order_file.fail())
		order_read(order_file);
	else
	{
		cout << "open orders failed\n";
		return 0;
	}
	order_file.close();
	cout << "read orders consume " << clock() - start << "/" << CLOCKS_PER_SEC << " (s) " << endl;

	cout << "input the quotes file name:";
	cin >> filename;
	quote_file.open(filename);

	if (quote_file.fail())
	{
		cout << "open quotes failed\n";
		return 0;
	}

	cout << "input the output filename,\n if you don't need output, Enter to continue.(max speed):";
	while (cin.get() != '\n'); //remove

	getline(cin, filename);
	if (filename != "")
		fout.open(filename);
	else
		output_switch = false;

	while (true)	// insert order before running
	{
		show_order();
		cout << "warnning: please ensure the format: OrderId,Side,Symbol,Price,Quantity\n";
		cout << "input a order(Enter to end):";
		if (cin.peek() == '\n')
		{
			cin.get();
			break;
		}
		else
			insert_order();
	}

	if (sleeptime != 0)
	{
		running = true;
		thread one_quote(thread_quote);
		one_quote.detach();
		thread_run();
	}
	else
	{
		running = true;
		start = clock();	// second start counting
		thread_quote();
		cout << "read quotes consume " << clock() - start << "/" << CLOCKS_PER_SEC << " (s) " << endl;
	}
	show_result();
	if (output_switch)
		write_result();
	return 0;
}

void thread_quote()
{
	_end = false;
	quote_read(quote_file);
	quote_file.close();
	fout.close();
	cout << "we have read all quotes.\n";
	_end = true;
}

void thread_run()
{
	cout << "\ninput 1: display the order status\n";
	cout << "input 2: display the book\n";
	cout << "input 3: insert a order\n";
	cout << "input 4: pause or continue quote reading\n";
	cout << "input 5: exit\n";
	char c;
	while (!_end)
	{
		c = cin.get();
		if (c == '1')
			show_order();
		else if (c == '2')
			show_allbook();
		else if (c == '3')
		{
			cout << "warnning: please ensure the format: OrderId,Side,Symbol,Price,Quantity\n";
			cout << "input a order:";
			insert_order();
		}
		else if (c == '4')
			running = !running;
		else if (c == '5')
			break;
		else if (c == '\n')
		{
			cout << "input 1: display the order status\n";
			cout << "input 2: display the book\n";
			cout << "input 3: insert a order\n";
			cout << "input 4: pause or continue quote reading\n";
			cout << "input 5: exit\n";
			continue;
		}
		else
			while (cin.get() != '\n');
	}
}