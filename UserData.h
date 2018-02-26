#pragma once
#include<vector>
#include<list>
#include<memory>
#include"Common\esUtil.h"
#include"scene.h"
#define PI 3.1415926535897932384626433832795f
struct UserData
{
	// Handle to a program object
	GLuint SphereProgramObject;//圆形渲染器
	GLuint PanelProgramObject;//平面渲染器
	GLuint curProgramObject;//平面渲染器
	//GLuint SceneProgramObject;

	//场景
	std::list<std::shared_ptr<Object3D>> objects;

	//光源
	std::list<std::shared_ptr<Light>> lights;
	
	CameraPerspective camera;

	GLint mvpCameraLoc;			//相机坐标
	GLint mPlaneLoc;			//世界坐标
	GLint mSphereLoc;		//世界坐标
	GLint colorLoc;			//颜色

	GLuint radiusLoc;	//半径
	GLuint centerLoc;	//圆心

	float ambientLight;//环境光
	Vec3 ambientColor;//环境光颜色
	//GLuint radiusLoc;

	float angle = PI;
	float speed = 0.05;
	float cameraradius = 2;

} ;