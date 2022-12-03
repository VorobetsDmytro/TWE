#include "scene/time.hpp"

namespace TWE {
	GLfloat Time::fps = 0.f;
	GLfloat Time::deltaTime = 0.f;
	GLfloat Time::curTime = 0.f;
	GLfloat Time::preTime = 0.f;

	void Time::calculate() {
		curTime = glfwGetTime();
		deltaTime = curTime - preTime;
		fps = 1.0f / deltaTime;
		preTime = curTime;
	}
}