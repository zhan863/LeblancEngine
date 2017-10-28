#include "LeblancEngine/Engine/LeblancEngine.h"
#include "LeblancEngine/BasicInclude/LeblancExceptions.h"
#include "LeblancEngine/BasicInclude/LeblancPCH.h"

Engine::Engine()
{

}

Engine::~Engine()
{
	clear();
}

void Engine::initialize()
{
	// initialize window

	// initialize global context and the managers inside it
}

void Engine::prepare()
{
	// initialize the pipeline

	// initialize all the render states and textures needed

	// load engine content
}

void Engine::clear()
{
	// clear all the resources handled by the managers inside the global context

	// clear global context and the managers inside it

	// unload the engine content
}

void Engine::update(float delta_time)
{
	// logic update the position and light position of the scene

	// visibility test and put the visible objects to pipeline for render
}

void Engine::render(float delta_time)
{
	// render the scene to a texture
}

void Engine::present()
{
	// d3d device present
}

void Engine::run()
{
	initialize();

	prepare();

	try
	{
		window.ShowWindow();

		initialize();

		prepare();

		while (window.IsAlive())
		{
			if (!window.IsMinimized())
			{
				update(0);

				render(0);
				
				present();
			}

			window.MessageLoop();
		}
	}
	catch (Exception exception)
	{
		exception.ShowErrorMessage();
	}
}

