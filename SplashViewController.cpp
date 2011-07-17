#include <cstring>
#include <cstdio>
#ifndef __APPLE__
#include <GL/glew.h>
#include <GL/glu.h>
#endif

#include "SplashViewController.h"

bool SplashViewController::menuFinishedView() {
	return (finishedView = true);
}

static bool menuQuit() {
	printf("Exiting\n");
	exit(0);
	return true;
}

SplashViewController::SplashViewController() {
	finishedView = false;

	splashMenu = new menu();
	splashMenu->add_menuitem(new actionmenuitem(&SplashViewController::menuFinishedView, NULL, (char *)"Play Game"));
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
	splashMenu->drawMenu();
	return;
}

void SplashViewController::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glFlush();
}

