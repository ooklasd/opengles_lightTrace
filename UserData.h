#pragma once
#include<vector>
#include<list>
#include"Common\esUtil.h"
#include"scene.h"
typedef struct
{
	// Handle to a program object
	GLuint SphereProgramObject;//圆形渲染器
	GLuint PanelProgramObject;//平面渲染器
	GLuint SceneProgramObject;

	//场景
	std::list<Object3D> objects;

	//光源
	std::list<Light> lights;
	
	CameraPerspective camera;

	GLuint mvpLoc;

	GLuint radiusLoc;
	GLuint radiusLoc;

} UserData;