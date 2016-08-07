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


float mixValue = 0.0;
bool open = false;


//deltaTime variables
void updateDeltaTime();
float timeSinceLastFrame;
float deltaTime;


glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
GLfloat fov = 45;

bool keys[1024];

void do_movement();

int main()
{
	printf("Hello World!\n");


	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

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

	//Shader
	Shader ourShader("shaders/vertexShader.vert", "shaders/fragmentShader.frag");


	//Vertices
	GLfloat vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

	glm::vec3 cubePositions[] = {
	  glm::vec3( 0.0f,  0.0f,  0.0f), 
	  glm::vec3( 2.0f,  5.0f, -15.0f), 
	  glm::vec3(-1.5f, -2.2f, -2.5f),  
	  glm::vec3(-3.8f, -2.0f, -12.3f),  
	  glm::vec3( 2.4f, -0.4f, -3.5f),  
	  glm::vec3(-1.7f,  3.0f, -7.5f),  
	  glm::vec3( 1.3f, -2.0f, -2.5f),  
	  glm::vec3( 1.5f,  2.0f, -2.5f), 
	  glm::vec3( 1.5f,  0.2f, -1.5f), 
	  glm::vec3(-1.3f,  1.0f, -1.5f)  
	};

	

	

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);







	//Texture

	int imageWidth1, imageHeight1;
	unsigned char* image1 = SOIL_load_image("res/container.jpg", &imageWidth1, &imageHeight1, 0, SOIL_LOAD_RGB);

	int imageWidth2, imageHeight2;
	unsigned char* image2 = SOIL_load_image("res/awesomeface1.png", &imageWidth2, &imageHeight2, 0, SOIL_LOAD_RGBA);

	GLuint texture1, texture2;
	glGenTextures(1, &texture1);
	glGenTextures(1, &texture2);

	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth1, imageHeight1, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image1);

	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth2, imageHeight2, 0, GL_RGBA, GL_UNSIGNED_BYTE, image2);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image2);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Object
	GLuint VBO, VAO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		//position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		//color attribute
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
		//glEnableVertexAttribArray(1);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	//Drawing Mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	float elapsedTime = 0;




	//Transformation
	
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	
	view = camera.GetViewMatrix();

	
	projection = glm::perspective(glm::radians(camera.Fov), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);
	
	GLuint modelLoc = glGetUniformLocation(ourShader.Program, "model");
	GLuint viewLoc = glGetUniformLocation(ourShader.Program, "view");
	GLuint projectionLoc = glGetUniformLocation(ourShader.Program, "projection");



	//Game loop
	while(!glfwWindowShouldClose(window))
	{
		updateDeltaTime();
		elapsedTime += deltaTime;
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//Check and call events
		glfwPollEvents();
		do_movement();

		//Rendering commands here
		glClearColor(0.2f,0.3f,0.3f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Activate the shader
		ourShader.Use();


		//enable textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture2"), 1);


		//Set the mix value for the textures
		if(open)
		{
			mixValue += deltaTime;
		}
		else
		{
			mixValue -= deltaTime;
		}
		if(mixValue >= 0.5)
			mixValue = 0.5;
		
		if(mixValue <= 0)
			mixValue = 0;


		glUniform1f(glGetUniformLocation(ourShader.Program, "mixValue"), mixValue);

		//transform matrices
		glm::mat4 model;
		model = glm::rotate(model, glm::radians(elapsedTime * 50.0f), glm::vec3(0.5,1.0,0.0));


		//transform the object
		glm::mat4 view;
		view = camera.GetViewMatrix();
		
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));



		//#####################################
		//#				DRAWING 			  #
		//#####################################



		glBindVertexArray(VAO);
		for(GLuint i = 0; i < 10; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			GLfloat angle = 20.0f * i;
			 if(i % 2 == 0)  // Every 3rd iteration (including the first) we set the angle using GLFW's time function.
       			angle += glfwGetTime() * 25.0f;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
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

	if(key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		if(mixValue == 0)
		{
			open = true;
		}
		
	}

	if(key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		if(mixValue == 0.5)
		{
			open = false;
		}
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
	projection = glm::perspective(glm::radians(camera.Fov), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);
}



void updateDeltaTime()
{
	float currentTime = glfwGetTime();
	deltaTime = currentTime - timeSinceLastFrame;
	timeSinceLastFrame = currentTime;
}
