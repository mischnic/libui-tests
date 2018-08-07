#include <stdio.h>
#include <string.h>
#include "ui.h"

static void cb(uiEntry *e, void *data){
	printf("%s\n", uiEntryText(e));
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

	uiBox *box = uiNewVerticalBox();
	uiBoxSetPadded(box, 1);
	uiWindowSetChild(mainwin, uiControl(box));

	uiEntry *entry = uiNewEntry();
	uiBoxAppend(box, uiControl(entry), 0);
	uiEntryOnChanged(entry, cb, NULL);
	uiEntrySetText(entry, "asd");

	uiControlShow(uiControl(mainwin));

	uiMain();
	return 0;
}