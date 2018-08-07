#include <stdio.h>
#include <string.h>
#include "ui.h"

double x = 0;
double y = 0;

static void handlerDraw(uiAreaHandler *a, uiArea *area, uiAreaDrawParams *p)
{
	uiDrawBrush brush;
	brush.Type = uiDrawBrushTypeSolid;
	brush.R = 255;
	brush.G = 0;
	brush.B = 0;
	brush.A = 1.0;

	double i = x / 100;
	double j = 75;//(0.5+(y / 600))*70

	uiDrawMatrix m;
	uiDrawMatrixSetIdentity(&m);

	double newX = 50, newY = 50;
	// uiDrawMatrixTranslate(&m, 50, 50);
	// uiDrawMatrixScale(&m, 0, 0, i, i);
	// uiDrawMatrixTranslate(&m, -50, -50);
	uiDrawMatrixScale(&m, 50, 50, i, i);

	// printf("%f %f, %f %f\n", i, i, i, i);

	uiDrawMatrixTransformPoint(&m, &newX, &newY);
	printf("%f: %f %f\n", i, newX, newY);
	
	uiDrawTransform(p->Context, &m);

	uiDrawPath *path = uiDrawNewPath(uiDrawFillModeWinding);
	uiDrawPathAddRectangle(path, 50, 50, 100, 100);
	uiDrawPathEnd(path);
	

	uiDrawFill(p->Context, path, &brush);
	uiDrawFreePath(path);
}

static void handlerMouseEvent(uiAreaHandler *a, uiArea *area, uiAreaMouseEvent *e) {
	x = e->X;
	y = e->Y;
	uiAreaQueueRedrawAll(area);
}
static void handlerMouseCrossed(uiAreaHandler *ah, uiArea *a, int left) {}
static void handlerDragBroken(uiAreaHandler *ah, uiArea *a) {}
static int handlerKeyEvent(uiAreaHandler *ah, uiArea *a, uiAreaKeyEvent *e) {return 0;}


int main(void)
{
	uiInitOptions o;
	const char *err;

	memset(&o, 0, sizeof (uiInitOptions));
	err = uiInit(&o);
	if (err != NULL) {
		fprintf(stderr, "error initializing ui: %s\n", err);
		uiFreeInitError(err);
		return 1;
	}

	uiWindow *mainwin = uiNewWindow("test", 600, 600, 1);
	uiAreaHandler handler;

	handler.Draw = handlerDraw;
	handler.MouseEvent = handlerMouseEvent;
	handler.MouseCrossed = handlerMouseCrossed;
	handler.DragBroken = handlerDragBroken;
	handler.KeyEvent = handlerKeyEvent;

	uiArea *area = uiNewArea(&handler);

	uiWindowSetChild(mainwin, uiControl(area));

	uiControlShow(uiControl(mainwin));

	uiMain();
	return 0;
}