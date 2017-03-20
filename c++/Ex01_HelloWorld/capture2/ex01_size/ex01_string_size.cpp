#include <iostream>
#include <string>
int main(int argc, const char *argv[])
{
	std::cout << "Please enter your first name: ";
	std::string name;
	std::cin >> name;
	std::string greeting = "Hello, " + name + '!';
	std::string spaces(greeting.size(), ' ');
	std::string displayHoldLind = '*' + spaces + '*';
	std::string displayGreeting = '*' + greeting + '*';
	std::string displayScanLine(displayGreeting.size(), '*');
	
	std::cout << displayScanLine << std::endl;
	std::cout << displayHoldLind<< std::endl;
	std::cout << displayGreeting << std::endl;
	std::cout << displayHoldLind<< std::endl;
	std::cout << displayScanLine << std::endl;
	return 0;
}
