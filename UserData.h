#pragma once
#include<vector>
#include<list>
#include"Common\esUtil.h"
#include"scene.h"
typedef struct
{
	// Handle to a program object
	GLuint programObject;

	//����
	std::list<Object3D> objects;

	//��Դ
	std::list<Light> lights;
	
	CameraPerspective camera;

} UserData;