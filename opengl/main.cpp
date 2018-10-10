// 28 may 2016
#include <string>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <iostream>

#include "glew.h"

#include "ui.h"

#include "lib/Vec.h"
#include "lib/Mat.h"

#define GLCall(x) GLClearError(); x; GLLogCall(#x, __FILE__, __LINE__);

static void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

static void GLLogCall(const char *function, const char *file, int line) {
	GLenum error;
	while((error = glGetError())) {
		fprintf(stderr, "OpenGL Error %s (%s:%d) - 0x%x\n", function, file, line, error);
	}
}

struct Vertex {
	GLfloat x, y, z;
	GLfloat u, v, w;
	GLfloat n_x, n_y, n_z;
};

typedef struct Vertex Vertex;

static const Vertex BOX[] = {
	//Bottom (yellow)
	{  -1.0f,  -1.0f, +1.0f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f, 0.0f },
	{  -1.0f,  -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f, 0.0f },
	{  +1.0f,  -1.0f, +1.0f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f, 0.0f },
	{  +1.0f,  -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f, 0.0f },

	//Top (white)
	{  -1.0f,  +1.0f, -1.0f,  1.0f, 1.0f, 1.0f,  0.0f, +1.0f, 0.0f },
	{  -1.0f,  +1.0f, +1.0f,  1.0f, 1.0f, 1.0f,  0.0f, +1.0f, 0.0f },
	{  +1.0f,  +1.0f, -1.0f,  1.0f, 1.0f, 1.0f,  0.0f, +1.0f, 0.0f },
	{  +1.0f,  +1.0f, +1.0f,  1.0f, 1.0f, 1.0f,  0.0f, +1.0f, 0.0f },

	//Left (red)
	{  -1.0f,  +1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f },
	{  -1.0f,  +1.0f, +1.0f,  1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f },
	{  -1.0f,  -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f },
	{  -1.0f,  -1.0f, +1.0f,  1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f },

	//Right (magenta)
	{  +1.0f,  +1.0f, +1.0f,  0.0f, 1.0f, 0.0f,  +1.0f, 1.0f, 0.0f },
	{  +1.0f,  +1.0f, -1.0f,  0.0f, 1.0f, 0.0f,  +1.0f, 1.0f, 0.0f },
	{  +1.0f,  -1.0f, +1.0f,  0.0f, 1.0f, 0.0f,  +1.0f, 1.0f, 0.0f },
	{  +1.0f,  -1.0f, -1.0f,  0.0f, 1.0f, 0.0f,  +1.0f, 1.0f, 0.0f },

	//Back (blue)
	{  +1.0f,  +1.0f, -1.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, -1.0f },
	{  +1.0f,  -1.0f, -1.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, -1.0f },
	{  -1.0f,  +1.0f, -1.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, -1.0f },
	{  -1.0f,  -1.0f, -1.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, -1.0f },

	//Front (magenta)
	{  -1.0f,  +1.0f, +1.0f,  1.0f, 0.0f, 1.0f,   0.0f, 0.0f, +1.0f },
	{  -1.0f,  -1.0f, +1.0f,  1.0f, 0.0f, 1.0f,   0.0f, 0.0f, +1.0f },
	{  +1.0f,  +1.0f, +1.0f,  1.0f, 0.0f, 1.0f,   0.0f, 0.0f, +1.0f },
	{  +1.0f,  -1.0f, +1.0f,  1.0f, 0.0f, 1.0f,   0.0f, 0.0f, +1.0f },
};

static GLint BOX_STARTS[] =  {0, 4, 8, 12, 16, 20};
static GLint BOX_LENGTHS[] = {4, 4, 4,  4,  4,  4};

static char* loadFile(std::string path) {
	FILE *f = fopen(path.c_str(), "r");
	if(f != NULL) {
		fseek(f, 0, SEEK_END);
		long length = ftell(f) + 1;
		
		char *buffer = new char[length];

		fseek(f, 0, SEEK_SET);
		fread(buffer, 1, length, f);
		buffer[length - 1] = 0;
		fclose(f);
		return buffer;
	}
	std::cerr << "Shader at `" << path << "` couldn't be loaded!" << "\n";
	exit(EXIT_FAILURE);
}

typedef struct ExampleOpenGLState {
	GLuint VBO;
	GLuint VAO;
	GLint VertexShader;
	GLint FragmentShader;
	GLint Program;
	GLint ProjectionMatrixUniform;
	GLint ModelViewMatrixUniform;
	GLint NormalMatrixUniform;
	GLint AmbientUniform;
	GLint LightDirectionUniform;
	GLint LightingTypesUniform;
	GLint PositionAttrib;
	GLint ColorAttrib;
	GLint NormalAttrib;
} ExampleOpenGLState;

static ExampleOpenGLState openGLState = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

static Vec3 rotation;

static bool mouseInWindow = false;
static Vec2 oldMouse;
static bool oldMousePressed = false;

static bool enableAmbient = true, enableDiffuse = true, enableSpecular = true;
static Vec3 lightDirection(1, -1, -1);

static void onMouseEvent(uiOpenGLAreaHandler *h, uiOpenGLArea *a, uiAreaMouseEvent *e)
{
	double width = e->AreaWidth, height = e->AreaHeight;

	if(e->Down == 1){
		oldMousePressed = true;
		oldMouse.x = (2*(e->X / width)-1);
		oldMouse.y = (2*(e->Y / height)-1);
	} else if(e->Up == 1){
		oldMousePressed = false;
	} else {
		if(oldMousePressed && e->Held1To64 & 1){
			float newMouseX = (2*(e->X / width)-1);
			float newMouseY = (2*(e->Y / height)-1);
			float diffX = newMouseX - oldMouse.x;
			float diffY = newMouseY - oldMouse.y;

			rotation.x += diffX/4 * uiPi;
			rotation.y += diffY/4 * uiPi;

			oldMouse.x = newMouseX;
			oldMouse.y = newMouseY;
			uiOpenGLAreaQueueRedrawAll(a);
		}
	}
}

static void onMouseCrossed(uiOpenGLAreaHandler *h, uiOpenGLArea *a, int left)
{
	mouseInWindow = !left;
}

static int curScale = 10;
static float pos_x = 0, pos_y = 0, pos_z = -5.0f;
static int onKeyEvent(uiOpenGLAreaHandler *h, uiOpenGLArea *a, uiAreaKeyEvent *e)
{
	char key = e->Key;
	if(48 <= key && key <= 57){
		if(key == 48)
			curScale = 10;
		else
			curScale = key - 48;
		uiOpenGLAreaQueueRedrawAll(a);
		return 1;
	}
	float s = sinf(rotation.x), c = cosf(rotation.x);
	float mult = 0.2f;
	if(key == 'w'){
		pos_z += c * mult;
		pos_x += -s * mult;
		uiOpenGLAreaQueueRedrawAll(a);
		return 1;
	} else if(key == 's'){
		pos_z += -c * mult;
		pos_x += s * mult;
		uiOpenGLAreaQueueRedrawAll(a);
		return 1;
	} else if(key == 'a'){
		pos_x += c * mult;
		pos_z += s * mult;
		uiOpenGLAreaQueueRedrawAll(a);
		return 1;
	} else if(key == 'd'){
		pos_x += -c * mult;
		pos_z += -s * mult;
		uiOpenGLAreaQueueRedrawAll(a);
		return 1;
	} else if(key == ' '){
		pos_y -= mult;
		uiOpenGLAreaQueueRedrawAll(a);
		return 1;
	} else if(key == '`'){
		pos_y += mult;
		uiOpenGLAreaQueueRedrawAll(a);
		return 1;
	}
	return 0;
}

static void compileShader(std::string type, GLuint shader)
{
	glCompileShader(shader);
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE){
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
		if(maxLength > 0){
			char log[maxLength];
			glGetShaderInfoLog(shader, maxLength, &maxLength, log);
			std::cerr << type << ": " << log << "\n";
			glDeleteShader(shader);
			exit(EXIT_FAILURE);
		}
	}
}

static void linkProgram(GLuint program)
{
	glLinkProgram(program);
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if(status == GL_FALSE){
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
		if(maxLength > 0){
			char log[maxLength];
			glGetProgramInfoLog(program, maxLength, &maxLength, log);
			std::cerr << log << "\n";
			glDeleteProgram(program);
			exit(EXIT_FAILURE);
		}
	}
}


static void onInitGL(uiOpenGLAreaHandler *h, uiOpenGLArea *a)
{

	openGLState.VertexShader = glCreateShader(GL_VERTEX_SHADER);
	char *vSource = loadFile("/Users/niklas/Desktop/DEV-TEST/libui/libui-tests/opengl/shader/cube.vert");
	GLCall(glShaderSource(openGLState.VertexShader, 1, (const char *const *) &vSource, NULL));
	free(vSource);
	compileShader("Cube Vertex shader", openGLState.VertexShader);

	openGLState.FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	char *fSource = loadFile("/Users/niklas/Desktop/DEV-TEST/libui/libui-tests/opengl/shader/cube.frag");
	GLCall(glShaderSource(openGLState.FragmentShader, 1, (const char *const *) &fSource, NULL));
	free(fSource);
	compileShader("Cube Fragment shader", openGLState.FragmentShader);

	openGLState.Program = glCreateProgram();
	GLCall(glAttachShader(openGLState.Program, openGLState.VertexShader));
	GLCall(glAttachShader(openGLState.Program, openGLState.FragmentShader));
	linkProgram(openGLState.Program);
	GLCall(glUseProgram(openGLState.Program));
	GLCall(glDetachShader(openGLState.Program, openGLState.VertexShader));
	GLCall(glDetachShader(openGLState.Program, openGLState.FragmentShader));

	GLClearError();
	openGLState.ProjectionMatrixUniform = glGetUniformLocation(openGLState.Program, "ProjectionM");
	GLLogCall("glGetUniformLocation(openGLState.Program, 'ProjectionM')", __FILE__, __LINE__);
	GLClearError();
	openGLState.ModelViewMatrixUniform = glGetUniformLocation(openGLState.Program, "ModelViewM");
	GLLogCall("glGetUniformLocation(openGLState.Program, 'ModelViewM')", __FILE__, __LINE__);
	GLClearError();
	openGLState.NormalMatrixUniform = glGetUniformLocation(openGLState.Program, "NormalM");
	GLLogCall("glGetUniformLocation(openGLState.Program, 'NormalM')", __FILE__, __LINE__);
	GLClearError();
	openGLState.AmbientUniform = glGetUniformLocation(openGLState.Program, "Ambient");
	GLLogCall("glGetUniformLocation(openGLState.Program, 'Ambient')", __FILE__, __LINE__);
	GLCall(glUniform3f(openGLState.AmbientUniform, 0.4, 0.4, 0.4));

	GLClearError();
	openGLState.LightDirectionUniform = glGetUniformLocation(openGLState.Program, "LightDirection");
	GLLogCall("glGetUniformLocation(openGLState.Program, 'LightDirection')", __FILE__, __LINE__);

	GLClearError();
	openGLState.LightingTypesUniform = glGetUniformLocation(openGLState.Program, "LightTypes");
	GLLogCall("glGetUniformLocation(openGLState.Program, 'LightDirection')", __FILE__, __LINE__);

	GLClearError();
	openGLState.PositionAttrib = glGetAttribLocation(openGLState.Program, "in_position");
	GLLogCall("glGetAttribLocation(openGLState.Program, 'in_position');", __FILE__, __LINE__);
	GLClearError();
	openGLState.ColorAttrib = glGetAttribLocation(openGLState.Program, "in_color");
	GLLogCall("glGetAttribLocation(openGLState.Program, 'in_color');", __FILE__, __LINE__);
	GLClearError();
	openGLState.NormalAttrib = glGetAttribLocation(openGLState.Program, "in_normal");
	GLLogCall("glGetAttribLocation(openGLState.Program, 'in_normal');", __FILE__, __LINE__);


	GLCall(glGenBuffers(1, &openGLState.VBO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, openGLState.VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(BOX), BOX, GL_STATIC_DRAW));

	GLCall(glGenVertexArrays(1, &openGLState.VAO));
	GLCall(glBindVertexArray(openGLState.VAO));

	GLCall(glVertexAttribPointer(openGLState.PositionAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)0));
	GLCall(glEnableVertexAttribArray(openGLState.PositionAttrib));

	GLCall(glVertexAttribPointer(openGLState.ColorAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)offsetof(Vertex, u)));
	GLCall(glEnableVertexAttribArray(openGLState.ColorAttrib));

	GLCall(glVertexAttribPointer(openGLState.NormalAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid *)offsetof(Vertex, n_x)));
	GLCall(glEnableVertexAttribArray(openGLState.NormalAttrib));

	GLCall(glEnable(GL_DEPTH_TEST));
}

static void onDrawGL(uiOpenGLAreaHandler *h, uiOpenGLArea *a, double width, double height)
{
	GLCall(glViewport(0, 0, width, height));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	Mat4 projection = Mat4::perspective(45.0f / 180.0f * uiPi,
									 (float)width / (float)height,
									 0.1f,
									 100.0f);
	GLCall(glUniformMatrix4fv(openGLState.ProjectionMatrixUniform, 1, GL_TRUE, &projection.m11));

	Mat4 modelview =
		Mat4::rotate(rotation.y, 1.0f, 0.0f, 0.0f) *
		Mat4::rotate(rotation.x, 0.0f, 1.0f, 0.0f) *
		Mat4::translate(pos_x, pos_y, pos_z) *
		Mat4::scale(curScale/10.0f, curScale/10.0f, curScale/10.0f)
	;
	GLCall(glUniformMatrix4fv(openGLState.ModelViewMatrixUniform, 1, GL_TRUE, &modelview.m11));

	Mat3 modelviewNormal = ((Mat3)modelview).inverse().transpose();
	GLCall(glUniformMatrix3fv(openGLState.NormalMatrixUniform, 1, GL_TRUE, &modelviewNormal.m11));

	Vec3 light = (modelviewNormal * lightDirection).normalize();
	// Vec3 light = (modelviewNormal * Vec3(0.0f, -1.0f, -1.0f)).normalize();
	GLCall(glUniform3f(openGLState.LightDirectionUniform, light.x, light.y, light.z));

	GLCall(glUniform3i(openGLState.LightingTypesUniform, enableAmbient, enableDiffuse, enableSpecular));

	// Vec3 normal = (modelviewNormal * Vec3(0, 0, 1.0f)).normalize();
	// Vec3 half = (light + Vec3(0, 0, -1.0f)).normalize();
	// std::cout << light << "\n";
	// std::cout << normal << "\n";
	// std::cout << -(half * normal) << "\n";

	GLCall(glMultiDrawArrays(GL_TRIANGLE_STRIP, BOX_STARTS, BOX_LENGTHS, 6));
	// GLCall(glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(VERTICES)));
	uiOpenGLAreaSwapBuffers(a);
}

static uiOpenGLAreaHandler AREA_HANDLER = {
	onDrawGL,
	onMouseEvent,
	onMouseCrossed,
	NULL,
	onKeyEvent,
	onInitGL
};

static int onClosing(uiWindow *w, void *data)
{
	uiControlDestroy(uiControl(w));
	uiQuit();
	return 0;
}

static int shouldQuit(void *data)
{
	uiControlDestroy((uiControl *)data);
	return 1;
}

// static int render(void *d)
// {
// 	uiOpenGLArea *area = d;
// 	if(!mouseInWindow)
// 		uiOpenGLAreaQueueRedrawAll(area);
// 	return 1;
// }

uiOpenGLArea *glarea;

static void checkboxCallback(uiCheckbox *c, void *data) {
	bool *v = (bool*) data;
	*v = uiCheckboxChecked(c);
	uiOpenGLAreaQueueRedrawAll(glarea);
}

static void sliderCallback(uiSlider *s, void *data) {
	float *p = (float*) data;
	*p = (2 * (uiSliderValue(s) / 100.0f) - 1) * 4;
	uiOpenGLAreaQueueRedrawAll(glarea);
}

int main(void)
{
	uiInitOptions o = { 0 };
	const char *err = uiInit(&o);
	if (err != NULL) {
		fprintf(stderr, "error initializing ui: %s\n", err);
		uiFreeInitError(err);
		return 1;
	}

	uiWindow *mainwin = uiNewWindow("libui OpenGL Example", 640, 480, 1);
	uiWindowOnClosing(mainwin, onClosing, NULL);
	uiWindowSetMargined(mainwin, 1);
	uiOnShouldQuit(shouldQuit, mainwin);

	uiOpenGLAttributes *attribs = uiNewOpenGLAttributes();
	uiOpenGLAttributesSetAttribute(attribs, uiOpenGLAttributeMajorVersion, 3);
	uiOpenGLAttributesSetAttribute(attribs, uiOpenGLAttributeMinorVersion, 3);
	uiOpenGLAttributesSetAttribute(attribs, uiOpenGLAttributeCompatProfile, 0);

	uiBox *b = uiNewVerticalBox();
	uiWindowSetChild(mainwin, uiControl(b));

	uiBox *top = uiNewHorizontalBox();
	uiBoxAppend(b, uiControl(top), 0);

	uiBox *left = uiNewVerticalBox();
	uiBoxAppend(top, uiControl(left), true);
	uiBoxAppend(left, uiControl(uiNewLabel("Press keys 0 to 9 to set the scale")), false);

	uiBox *options = uiNewHorizontalBox();
	uiBoxSetPadded(options, true);
	uiCheckbox *checkboxAmbient = uiNewCheckbox("Ambient");
	uiCheckboxOnToggled(checkboxAmbient, checkboxCallback, &enableAmbient);
	uiCheckboxSetChecked(checkboxAmbient, true);
	uiBoxAppend(options, uiControl(checkboxAmbient), false);
	uiCheckbox *checkboxDiffuse = uiNewCheckbox("Diffuse");
	uiCheckboxOnToggled(checkboxDiffuse, checkboxCallback, &enableDiffuse);
	uiCheckboxSetChecked(checkboxDiffuse, true);
	uiBoxAppend(options, uiControl(checkboxDiffuse), false);
	uiCheckbox *checkboxSpecular = uiNewCheckbox("Specular");
	uiCheckboxOnToggled(checkboxSpecular, checkboxCallback, &enableSpecular);
	uiCheckboxSetChecked(checkboxSpecular, true);
	uiBoxAppend(options, uiControl(checkboxSpecular), false);
	uiBoxAppend(left, uiControl(options), true);

	uiBox *right = uiNewVerticalBox();
	uiBoxSetPadded(right, true);
	uiSlider *lightX = uiNewSlider(0, 100);
	uiSliderSetValue(lightX, 50);
	uiSliderOnChanged(lightX, sliderCallback, &lightDirection.x);
	uiBoxAppend(right, uiControl(lightX), false);
	uiSlider *lightY = uiNewSlider(0, 100);
	uiSliderSetValue(lightY, 50);
	uiSliderOnChanged(lightY, sliderCallback, &lightDirection.y);
	uiBoxAppend(right, uiControl(lightY), false);
	uiSlider *lightZ = uiNewSlider(0, 100);
	uiSliderSetValue(lightZ, 50);
	uiSliderOnChanged(lightZ, sliderCallback, &lightDirection.z);
	uiBoxAppend(right, uiControl(lightZ), false);
	uiBoxAppend(top, uiControl(right), true);

	glarea = uiNewOpenGLArea(&AREA_HANDLER, attribs);
	uiBoxAppend(b, uiControl(glarea), true);
	uiBoxSetPadded(b, true);
	uiOpenGLAreaSetVSync(glarea, true);
	uiOpenGLAreaMakeCurrent(glarea);

	GLenum glewStatus = glewInit();
	if (glewStatus != GLEW_OK) {
		fprintf(stderr, "GLEW init error: %s\n", glewGetErrorString(glewStatus));
		exit(EXIT_FAILURE);
	}

	// uiTimer(1000/60, render, glarea);

	uiControlShow(uiControl(mainwin));
	uiMain();

	uiFreeOpenGLAttributes(attribs);

	uiUninit();
	return 0;
}
