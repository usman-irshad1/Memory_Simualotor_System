#include <iostream>
#include "Virtual_Memory_manager.h"
#include <fstream>
#include <iomanip>
#include <raylib.h>
using namespace std;



int main() {
     
	Virtual_Memory_manager Sherlock; 
	cout << "this is the one with FIFO\n";
	Sherlock.Walker();
	cout << "This is the one with LRU\n";
	Sherlock.Runner();
	cout << "This is the one with OPT\n";
	Sherlock.SPRINTER();

}