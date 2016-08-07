//Standard includes
#include <stdio.h>
#include <math.h>
#include <conio.h>
#include <string>
#include <fstream>
#include <iostream>

//My includes
#include "Shader.h"
#include "Camera.h"

//OpenGL includes
//GLEW
#include <GL/glew.h>
//GLFW
#include <GLFW/glfw3.h>
//Soil
#include <SOIL.h>
//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



//window variables
int SCREEN_WIDTH, SCREEN_HEIGHT;

//Matrices
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void mouse_callback(GLFWwindow* window, double xPos, double yPos);
bool firstMouse = true;
GLfloat lastX = 400, lastY = 300;
GLfloat yaw = 0, pitch = 0;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
bool zoom = false;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

//deltaTime variables
void updateDeltaTime();
float timeSinceLastFrame;
float deltaTime;

bool keys[1024];

void do_movement();



//temp
glm::vec3 lightDefaultPos(0.0f, 1.0f, 2.0f);
glm::vec3 lightPos = lightDefaultPos;





int main()
{
	printf("Hello World!\n");


	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);




	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow* window = glfwCreateWindow(800,600, "LearnOpenGL", nullptr, nullptr);
	if(window == nullptr)
	{
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetScrollCallback(window, scroll_callback);

	glfwSwapInterval(0);

	


	
	timeSinceLastFrame = glfwGetTime();
	
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
	{
		printf("Failed to initialize GLEW\n");
		return -1;
	}

	
	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	//Shaders
	Shader lightingShader("shaders/vertexShader.vert", "shaders/fragmentShader.frag");

	Shader lampShader("shaders/lampVertexShader.vert", "shaders/lampFragmentShader.frag");

	//Vertices
	GLfloat vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

	





	//Texture

	//Object
	GLuint VBO, VAO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		//position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);



	//light object
	GLuint lightVAO;

	glGenVertexArrays(1, &lightVAO);
		glBindVertexArray(lightVAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	//Drawing Mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	float elapsedTime = 0;

	//Transformation
	view = camera.GetViewMatrix();

	
	projection = glm::perspective(glm::radians(camera.Fov), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);
	

	GLint objectColorLoc = glGetUniformLocation(lightingShader.Program, "objectColor");
	GLint lightColorLoc = glGetUniformLocation(lightingShader.Program, "lightColor");
	GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "lightPos");
	GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");

	GLuint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
	GLuint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
	GLuint projectionLoc = glGetUniformLocation(lightingShader.Program, "projection");

	GLuint lampModelLoc = glGetUniformLocation(lampShader.Program, "model");
	GLuint lampViewLoc = glGetUniformLocation(lampShader.Program, "view");
	GLuint lampProjectionLoc = glGetUniformLocation(lampShader.Program, "projection");



	//Game loop
	while(!glfwWindowShouldClose(window))
	{
		updateDeltaTime();
		elapsedTime += deltaTime;

		//Check and call events
		glfwPollEvents();
		do_movement();

		//Rendering commands here
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		//#####################################
		//#				UPDATING 			  #
		//#####################################

		glm::mat4 model;
		glm::mat4 view;
		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Fov), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);
		

		//Animation

		lightPos = glm::vec3(cos(glfwGetTime()), sin(glfwGetTime() * 1)/5, 0.0f) + lightDefaultPos;

		



		//#####################################
		//#				DRAWING 			  #
		//#####################################


		//Draw the object
		glBindVertexArray(VAO);
			lightingShader.Use();

			glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
			glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
			glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
			glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);

			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

			glDrawArrays(GL_TRIANGLES, 0, 36);		
		glBindVertexArray(0);


		//draw the lamp
		glBindVertexArray(lightVAO);
			model = glm::mat4();
			model = glm::translate(model, lightPos);
			model = glm::scale(model, glm::vec3(0.2f));
			lampShader.Use();

			glUniformMatrix4fv(lampModelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(lampViewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(lampProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		//Swap the buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if(action == GLFW_PRESS)
	{
		keys[key] = true;
	}else if(action == GLFW_RELEASE)
	{
		keys[key] = false;
	}
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if(firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		yaw = -90.0f;
		firstMouse = false;
	}


	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	zoom = yoffset + 1;
	printf(zoom ? "sant\n" : "falskt\n");
}


void do_movement()
{
	GLfloat cameraSpeed;
	if(keys[GLFW_KEY_LEFT_SHIFT])
		camera.MovementSpeed = 15;
	else
		camera.MovementSpeed = 5.0f;

	if(keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if(keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if(keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if(keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);

	if(keys[GLFW_KEY_E])
		camera.ProcessKeyboard(UP, deltaTime);
	if(keys[GLFW_KEY_Q])
		camera.ProcessKeyboard(DOWN, deltaTime);

	if(zoom)
		camera.Fov = 15.0f;
	else
		camera.Fov = 45.0f;
	
}



void updateDeltaTime()
{
	float currentTime = glfwGetTime();
	deltaTime = currentTime - timeSinceLastFrame;
	timeSinceLastFrame = currentTime;
}
