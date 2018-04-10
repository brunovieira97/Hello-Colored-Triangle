using namespace std;

#include "SceneManager.h"


// The MAIN function, from here we start the application and run the game loop
int main()
{
	SceneManager *scene = new SceneManager;
	scene->initialize(1000, 800);

	scene->run();

	scene->finish();

	return 0;
}
