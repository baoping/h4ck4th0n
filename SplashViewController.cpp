#include <cstring>
#include <cstdio>
#include <SDL/SDL.h>
#ifndef __APPLE__
#include <GL/glew.h>
#include <GL/glu.h>
#endif

#include "SplashViewController.h"

static bool finishedView = false;

static bool menuQuit() {
	printf("Exiting\n");
	exit(0);
	return true;
}

static bool menuFinishedView() {
	return finishedView = true;
}

SplashViewController::SplashViewController() {
	SDL_WM_GrabInput(SDL_GRAB_OFF);
	finishedView = false;

	splashMenu = new menu();
	splashMenu->add_menuitem(new actionmenuitem(menuFinishedView, NULL, (char *)"Play Game"));
	splashMenu->add_menuitem(new actionmenuitem(menuQuit, NULL, (char *)"Quit"));

	splashMenu->set_active(true);
}

SplashViewController::~SplashViewController() {
	delete splashMenu;
}

bool SplashViewController::didFinishView() {
	return finishedView;
}

void SplashViewController::process() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_KEYUP:
				splashMenu->key_input(event.key.keysym.sym);
				break;
		}
	}
	return;
}

void SplashViewController::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
	splashMenu->drawMenu();
	glFlush();
}
