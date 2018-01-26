#pragma once
#include<vector>
#include<list>
#include"Common\esUtil.h"
#include"scene.h"
typedef struct
{
	// Handle to a program object
	GLuint programObject;

	//≥°æ∞
	std::list<Object3D> objects;

	//π‚‘¥
	std::list<Light> lights;
	
	CameraPerspective camera;

} UserData;