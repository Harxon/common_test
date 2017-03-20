#include <iostream>
#include <string>

int main(int argc, const char *argv[])
{
	std::cout << "Please enter your first name: " << std::endl;
	std::string name;
	std::cin >> name;
	std::cout << "Hello, " << name << "\n";
	return 0;
}
