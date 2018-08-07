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

	uiDrawBrush brush2;
	brush2.Type = uiDrawBrushTypeSolid;
	brush2.R = 0;
	brush2.G = 0;
	brush2.B = 0;
	brush2.A = 1.0;

	uiDrawStrokeParams sp = {0};
	sp.Thickness = 20;

	uiDrawPath *path = uiDrawNewPath(uiDrawFillModeWinding);
	uiDrawPathAddRectangle(path, 0, 0, 10000, 10000);
	uiDrawPathEnd(path);	

	uiDrawFill(p->Context, path, &brush);
	uiDrawFreePath(path);

	path = uiDrawNewPath(uiDrawFillModeWinding);
	uiDrawPathAddRectangle(path, 0, 0, width, height);
	uiDrawPathEnd(path);	

	uiDrawStroke(p->Context, path, &brush2, &sp);
	uiDrawFreePath(path);
}

static void handlerMouseEvent(uiAreaHandler *a, uiArea *area, uiAreaMouseEvent *e) {
	width = 2*e->X;
	height = 2*e->Y;
	// printf("%d %d\n", (int)width, (int)height);
	if(e->Down == 1 || (e->Held1To64 & 1) == 1){
		uiAreaSetSize(area, width, height);
		uiAreaQueueRedrawAll(area);
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

	uiWindow *mainwin = uiNewWindow("test", 600, 600, 1);
	uiAreaHandler handler;

	handler.Draw = handlerDraw;
	handler.MouseEvent = handlerMouseEvent;
	handler.MouseCrossed = handlerMouseCrossed;
	handler.DragBroken = handlerDragBroken;
	handler.KeyEvent = handlerKeyEvent;

	uiArea *area = uiNewScrollingArea(&handler, 400, 400);

	uiBox *box = uiNewVerticalBox();
	uiBoxAppend(box, uiControl(area), 1);

	uiWindowSetChild(mainwin, uiControl(box));

	uiControlShow(uiControl(mainwin));

	uiMain();
	return 0;
}