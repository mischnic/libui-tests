#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ui.h"
#include "lodepng.h"

static uiTableModelHandler mh;

void appendImageNamed(uiImage *i, char *path){
	unsigned error;
	unsigned char* image;
	unsigned width, height;

	error = lodepng_decode32_file(&image, &width, &height, path);
	if(error) {
		fprintf(stderr, "error %u: %s\n", error, lodepng_error_text(error));
		exit(1);
	}

	uiImageAppend(i, image, width, height, width * 4);
	
	free(image);
}

static int modelNumColumns(uiTableModelHandler *mh, uiTableModel *m)
{
	return 1;
}

static uiTableValueType modelColumnType(uiTableModelHandler *mh, uiTableModel *m, int column)
{
	return uiTableValueTypeImage;
}

static int modelNumRows(uiTableModelHandler *mh, uiTableModel *m)
{
	return 5;
}

static uiImage *img;
static uiTableModel *m;

static uiTableValue *modelCellValue(uiTableModelHandler *mh, uiTableModel *m, int row, int col)
{
	return uiNewTableValueImage(img);
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

	uiWindow *mainwin = uiNewWindow("test", 300, 300, 1);

	uiBox *box;
	uiTable *t;
	uiTableParams p;

	img = uiNewImage(20, 22);
	appendImageNamed(img, "lightning-orb.png");

	box = uiNewVerticalBox();
	uiWindowSetChild(mainwin, uiControl(box));

	mh.NumColumns = modelNumColumns;
	mh.ColumnType = modelColumnType;
	mh.NumRows = modelNumRows;
	mh.CellValue = modelCellValue;
	m = uiNewTableModel(&mh);

	memset(&p, 0, sizeof (uiTableParams));
	p.Model = m;
	t = uiNewTable(&p);
	uiBoxAppend(box, uiControl(t), 1);

	uiTableAppendImageColumn(t, "Column 2", 1);

	uiControlShow(uiControl(mainwin));

	uiMain();

	return 0;
}
