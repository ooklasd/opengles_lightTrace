#include "scene.h"
#include "UserData.h"

CameraPerspective::CameraPerspective()
{
	
}

void CameraPerspective::setPosition(GLfloat x, GLfloat y, GLfloat z)
{
	_position[0] = x;
	_position[1] = y;
	_position[2] = z;
}

void CameraPerspective::setLookAt(GLfloat x, GLfloat y, GLfloat z)
{
	_lookAt[0] = x;
	_lookAt[1] = y;
	_lookAt[2] = z;
}

void CameraPerspective::setUp(GLfloat x, GLfloat y, GLfloat z)
{
	_up[0] = x;
	_up[1] = y;
	_up[2] = z;
}

void CameraPerspective::init(ESContext *esContext)
{
	UserData* userData = (UserData*)esContext->userData;

	esMatrixLoadIdentity(&_perspectiveM);
	esMatrixLoadIdentity(&_translate);

	GLfloat aspect = (GLfloat)esContext->width / esContext->height;
	esPerspective(&_perspectiveM, 45.0, aspect, 0.1, 100);

	esMatrixLookAt(&_translate
		, _position[0], _position[1], _position[2]
		, _lookAt[0], _lookAt[1], _lookAt[2]
		, _up[0], _up[1], _up[2]);

}

void Scene::initScene()
{

}
