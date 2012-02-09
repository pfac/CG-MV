#include "HdrApp.h"
#include "AllocOverLoaderOn.h"
#include "SDL.h"
int wmain(int, char**)
{
	try
	{
		HdrApp *app = new HdrApp();
		app->run(800, 600);
		delete app;
	}
	catch (const std::exception &e)
	{
		std::cerr << "-- Exception--\n" << e.what() << std::endl;
	}

	return 0;
}
