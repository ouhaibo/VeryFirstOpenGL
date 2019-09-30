#include <glad/glad.h>
#include <GLFW/glfw3.h>//������glad.h֮��
#include <iostream>

float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left  
		 0.5f, -0.5f, 0.0f, // right 
		 0.0f,  0.5f, 0.0f  // top   
};

const char* vertexShaderString = "#version 330 core\n"
"layout (location=0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderString = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(0.85f, 1.0f, 0.12f, 1.0f);\n"
"}\n";

long(*innerFunc)(long, int);

void framebuffer_sizechanged_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void keyevent_callback(GLFWwindow* window, int arg1, int arg2, int arg3, int arg4)
{
	std::cout << arg1 << std::endl;
}

long mock(long arg1, int arg2)
{
	return 1l;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
int main()
{
	innerFunc = mock;
	innerFunc(2l, 3);
	int count = 3;
/************************************************************************** OpenGL is a huge state machine **********************************************************************************************************************/
	
	//1.Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//2.Create a window
	GLFWwindow* window = glfwCreateWindow(1280, 960, "Ouhaibo learn OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create window!!!" << std::endl;
		glfwTerminate();
		return -1;
	}

	//3.Make the context of the window
	glfwMakeContextCurrent(window);

	//4.Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD!!!" << std::endl;
		return -1;
	}

	//5.Set the coordinate and dimension
	glViewport(0, 0, 1280, 960);

	//6.Register a callback for window resize
	glfwSetFramebufferSizeCallback(window, framebuffer_sizechanged_callback);
	glfwSetKeyCallback(window, keyevent_callback);

	//generating an unused buffer object name(id)
	GLuint vertex_bufffer_obj;
	glGenBuffers(1, &vertex_bufffer_obj);
	//binding an actual buffer object to the buffer object name(id)
	glBindBuffer(GL_ARRAY_BUFFER, vertex_bufffer_obj);
	//copies vertices data to our buffer object(GPU) 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//create a shader object referenced by an ID
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//attach shader source code to the shader object
	glShaderSource(vertexShader, 1, &vertexShaderString, nullptr);
	//compile the shader
	glCompileShader(vertexShader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "Error in compiling vertex shader:" << infoLog << std::endl;
		return 0;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderString, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cout << "Error in compiling fragment shader:" << infoLog << std::endl;
		return 0;
	}

	//create a shader program object referenced by returned ID
	GLuint shader_program = glCreateProgram();
	//attach the compiled shader objects to the shader program
	glAttachShader(shader_program, vertexShader);
	glAttachShader(shader_program, fragmentShader);
	//link shader objects and shader program object
	glLinkProgram(shader_program);
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader_program, 512, nullptr, infoLog);
		std::cout << "Error in linking shader program:" << infoLog << std::endl;
		return 0;
	}
	//use the shader program object and linked shader objects to rendering call
	glUseProgram(shader_program);
	//delete the shader objects after being linked into the shader program since we no longer need them
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);










	//7.Start render loop
	while (!glfwWindowShouldClose(window))
	{
		count++;

		//process input
		processInput(window);

		//rendering(this is state-setting function,changes the state of the OpenGL context)
		if (count % 12 == 0)
		{
			glClearColor(0.2f, 0.5f, 0.75f, 0.31f);
		}
		else
		{
			glClearColor(0.9f, 0.12f, 0.45f, 0.77f);
		}
		
		//specify which buffer to clear(here is color buffer)
		//this is a state-using function,performs some operations based on the current state
		glClear(GL_COLOR_BUFFER_BIT);

		//check and call events
		glfwPollEvents();

		//swap front and back buffer
		glfwSwapBuffers(window);
	}

	//8.Close and release things
	glfwTerminate();
	return 0;
}

