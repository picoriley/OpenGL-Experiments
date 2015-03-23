#include "Camera.h"

Camera::Camera(int MVPLocation)
{
	this->MVPLocation = MVPLocation;
	updateMVP();
}

Camera::~Camera()
{
}

void Camera::updateMVP()
{
	glm::mat4 Translation = glm::translate(glm::mat4(), -position);
	glm::mat4 Rotation = glm::rotate(glm::mat4(), -yaw, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 View = Rotation * Translation;

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);  // Changes for each model !
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

	glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &MVP[0][0]);
}

void Camera::move(GLFWwindow* window, double elapsedSeconds)
{
	//Camera Control
	bool camMoved = false;

	//If holding left shift, "Run"
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
	{
		speed = 2.0f;
		yawSpeed = 10.0f;
	}
	else
	{
		speed = 1.0f;
		yawSpeed = 5.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_A))
	{
		position.x -= speed * elapsedSeconds;
		camMoved = true;
	}
	if (glfwGetKey(window, GLFW_KEY_D))
	{
		position.x += speed * elapsedSeconds;
		camMoved = true;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE))
	{
		position.y += speed * elapsedSeconds;
		camMoved = true;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))
	{
		position.y -= speed * elapsedSeconds;
		camMoved = true;
	}
	if (glfwGetKey(window, GLFW_KEY_W))
	{
		position.z -= speed * elapsedSeconds;
		camMoved = true;
	}
	if (glfwGetKey(window, GLFW_KEY_S))
	{
		position.z += speed * elapsedSeconds;
		camMoved = true;
	}
	if (glfwGetKey(window, GLFW_KEY_Q))
	{
		yaw += yawSpeed * elapsedSeconds;
		camMoved = true;
	}
	if (glfwGetKey(window, GLFW_KEY_E))
	{
		yaw -= yawSpeed * elapsedSeconds;
		camMoved = true;
	}
	if (camMoved)
	{
		updateMVP();
	}
}