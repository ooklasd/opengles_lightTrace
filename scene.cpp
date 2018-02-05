#include "scene.h"
#include "UserData.h"
#include "GLES3/gl3.h"

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


Plane::Plane(const std::vector<Vec3>& points)
{
	this->_points.reserve(points.size() * 3);
	for each (auto p in points)
	{
		this->_points.push_back(p._v[0]);
		this->_points.push_back(p._v[1]);
		this->_points.push_back(p._v[2]);
	}

	if (points.size() >= 3)
	{
		_pointsIndices.reserve((points.size() - 2) * 3);
		for (size_t i = 1; i < points.size(); ++i)
		{
			_pointsIndices.push_back(0);
			_pointsIndices.push_back(i);
			_pointsIndices.push_back(i + 1);
		}
	}

	_type = Object3D::TYPE_Plane;
}
void Plane::initGL(UserData* userdata)
{

	//创建顶点数据缓存并录入
	glGenBuffers(_points.size(), &_pointsBufferVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _pointsBufferVBO);
	glBufferData(GL_ARRAY_BUFFER, _points.size() * sizeof(float), &_points.front(), GL_STATIC_DRAW);

	//创建索引数据缓存并录入
	glGenBuffers(_points.size(), &_pointsBufferIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _pointsBufferIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*_pointsIndices.size(), &_pointsIndices.front(), GL_STATIC_DRAW);


}

void Plane::releaseGL()
{
	glDeleteBuffers(1, &_pointsBufferIBO);
	glDeleteBuffers(1, &_pointsBufferVBO);
}

void Plane::drawabletoGL(ESContext * esContext)
{
	auto userdata = (UserData*)esContext->userData;
	glBindBuffer(GL_ARRAY_BUFFER, _pointsBufferVBO);
	glVertexAttribPointer(POSITION_LOC, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
	glEnableVertexAttribArray(POSITION_LOC);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_pointsBufferIBO);

	glUniformMatrix4fv(userdata->mvpLoc, 1, GL_FALSE, &_m.m[0][0]);

	glVertexAttrib3f(GL_COLOR, _color._v[0], _color._v[1], _color._v[2] );

	glDrawElements(GL_TRIANGLES, _pointsIndices.size(), GL_UNSIGNED_INT, nullptr);
}

bool Plane::intersect(const Vec3 & pos, const Vec3 & dir, Vec3 & hitPoint)
{
	return false;
}





Sphere::Sphere(Vec3 center, float radius) :_center(center), _radius(radius)
{
	_type = Object3D::TYPE_Sphere;

	_vertex = nullptr;
	_indices = nullptr;
	_pointsBufferVBO = 0;
}

Sphere::~Sphere()
{
	releaseGL();
}

bool Sphere::intersect(const Vec3& pos, const Vec3& dir, Vec3& hitPoint)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void Sphere::drawabletoGL(ESContext *esContext)
{
	auto userdata = (UserData*)esContext->userData;
	glUniformMatrix4fv(userdata->mvpLoc, 1, GL_FALSE, &_m.m[0][0]);
	glUniform1f(userdata->radiusLoc, _radius);
	glUniform3fv(userdata->centerLoc,3 ,_center._v);

	glVertexAttribPointer(POSITION_LOC, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),_vertex);
	glEnableVertexAttribArray(POSITION_LOC);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _pointsBufferIBO);

	glVertexAttrib3f(GL_COLOR, _color._v[0], _color._v[1], _color._v[2]);
	glDrawElements(GL_TRIANGLE_STRIP, _indicesNumb, GL_UNSIGNED_INT, nullptr);
}

void Sphere::initGL(UserData* userdata)
{
	releaseGL();
	_indicesNumb = esGenSphere(10, _radius, &_vertex, nullptr, nullptr, &_indices);

	glGenBuffers(_indicesNumb, &_pointsBufferIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _pointsBufferIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indicesNumb * sizeof(GLuint), _indices, GL_STATIC_DRAW);

}

void Sphere::releaseGL()
{
	delete[] _vertex;
	_vertex = nullptr;
	delete[] _indices;
	_indices = nullptr;

	glDeleteBuffers(1,&_pointsBufferVBO);
	glDeleteBuffers(1, &_pointsBufferIBO);
}
