#include "Common/esUtil.h"
#include"scene.h"
#include"UserData.h"
#include <assert.h>



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
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	UserData *userData = (UserData *)esContext->userData;
	char vShaderStr[] =
		"#version 300 es                          \n"
		"uniform mat4 u_mvpMatrix;                         \n"
		"uniform mat4 u_objectMatrix;                         \n"
		"layout(location = 0) in vec4 vPosition;  \n"
		"void main()                              \n"
		"{                                        \n"
		"   gl_Position = u_mvpMatrix*u_objectMatrix*vPosition;              \n"
		"}                                        \n";

	char fShaderStr[] =
		"#version 300 es                              \n"
		"precision mediump float;                     \n"
		"uniform vec3 u_colorVec3;					 \n"
		"uniform vec3 u_shpereCenter;					 \n"
		"uniform float u_radius;					 \n"
		"out vec4 fragColor;                          \n"
		"void main()                                  \n"
		"{                                            \n"
		"   fragColor = vec4 ( u_colorVec3, 1.0 );  \n"
		"}                                            \n";

	userData->PanelProgramObject = esLoadProgram(vShaderStr, fShaderStr);
	userData->SphereProgramObject = userData->PanelProgramObject;
	//userData->SphereProgramObject = esLoadProgram(vShaderStr, fShaderStr);

	assert(userData->PanelProgramObject != 0);
	assert(userData->SphereProgramObject != 0);
	if (userData->PanelProgramObject == 0) return false;
	if (userData->SphereProgramObject == 0) return false;

	glClearColor ( 1.0f, 1.0f, 1.0f, 0.0f );

	//设置相机
	auto& camera = userData->camera;
	camera.setPosition(0.f, 1.0f, -3.f);
	camera.setLookAt(0, 0.5f, 0.0f);
	camera.setUp(0.0f,1.0f,0.0f);
	camera.initCamera(esContext);
	camera.initGL(userData);


	//设置对象
	auto& objs = userData->objects;	

	{
		//添加球体
		auto p = new Sphere({
			Vec3({ 0.0f,0.5f,0.5f }),0.3f
			});
		p->setColor({ 0.0f,0.f,0.5f });
		objs.push_back(std::shared_ptr<Object3D>(p));
	}

	{
		//添加地面面
		auto p = new Plane({
			Vec3{ 0.5f	,0.0f,0.5f },
			Vec3{ 0.5f	,0.0f,-0.5f },
			Vec3{ -0.5f	,0.0f,-0.5f },
			Vec3{ -0.5f	,0.0f,0.5f }
			});
		p->setColor({ 0.f,0.5f,0.f });
		objs.push_back(std::shared_ptr<Object3D>(p));
	}

	{
		//添加墙面
		auto p = new Plane({
			Vec3{ 0.5f	,1.0f  ,0.5f },
			Vec3{ 0.5f	,0.0f ,0.5f },
			Vec3{ -0.5f	,0.0f ,0.5f },
			Vec3{ -0.5f	,1.0f  ,0.5f }
			});
		p->setColor({ 0.5f,0.f,0.f });
		objs.push_back(std::shared_ptr<Object3D>(p));
	}

	


	//初始化对象
	for each (auto obj in userData->objects)
	{
		switch (obj->_type)
		{
		case Object3D::TYPE_Plane:userData->curProgramObject = userData->PanelProgramObject; break;
		case Object3D::TYPE_Sphere:userData->curProgramObject = userData->SphereProgramObject; break;
		}
		obj->initGL(userData);
		userData->curProgramObject = 0;
	}

	
	return TRUE;
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw ( ESContext *esContext )
{
	UserData *userData = (UserData *)esContext->userData;
	userData->camera.drawGL(userData);

	// Set the viewport
	glViewport ( 0, 0, esContext->width, esContext->height );
	glClearColor(0.5, 0.5, 0.5, 0);
	glClearDepthf(1.0f);
	// Clear the color buffer
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//改变相机
	auto camera = &userData->camera;
	camera->setPosition(cos(userData->angle)*userData->cameraradius
		,1.0f
		,sin(userData->angle)*userData->cameraradius);
	camera->initCamera(esContext);

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
	Sleep(16);
	if (userData->angle > PI*10) userData->angle -= PI*10;
	userData->angle += userData->speed;
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
