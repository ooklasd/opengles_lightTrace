#pragma once
#include"Common\esUtil.h"
class Object3D
{
public:

	//计算交点
	virtual bool intersect();

	//画图
	virtual void toGL();

	ESMatrix _m;//世界坐标
};


class Light
{ 
public:
	Light() {}

	float _position[3];

	virtual void toGL();
};




class CameraPerspective
{
public:
	CameraPerspective();

	void setPosition(GLfloat x, GLfloat y, GLfloat z);
	void setLookAt(GLfloat x, GLfloat y, GLfloat z);
	void setUp(GLfloat x, GLfloat y, GLfloat z);

public:
	void init(ESContext *esContext);

private:
	ESMatrix _perspectiveM;
	ESMatrix _translate;

	GLfloat _position[3];
	GLfloat _lookAt[3];
	GLfloat _up[3];
};