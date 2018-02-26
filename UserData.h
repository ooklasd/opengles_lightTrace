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
	GLuint SphereProgramObject;//Բ����Ⱦ��
	GLuint PanelProgramObject;//ƽ����Ⱦ��
	GLuint curProgramObject;//ƽ����Ⱦ��
	//GLuint SceneProgramObject;

	//����
	std::list<std::shared_ptr<Object3D>> objects;

	//��Դ
	std::list<std::shared_ptr<Light>> lights;
	
	CameraPerspective camera;

	GLint mvpCameraLoc;			//�������
	GLint mPlaneLoc;			//��������
	GLint mSphereLoc;		//��������
	GLint colorLoc;			//��ɫ

	GLuint radiusLoc;	//�뾶
	GLuint centerLoc;	//Բ��

	float ambientLight;//������
	Vec3 ambientColor;//��������ɫ
	//GLuint radiusLoc;

	float angle = PI;
	float speed = 0.05;
	float cameraradius = 2;

} ;