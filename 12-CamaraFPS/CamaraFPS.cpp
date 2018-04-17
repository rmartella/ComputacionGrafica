#include "CamaraFPS.h"

CamaraFPS::CamaraFPS(Vertex3 position, Vertex3 up, float yaw, float pitch) :
	Front(Vertex3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(
		SENSITIVTY), Zoom(ZOOM) {
	this->Position = position;
	this->WorldUp = up;
	this->Yaw = yaw;
	this->Pitch = pitch;
	this->updateCameraVectors();
}

CamaraFPS::CamaraFPS(float posX, float posY, float posZ, float upX, float upY,
	float upZ, float yaw, float pitch) :
	Front(Vertex3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(
		SENSITIVTY), Zoom(ZOOM) {
	this->Position = Vertex3(posX, posY, posZ);
	this->WorldUp = Vertex3(upX, upY, upZ);
	this->Yaw = yaw;
	this->Pitch = pitch;
	this->updateCameraVectors();
}

CamaraFPS::~CamaraFPS()
{
}

void CamaraFPS::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
	float velocity = this->MovementSpeed * deltaTime;
	Vertex3 copyFront = this->Front;
	Vertex3 copyRight = this->Right;
	if (direction == C_FORWARD)
		this->Position += copyFront * velocity;
	if (direction == C_BACKWARD)
		this->Position -= copyFront * velocity;
	if (direction == C_LEFT)
		this->Position -= copyFront.cross(Up).normalize() * velocity;
	if (direction == C_RIGHT)
		this->Position += copyFront.cross(Up).normalize() * velocity;
}

void CamaraFPS::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
	xoffset *= this->MouseSensitivity;
	yoffset *= this->MouseSensitivity;

	this->Yaw += xoffset;
	this->Pitch += yoffset;

	if (constrainPitch) {
		if (this->Pitch > 89.0f)
			this->Pitch = 89.0f;
		if (this->Pitch < -89.0f)
			this->Pitch = -89.0f;
	}

	this->updateCameraVectors();
}

void CamaraFPS::setViewMatrix() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Position.x, Position.y, Position.z, 
			  Position.x + Front.x, Position.y + Front.y, Position.z + Front.z, 
			  Up.x, Up.y, Up.z);
}

void CamaraFPS::updateCameraVectors() {
	Vertex3 front;
	front.x = cos(toRadians(this->Yaw)) * cos(toRadians(this->Pitch));
	front.y = sin(toRadians(this->Pitch));
	front.z = sin(toRadians(this->Yaw)) * cos(toRadians(this->Pitch));
	this->Front = front.normalize();
	this->Right = this->Front.cross(this->WorldUp).normalize();
	this->Up = this->Right.cross(this->Front).normalize();
}

float CamaraFPS::toRadians(float grados) {
	return grados * 3.1416 / 180.0f;
}
