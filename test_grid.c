#include <stdio.h>
#include <string.h>
#include "ui.h"

uiBox *b;
uiGrid *g;

uiControl *testControl(char *s) {
	return uiControl(uiNewButton(s));
}

void btnClicked(uiButton *btn, void *data){
	if(g == NULL){
		g = uiNewGrid();
		uiBoxAppend(b, uiControl(g), 1);
		uiGridAppend(g, testControl("1"),
			0, 0, 1, 1,
			1, uiAlignStart, 1, uiAlignStart);
		uiGridAppend(g, testControl("2"),
			1, 0, 1, 1,
			1, uiAlignStart, 1, uiAlignStart);
		// uiGridAppend(g, testControl("3"),
		// 	0, 1, 1, 1,
		// 	1, uiAlignStart, 1, uiAlignStart);
		// uiGridAppend(g, testControl("4"),
		// 	1, 1, 1, 1,
		// 	1, uiAlignStart, 1, uiAlignStart);
	}
}

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

	b = uiNewVerticalBox();
	uiWindowSetChild(mainwin, uiControl(b));

	// uiButton *btn = uiNewButton("Display grid");
	// uiBoxAppend(b, uiControl(btn), 0);
	// uiButtonOnClicked(btn, btnClicked, NULL);
	btnClicked(NULL, NULL);
	
	// uiGridAppend(g, testControl("1"),
	// 	0, 0, 1, 1,
	// 	0, uiAlignFill, 0, uiAlignFill);
	// uiGridAppend(g, testControl("2"),
	// 	1, 0, 1, 1,
	// 	0, uiAlignFill, 0, uiAlignFill);
	// uiGridAppend(g, testControl("3"),
	// 	2, 0, 1, 1,
	// 	0, uiAlignFill, 0, uiAlignFill);
	// uiControl *t4 = testControl("4");
	// uiGridAppend(g, t4,
	// 	0, 1, 1, 1,
	// 	0, uiAlignFill, 1, uiAlignFill);
	// uiGridInsertAt(g, testControl("5"),
	// 	t4, uiAtTrailing, 2, 1,
	// 	0, uiAlignFill, 0, uiAlignFill);
	// uiGridAppend(g, testControl("6"),
	// 	-1, 0, 1, 2,
	// 	1, uiAlignFill, 0, uiAlignFill);

	uiControlShow(uiControl(mainwin));

	uiMain();
	return 0;
}
