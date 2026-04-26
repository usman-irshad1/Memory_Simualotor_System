#include <iostream>
#include "Virtual_Memory_manager.h"
#include <fstream>
#include <iomanip>
#include"Drawer.h"
using namespace std;



int main() {
     
	//Virtual_Memory_manager Sherlock; 
	//cout << "this is the one with FIFO\n";
	//Sherlock.Walker();
	//cout << "This is the one with LRU\n";
	//Sherlock.Runner();
	//cout << "This is the one with OPT\n";
	//Sherlock.SPRINTER();

	Drawer Sherlock1;
	cout << "this is the one with FIFO\n";
	Sherlock1.Walker1();
	cout << "This is the one with LRU\n";
	Sherlock1.Runner1();
	cout << "This is the one with OPT\n";
	Sherlock1.SPRINTER1();

}