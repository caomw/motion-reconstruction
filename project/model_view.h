#include <windows.h>
#include <GL/gl.h>
#include "GL/glut.h"
#include "model_obj.h"


void load();
void face_disp(int view, double * ps);
float linearcomb(double * p, float * u, int j);