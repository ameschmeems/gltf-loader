#pragma once
#include <glm/glm.hpp>

/**
 * @class Camera Camera.hpp "include/Camera.hpp"
 * @brief A first person fly camera
 */
class Camera
{
public:

	Camera() = delete;
	Camera(const Camera &rhs) = default;
	Camera(Camera &&rhs) = default;
	Camera(glm::vec3 position=glm::vec3(0.0f), glm::vec3 up=glm::vec3(0.0f, 1.0f, 0.0f));
	~Camera() = default;

	Camera &operator=(const Camera &rhs) = default;
	Camera &operator=(Camera &&rhs) = default;

	void updateVectors();
	glm::mat4 getViewMatrix();
	glm::vec3 getRight();
	glm::vec3 getFront();
	glm::vec3 getUp();
	glm::vec3 getPosition();
	void setPosition(glm::vec3 &pos);
	float getYaw();
	void setYaw(float yaw);
	float getPitch();
	void setPitch(float pitch);
	float getFov();
	void setFov(float fov);
	float getSpeed();
	float getSensitivity();

private:

	glm::vec3 _pos {};
	glm::vec3 _front { 0.0f, 0.0f, -1.0f };
	glm::vec3 _up {};
	glm::vec3 _right {};
	glm::vec3 worldUp { 0.0f, 1.0f, 0.0f };
	float _yaw { -90.0f };
	float _pitch { 0.0f };
	float _speed { 2.5f };
	float _sensitivity { 0.1f };
	float _fov { 45.0f };
};