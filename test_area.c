#include <stdio.h>
#include <string.h>
#include "ui.h"

double width = 0;
double height = 0;

static void handlerDraw(uiAreaHandler *a, uiArea *area, uiAreaDrawParams *p)
{
	uiDrawBrush brush;
	brush.Type = uiDrawBrushTypeSolid;
	brush.R = 255;
	brush.G = 0;
	brush.B = 0;
	brush.A = 1.0;

	uiDrawPath *path = uiDrawNewPath(uiDrawFillModeWinding);
	uiDrawPathAddRectangle(path, 0, 0, 100, 100);
	uiDrawPathEnd(path);	

	uiDrawFill(p->Context, path, &brush);
	uiDrawFreePath(path);
}

static void handlerMouseEvent(uiAreaHandler *a, uiArea *area, uiAreaMouseEvent *e) {
	if(e->Down == 1){
		uiAreaBeginUserWindowResize(area, uiWindowResizeEdgeTop);
		// uiAreaBeginUserWindowMove(area);
	}
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

	uiWindow *mainwin = uiNewWindow("test", 400, 400, 1);
	uiWindowSetMargined(mainwin, 1);
	uiAreaHandler handler;

	handler.Draw = handlerDraw;
	handler.MouseEvent = handlerMouseEvent;
	handler.MouseCrossed = handlerMouseCrossed;
	handler.DragBroken = handlerDragBroken;
	handler.KeyEvent = handlerKeyEvent;

	uiArea *area = uiNewArea(&handler);

	uiBox *box = uiNewVerticalBox();
	uiBoxSetPadded(box, 1);
	uiWindowSetChild(mainwin, uiControl(box));


	uiSlider *slider = uiNewSlider(0, 100);
	uiButton *button = uiNewButton("Toggle");

	uiBoxAppend(box, uiControl(area), 0);
	uiBoxAppend(box, uiControl(slider), 0);
	uiBoxAppend(box, uiControl(button), 0);

	uiControlShow(uiControl(mainwin));

	uiMain();
	return 0;
}