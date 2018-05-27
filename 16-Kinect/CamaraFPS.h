#pragma once
#include <math.h>
#include <GL\glut.h>

enum Camera_Movement {
	C_FORWARD, C_BACKWARD, C_LEFT, C_RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 3.0f;
const float SENSITIVTY = 0.25f;
const float ZOOM = 45.0f;

typedef struct _Vertex3 {
	float x, y, z;
	_Vertex3() {
	}
	_Vertex3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	static _Vertex3 Zero() {
		_Vertex3 vertex;
		vertex.x = 0.0;
		vertex.y = 0.0;
		vertex.z = 0.0;
		return vertex;
	}
	_Vertex3 sub(_Vertex3 v) {
		_Vertex3 vertex;
		vertex.x = v.x - x;
		vertex.y = v.y - y;
		vertex.z = v.z - z;
		return vertex;
	}
	float norm() {
		return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	}

	_Vertex3 normalize() {
		_Vertex3 result;
		float norm = this->norm();
		result.x = this->x / norm;
		result.y = this->y / norm;
		result.z = this->z / norm;
		return result;
	}

	_Vertex3 cross(_Vertex3 v) {
		_Vertex3 result;
		result.x = this->y * v.z - this->z * v.y;
		result.y = this->z * v.x - this->x * v.z;
		result.z = this->x * v.y - this->y * v.x;
		return result;
	}

	float dot(_Vertex3 v) {
		float result;
		result = this->x * v.x + this->y * v.y + this->z * v.z;
		return result;
	}

	_Vertex3& operator * (const float &o) {
		_Vertex3 result;
		this->x *= o;
		this->y *= o;
		this->z *= o;
		return *this;
	}

	_Vertex3& operator += (const _Vertex3 &o) {
		this->x += o.x;
		this->y += o.y;
		this->z += o.z;
		return *this;
	}

	_Vertex3& operator -= (const _Vertex3 &o) {
		this->x -= o.x;
		this->y -= o.y;
		this->z -= o.z;
		return *this;
	}
	_Vertex3& operator=(const _Vertex3 &o) {
		this->x = o.x;
		this->y = o.y;
		this->z = o.z;
		return *this;
	}
} Vertex3;

class CamaraFPS
{
public:

	// Camera Attributes
	Vertex3 Position;
	Vertex3 Front;
	Vertex3 Up;
	Vertex3 Right;
	Vertex3 WorldUp;
	// Eular Angles
	float Yaw;
	float Pitch;
	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	CamaraFPS(Vertex3 position = Vertex3(0.0f, 0.0f, 0.0f), Vertex3 up =
		Vertex3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	CamaraFPS(float posX, float posY, float posZ, float upX, float upY,
		float upZ, float yaw, float pitch);
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset,
		bool constrainPitch = true);
	void setViewMatrix();
	~CamaraFPS();
private:
	void updateCameraVectors();
	float toRadians(float grados);
};

