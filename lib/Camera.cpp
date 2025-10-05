#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

/**
 * @brief Constructor for Camera with position and up vectors
 * 
 * @param position Camera's starting position
 * @param up Camera's up direction
 */
Camera::Camera(glm::vec3 position, glm::vec3 up) : _pos { position }, _up { up }
{
	updateVectors();
}

/**
 * @brief updates the camera's front, right and up vectors
 */
void Camera::updateVectors()
{
	_front = glm::normalize(
		glm::vec3(
			cos(glm::radians(_yaw)) * cos(glm::radians(_pitch)),
			sin(glm::radians(_pitch)),
			sin(glm::radians(_yaw)) * cos(glm::radians(_pitch))
		)
	);
	_right = glm::normalize(glm::cross(_front, worldUp));
	_up = glm::normalize(glm::cross(_right, _front));
}

/**
 * @brief get the view matrix of camera
 * 
 * @returns view matrix of the camera
 */
glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(_pos, _pos + _front, _up);
}

/**
 * @brief Getter for camera's right vector
 * 
 * @returns Camera's right vector
 */
glm::vec3 Camera::getRight()
{
	return _right;
}

/**
 * @brief Getter for camera's front vector
 * 
 * @returns Camera's front vector
 */
glm::vec3 Camera::getFront()
{
	return _front;
}

/**
 * @brief Getter for camera's up vector
 * 
 * @returns Camera's up vector
 */
glm::vec3 Camera::getUp()
{
	return _up;
}

/**
 * @brief Getter for camera's position
 * 
 * @returns Camera's position vector
 */
glm::vec3 Camera::getPosition()
{
	return _pos;
}

/**
 * @brief Setter for camera's position
 */
void Camera::setPosition(glm::vec3 &pos)
{
	_pos = pos;
}

/**
 * @brief Getter for camera's yaw
 * 
 * @returns Camera's yaw
 */
float Camera::getYaw()
{
	return _yaw;
}

/**
 * @brief Setter for camera's yaw
 */
void Camera::setYaw(float yaw)
{
	_yaw = yaw;
}

/**
 * @brief Getter for camera's pitch
 * 
 * @returns Camera's pitch
 */
float Camera::getPitch()
{
	return _pitch;
}

/**
 * @brief Setter for camera's pitch
 */
void Camera::setPitch(float pitch)
{
	_pitch = pitch;
}

/**
 * @brief Getter for camera's FOV
 * 
 * @returns Camera's FOV
 */
float Camera::getFov()
{
	return _fov;
}

/**
 * @brief Setter for camera's FOV
 */
void Camera::setFov(float fov)
{
	_fov = fov;
}

/**
 * @brief Getter for camera's movement speed
 * 
 * @returns Camera's movement speed
 */
float Camera::getSpeed()
{
	return _speed;
}

/**
 * @brief Getter for camera's mouse sensitivity
 * 
 * @returns Camera's mouse sensitivity
 */
float Camera::getSensitivity()
{
	return _sensitivity;
}