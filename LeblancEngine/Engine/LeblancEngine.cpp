#include "LeblancEngine/Engine/LeblancEngine.h"
#include "LeblancEngine/BasicInclude/LeblancExceptions.h"
#include "LeblancEngine/BasicInclude/LeblancPCH.h"
#include "LeblancEngine/Render/Basics/LeblancWindow.h"
#include "LeblancEngine/Global/LeblancGlobalContext.h"

// To do: replace these strings with another way of loading scene
const char* scene_file = "Content/Scene/leblanc_test_scene.scene";

Engine::Engine() : m_window(NULL, L"Leblanc Engine", WS_OVERLAPPEDWINDOW, WS_EX_APPWINDOW, 1280, 720)
{

}

Engine::~Engine()
{
	clear();
}

void Engine::initialize()
{
	// initialize global context and the managers inside it
	g_global_context.initialize(m_window);
}

void Engine::prepare()
{
	// initialize the pipeline

	// initialize all the render states and textures needed

	// load engine content
	g_global_context.m_scene_manager.buildScene(scene_file);
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
	g_global_context.m_pipeline_manager.render(RenderType::Deferred);
}

void Engine::present()
{
	// d3d device present
	g_global_context.m_device_manager.present();
}

void Engine::run()
{
	try
	{
		initialize();

		prepare();

		m_window.ShowWindow();

		while (m_window.IsAlive())
		{
			if (!m_window.IsMinimized())
			{
				update(0);

				render(0);

				present();
			}

			m_window.MessageLoop();
		}
	}
	catch (Exception exception)
	{
		exception.ShowErrorMessage();
	}
}

