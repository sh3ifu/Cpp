#include <iostream>
#include <string>
#include <conio.h>
#include "MD5.cpp"
using namespace std;


int main()
{
	string text;

	cout << "Please enter a text : ";
	cin >> text;

	MD5 md5(text);

	cout << "\nHash : " << md5.hash();


	_getch();
	return 0;
}