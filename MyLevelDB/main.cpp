#include <iostream>
using namespace std;
#include <stdio.h>

int main()
{
	char* alloc("hello");
	size_t align = 1024;
	size_t current_mod = reinterpret_cast<uintptr_t>(alloc) &(align - 1);
	cout << current_mod;

	cout << endl << "OK" << endl;
	system("Pause");
	return 0;
}