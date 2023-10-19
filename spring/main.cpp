#include "application.h"

int main()
{	
	try
	{
		Application app = Application();
		app.MainLoop();
		app.CleanUp();
	}
	catch(std::exception& e)
	{
		std::cout << e.what();
	}

	return 0;
}