#include "LeblancEngine/BasicInclude/LeblancPCH.h"
#include "LeblancEngine/Engine/LeblancEngine.h"

int APIENTRY wWinMain(HINSTANCE instance, HINSTANCE prev_instance, LPWSTR lp_cmdline, int n_cmdshow)
{
	Engine engine;

	engine.run();

	return 0;
}
