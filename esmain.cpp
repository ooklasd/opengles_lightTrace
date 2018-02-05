#include "Common/esUtil.h"
#include"scene.h"
#include"UserData.h"



///
// Create a shader object, load the shader source, and
// compile the shader.
//
GLuint LoadShader ( GLenum type, const char *shaderSrc )
{
	GLuint shader;
	GLint compiled;

	// Create the shader object
	shader = glCreateShader ( type );

	if ( shader == 0 )
	{
		return 0;
	}

	// Load the shader source
	glShaderSource ( shader, 1, &shaderSrc, NULL );

	// Compile the shader
	glCompileShader ( shader );

	// Check the compile status
	glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

	if ( !compiled )
	{
		GLint infoLen = 0;

		glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );

		if ( infoLen > 1 )
		{
			char *infoLog = (char*)malloc ( sizeof ( char ) * infoLen );

			glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
			esLogMessage ( "Error compiling shader:\n%s\n", infoLog );

			free ( infoLog );
		}

		glDeleteShader ( shader );
		return 0;
	}

	return shader;

}

///
// Initialize the shader and program object
//
int Init ( ESContext *esContext )
{
	UserData *userData = (UserData *)esContext->userData;
	char vShaderStr[] =
		"#version 300 es                          \n"
		"layout(location = 0) in vec4 vPosition;  \n"
		"uniform mat4 u_mvpMatrix;                   \n"
		"void main()                              \n"
		"{                                        \n"
		"   gl_Position = vPosition;              \n"
		"}                                        \n";

	char fShaderStr[] =
		"#version 300 es                              \n"
		"precision mediump float;                     \n"
		"out vec4 fragColor;                          \n"
		"void main()                                  \n"
		"{                                            \n"
		"   fragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );  \n"
		"}                                            \n";

	userData->PanelProgramObject = esLoadProgram(vShaderStr, fShaderStr);
	userData->SphereProgramObject = esLoadProgram(vShaderStr, fShaderStr);

	glClearColor ( 1.0f, 1.0f, 1.0f, 0.0f );

	//设置对象
	auto& objs = userData->objects;

	//添加一个球体
	//objs.push_back(std::shared_ptr<Object3D>(new Sphere(Vec3({0.f,0.f,0.f}),1.f )));

	//添加地面面
	auto p = new Plane({ 
		Vec3{ -0.3f	,-0.3f	,0.0f },
		Vec3{ 0.3f	,-0.3f	,0.0f },
		Vec3{ 0.3f	,0.3f	,0.0f } });
	objs.push_back(std::shared_ptr<Object3D>(p));


	//初始化对象
	for each (auto obj in userData->objects)
	{
		obj->initGL(userData);
	}

	return TRUE;
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw ( ESContext *esContext )
{
	UserData *userData = (UserData *)esContext->userData;
	
	// Set the viewport
	glViewport ( 0, 0, esContext->width, esContext->height );

	// Clear the color buffer
	glClear ( GL_COLOR_BUFFER_BIT );


	// Load the vertex data

	for each (auto it in userData->objects)
	{
		// Use the program object
		switch (it->_type)
		{
		case Object3D::TYPE_Plane:
			userData->curProgramObject = (userData->PanelProgramObject);
			break;
		case Object3D::TYPE_Sphere:
			userData->curProgramObject = (userData->SphereProgramObject);
			break;
		}
		glUseProgram(userData->curProgramObject);

		//draw
		it->drawabletoGL(esContext);
	}

	////设置light
	//for each (Light it in userData->lights)
	//{
	//	it.drawabletoGL(userData);
	//}

}

void Shutdown ( ESContext *esContext )
{
	UserData *userData = (UserData *)esContext->userData;

	glDeleteProgram(userData->PanelProgramObject);
	glDeleteProgram(userData->SphereProgramObject);

	for each (auto obj in userData->objects)
	{
		if (obj) obj->releaseGL();
	}

	delete userData;
	esContext->userData = nullptr;
}

extern "C" int esMain( ESContext *esContext )
{
	esContext->userData = new UserData();

	esCreateWindow ( esContext, "light trace", 640, 320, ES_WINDOW_RGB );

	if ( !Init ( esContext ) )
	{
		return GL_FALSE;
	}
	esRegisterShutdownFunc ( esContext, Shutdown );
	esRegisterDrawFunc ( esContext, Draw );

	return GL_TRUE;
}
