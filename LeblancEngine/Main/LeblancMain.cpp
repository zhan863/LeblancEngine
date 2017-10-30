#include "LeblancEngine/Engine/LeblancEngine.h"

int APIENTRY leblancMain(HINSTANCE instance, HINSTANCE prev_instance, LPWSTR lp_cmdline, int n_cmdshow)
{
	Engine engine;

	engine.run();
}
