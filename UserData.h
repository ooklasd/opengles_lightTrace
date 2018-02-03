#pragma once
#include<vector>
#include<list>
#include"Common\esUtil.h"
#include"scene.h"
typedef struct
{
	// Handle to a program object
	GLuint SphereProgramObject;//Բ����Ⱦ��
	GLuint PanelProgramObject;//ƽ����Ⱦ��
	GLuint SceneProgramObject;

	//����
	std::list<Object3D> objects;

	//��Դ
	std::list<Light> lights;
	
	CameraPerspective camera;

	GLuint mvpLoc;

	GLuint radiusLoc;
	GLuint radiusLoc;

} UserData;