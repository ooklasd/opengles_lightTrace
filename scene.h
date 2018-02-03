#pragma once
#include"Common\esUtil.h"
#include <vector>
#include <algorithm>

struct Vec3
{
	GLfloat _v[3];
};

#define POSITION_LOC    0
#define COLOR_LOC       1

class Object3D
{
public:
	enum TYPE
	{
		TYPE_Plane, TYPE_Sphere
	};
	virtual void initGL(UserData* userdata) {}
	virtual void releaseGL() {}
	//计算交点
	virtual bool intersect(const Vec3& pos, const Vec3& dir, Vec3& hitPoint) = 0;

	//画图
	virtual void drawabletoGL(ESContext *esContext) = 0;

	ESMatrix _m;//世界坐标
	TYPE _type;
	Vec3 _color;
};

//平面
class Plane : public Object3D
{
public:
	Plane(const std::vector<Vec3>& points);

	// 通过 Object3D 继承
	virtual void drawabletoGL(ESContext * esContext) override;

	// 通过 Object3D 继承
	virtual bool intersect(const Vec3 & pos, const Vec3 & dir, Vec3 & hitPoint) override;

	virtual void initGL(UserData* userdata) override;


	virtual void releaseGL() override;

private:
	std::vector<GLfloat> _points;
	std::vector<GLuint> _pointsIndices;
	GLuint _pointsBufferVBO;
	GLuint _pointsBufferIBO;
};

//球体
class Sphere : public Object3D
{
public:
	Sphere(Vec3 center, float radius) 
		:_center(center), _radius(radius) 
	{
		_type = Object3D::TYPE_Sphere;
	}


	virtual bool intersect(const Vec3& pos, const Vec3& dir, Vec3& hitPoint) override;


	virtual void drawabletoGL(ESContext *esContext) override;

private:
	Vec3 _center;
	float _radius;
};


class Light
{ 
public:
	Light() {}

	float _position[3];

	virtual void toGL(ESContext *esContext);
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