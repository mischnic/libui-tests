#include <stdio.h>
#include <string.h>
#include "ui.h"

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

	uiBox *box = uiNewHorizontalBox();
	// uiBox *box = uiNewVerticalBox();
	uiBoxSetPadded(box, 1);
	uiWindowSetChild(mainwin, uiControl(box));

	uiLabel *label = uiNewLabel("Status: Stopped");
	uiBoxAppend(box, uiControl(label), 0);
	// uiSeparator *sep = uiNewHorizontalSeparator();
	uiSlider *sep = uiNewSlider(0, 100);
	uiBoxAppend(box, uiControl(sep), 0);
	uiButton *but = uiNewButton("Start");
	uiBoxAppend(box, uiControl(but), 0);

	uiControlShow(uiControl(mainwin));

	uiMain();
	return 0;
}
