#ifndef LEBLANC_ENGINE_H
#define LEBLANC_ENGINE_H

#include "LeblancEngine/Render/Basics/LeblancWindow.h"
class Engine
{
public:
	Engine();

	~Engine();

	void run();

private:
	void initialize();

	void prepare();

	void clear();

	void update(float delta_time);

	void render(float delta_time);

	void present();

private:

	Window window;
};

#endif