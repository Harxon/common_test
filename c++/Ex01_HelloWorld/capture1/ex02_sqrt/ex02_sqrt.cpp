#include<iostream>
//#include<cmath>
#include<math.h>
#include <cstdlib>
using namespace std;

int main(int argc, const char *argv[])
{
	cout << "How many apples do you have?" << endl;
	int num;
	cin >> num;
	double apples = sqrt(num);
	cout << "I have " << apples << " apples" << endl;
	cout << "A rand: ";
	int r = rand();
	cout << r << endl;
//	cout << endl << bucks(apples) << endl;
	

	return 0;
}
