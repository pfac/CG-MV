#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

void init(int *argc, char **argv, void (*renderFunc)());
void changeSize(int w, int h);
