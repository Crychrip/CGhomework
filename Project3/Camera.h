#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/transform.hpp>


class Camera {
public:
	glm::vec3 Position;
	glm::vec3 WorldUp;
	glm::vec3 Front;
	glm::vec3 Up;
	float MoveStep = 0.1f;

	float Pitch;
	float Yaw;

	glm::vec2 LastMousePos;
	bool moving = false;

	Camera() {
		Position = glm::vec3(0.0f, 0.0f, 0.0f);
		WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		Pitch = 0;
		Yaw = -90;
		Front = glm::vec3(0.0f, 0.0f, -1.0f);
		Up = glm::vec3(0.0f, 1.0f, 0.0f);

		updateCameraVectors();
	}

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float pitch, float yaw)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Pitch = pitch;
		Yaw = yaw;
		Front = glm::vec3(0.0f, 0.0f, -1.0f);
		Up = glm::vec3(0.0f, 1.0f, 0.0f);

		updateCameraVectors();
	}

	Camera(glm::vec3 pos, glm::vec3 up, float pitch, float yaw)
	{
		Position = pos;
		WorldUp = up;
		Pitch = pitch;
		Yaw = yaw;
		Front = glm::vec3(0.0f, 0.0f, -1.0f);
		Up = glm::vec3(0.0f, 1.0f, 0.0f);
		updateCameraVectors();
	}

	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(
			Position,
			Position + Front,
			Up
		);
	}

	void Move(float dx, float dy, float dz)
	{
		Position += glm::vec3(dx, dy, dz);

		updateCameraVectors();
	}

	void MoveForward(float delta)
	{
		Move(delta * glm::cos(glm::radians(Yaw)), 0, delta * glm::sin(glm::radians(Yaw)));
	}

	void MoveBackward(float delta)
	{
		float toward = glm::radians(Yaw) + glm::pi<float>();
		Move(delta * glm::cos(toward), 0, delta * glm::sin(toward));
	}

	void MoveLeft(float delta)
	{
		float toward = glm::radians(Yaw) - glm::pi<float>() / 2;
		Move(delta * glm::cos(toward), 0, delta * glm::sin(toward));
	}

	void MoveRight(float delta)
	{
		float toward = glm::radians(Yaw) + glm::pi<float>() / 2;
		Move(delta * glm::cos(toward), 0, delta * glm::sin(toward));
	}

	void ProcessKey(unsigned char key, int x, int y)
	{
		switch (key)
		{
		case 'W':
		case 'w':
			MoveForward(MoveStep);
			break;
		case 'S':
		case 's':
			MoveBackward(MoveStep);
			break;
		case 'A':
		case 'a':
			MoveLeft(MoveStep);
			break;
		case 'D':
		case 'd':
			MoveRight(MoveStep);
			break;
		case 'Z':
		case 'z':
			Move(0.0f, MoveStep, 0.0f);
			break;
		case 'X':
		case 'x':
			Move(0.0f, -MoveStep, 0.0f);
			break;
		default:
			break;
		}
	}

	void ProcessMotion(int x, int y)
	{
		if (!moving) return;

		float dx = x - LastMousePos.x;
		float dy = y - LastMousePos.y;

		float sensitivity = 0.5f;

		Turn(-dy * sensitivity, dx * sensitivity);

		LastMousePos.x = x;
		LastMousePos.y = y;
	}

	void ProcessMouse(int button, int state, int x, int y)
	{
		if (state == 0) 
		{
			LastMousePos.x = (float)x;
			LastMousePos.y = (float)y;
			moving = true;
		}
		else if (state == 1)
		{
			moving = false;
		}
	}

	void Turn(float dpitch, float dyaw)
	{
		Pitch += dpitch;
		Yaw += dyaw;

		if (Pitch >= 89.0f)
		{
			Pitch = 89.0f;
		}
		else if (Pitch <= -89.0f)
		{
			Pitch = -89.0f;
		}

		updateCameraVectors();
	}

private:
	void updateCameraVectors() {
		Front.x = glm::cos(glm::radians(Pitch)) * glm::cos(glm::radians(Yaw));
		Front.y = glm::sin(glm::radians(Pitch));
		Front.z = glm::cos(glm::radians(Pitch)) * glm::sin(glm::radians(Yaw));
		Front = glm::normalize(Front);
		glm::vec3 Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};
