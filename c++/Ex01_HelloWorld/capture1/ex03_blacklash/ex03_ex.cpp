#include <iostream>
#define DEBUG 3

#if (DEBUG == 1)
int main(int argc, const char *argv[])
{
	std::cout << "This (\") is a quote, and this (\\) is a blacklash." << std::endl;
	return 0;
}
#endif 
#if (DEBUG == 2)
int main(int argc, const char *argv[])
{
	{
		{
			std::cout << "This (\") is a quote, and this (\\) is a blacklash." << std::endl;
		}
	}
}
#endif 
#if (DEBUG == 3)
int main(int argc, const char *argv[])
{
	//This is /*
	// a test sample*/ 
	std::cout << "This\n(\")\nis\na\nquote, and this (\\) is a blacklash." << std::endl;
	return 0;
}
#endif 
