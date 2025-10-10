#include <gtest/gtest.h>
#include <glm/gtc/epsilon.hpp>
#include "Camera.hpp"

bool vecEqEpsilon(glm::vec3 &right, glm::vec3 &left, float epsilon=0.001f)
{
	return (
		(abs(right.x - left.x) < epsilon) 
		&& (abs(right.y - left.y) < epsilon)
		&& (abs(right.z - left.z) < epsilon)
	);
}

TEST(CameraTest, shouldConstructCameraWithGivenPosition)
{
	glm::vec3 pos { 0.0f, 0.0f, 3.0f };
	glm::vec3 up { 0.0f, 1.0f, 0.0f };
	glm::vec3 right { 1.0f, 0.0f, 0.0f };
	glm::vec3 front { 0.0f, 0.0f, -1.0f };
	Camera camera { pos };

	ASSERT_TRUE(vecEqEpsilon(pos, camera.getPosition()));
	ASSERT_TRUE(vecEqEpsilon(up, camera.getUp()));
	ASSERT_TRUE(vecEqEpsilon(right, camera.getRight()));
	ASSERT_TRUE(vecEqEpsilon(front, camera.getFront()));
}

TEST(CameraTest, shouldPitchCameraDown)
{
	glm::vec3 down { 0.0f, -1.0f, 0.0f };
	Camera camera { glm::vec3(0.0f) };
	camera.setPitch(-90.0f);
	camera.updateVectors();

	ASSERT_TRUE(vecEqEpsilon(down, camera.getFront()));
}

TEST(CameraTest, shouldPitchCameraUp)
{
	glm::vec3 up { 0.0f, 1.0f, 0.0f };
	Camera camera { glm::vec3(0.0f) };
	camera.setPitch(90.0f);
	camera.updateVectors();

	ASSERT_TRUE(vecEqEpsilon(up, camera.getFront()));
}

TEST(CameraTest, shouldYawCameraLeft)
{
	glm::vec3 left { -1.0f, 0.0f, 0.0f };
	Camera camera { glm::vec3(0.0f) };
	camera.setYaw(camera.getYaw() - 90.0f);
	camera.updateVectors();
	ASSERT_TRUE(vecEqEpsilon(left, camera.getFront()));
}

TEST(CameraTest, shouldYawCameraRight)
{
	glm::vec3 right { 1.0f, 0.0f, 0.0f };
	Camera camera { glm::vec3(0.0f) };
	camera.setYaw(camera.getYaw() + 90.0f);
	camera.updateVectors();
	ASSERT_TRUE(vecEqEpsilon(right, camera.getFront()));
}