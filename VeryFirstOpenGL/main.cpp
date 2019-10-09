#include <glad/glad.h>
#include <GLFW/glfw3.h>//必须在glad.h之后
#include <iostream>

float vertices[] = {
		-0.89f, -0.88f, 0.0f, // left  
		-0.41f, -0.48f, 0.0f, // right 
		-0.66f,  0.66f, 0.0f  // top   
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


float rect_vertices[] = {
	0.12f,  0.86f, 0.0f,
	0.72f,  0.53f, 0.0f,
	0.64f, -0.39f, 0.0f,
	0.28f, -0.66f, 0.0f
};

unsigned int rect_indices[] = {
	0,1,3,1,2,3
};

const char* rectVertexShaderString = "#version 330 core\n"
"layout (location=0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* rectFragmentShaderString = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(0.302f, 0.75f, 0.908f, 1.0f);\n"
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

	//Generate an unused buffer object name(id)
	GLuint vertex_bufffer_obj;
	glGenBuffers(1, &vertex_bufffer_obj);

	//Generate an unused vertex array object name(id)
	GLuint vertex_array_obj;
	glGenVertexArrays(1, &vertex_array_obj);
	//Bind the actual Vertex Array Object with the name "vertex_array_obj" first,then bind and set vertex buffer objects,and then configure vertex attributes
	glBindVertexArray(vertex_array_obj);

	//Bind an actual Buffer Object to the buffer object name(id)
	glBindBuffer(GL_ARRAY_BUFFER, vertex_bufffer_obj);
	//Copy vertices data to our buffer object(Upload data to GPU) 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Create a shader object referenced by an ID
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//Attach shader source code to the shader object
	glShaderSource(vertexShader, 1, &vertexShaderString, nullptr);
	//Compile the shader
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

	//Create a shader program object referenced by returned ID
	GLuint shader_program = glCreateProgram();
	//Attach the compiled shader objects to the shader program
	glAttachShader(shader_program, vertexShader);
	glAttachShader(shader_program, fragmentShader);
	//Link attached shader objects and shader program object
	glLinkProgram(shader_program);
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader_program, 512, nullptr, infoLog);
		std::cout << "Error in linking shader program:" << infoLog << std::endl;
		return 0;
	}

	//Delete the shader objects after being linked into the shader program since we no longer need them
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//Specify what part of our input data goes to which vertex attribute
	glVertexAttribPointer(
		0, //Index of the generic vertex attribute to be modified
		3, // Number of components per generic vertex attribute
		GL_FLOAT, //Data type of each component in the array
		GL_FALSE, //Specifies whether fixed-point data values should be normalized (GL_TRUE) or converted directly as fixed-point values (GL_FALSE) when they are accessed
		3 * sizeof(float), //Byte offset between consecutive generic vertex attributes
		(void*)0);//Offset of the first component of the first generic vertex attribute in the array in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target
	//Enable the vertex attribute(Vertex attributes are disabled by default)
	glEnableVertexAttribArray(0);

	//After setting the content of vertex buffer objects and storing the vertex attribute configurations and vertex buffer objects in vertex array object,we unbind them for later use
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);



	GLuint rectVBOid, rectVAOid, rectEBOid;

	glGenVertexArrays(1, &rectVAOid);
	glBindVertexArray(rectVAOid);

	glGenBuffers(1, &rectVBOid);
	glGenBuffers(1, &rectEBOid);


	glBindBuffer(GL_ARRAY_BUFFER, rectVBOid);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vertices), rect_vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectEBOid);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rect_indices), rect_indices, GL_STATIC_DRAW);

	GLuint rectVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(rectVertexShader, 1, &rectVertexShaderString, nullptr);
	glCompileShader(rectVertexShader);

	GLuint rectFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(rectFragmentShader, 1, &rectFragmentShaderString, nullptr);
	glCompileShader(rectFragmentShader);

	GLuint rectShaderProgram = glCreateProgram();
	glAttachShader(rectShaderProgram, rectVertexShader);
	glAttachShader(rectShaderProgram, rectFragmentShader);
	glLinkProgram(rectShaderProgram);

	glDeleteShader(rectVertexShader);
	glDeleteShader(rectFragmentShader);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	/* glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 如果在unbind VAO之前unbind了GL_ELEMENT_ARRAY_BUFFER，将导致GL_ELEMENT_ARRAY_BUFFER未被设置（A VAO stores the glBindBuffer calls when the target is GL_ELEMENT_ARRAY_BUFFER.
																																            This also means it stores its unbind calls so make sure you don’t unbind the element array buffer
																																            before unbinding your VAO, otherwise it doesn’t have an EBO configured.）*/
	glBindVertexArray(0);

	//7.Start render loop
	while (!glfwWindowShouldClose(window))
	{
		count++;

		//process input
		processInput(window);

		//rendering(this is state-setting function,changes the state of the OpenGL context)
		if (count % 13 <= 6)
		{
			glClearColor(0.2f, 0.5f, 0.75f, 0.31f);
		}
		else
		{
			glClearColor(0.9f, 0.02f, 0.415f, 0.77f);
		}
		
		//specify which buffer to clear(here is color buffer)
		//this is a state-using function,performs some operations based on the current state
		glClear(GL_COLOR_BUFFER_BIT);

		//Use the shader program object and linked shader objects to rendering call
		glUseProgram(shader_program);
		//When drawing our objects,we bind the vertex array object again
		glBindVertexArray(vertex_array_obj);//As we only have a single vertex array object, there's no need to bind it every time,but we'll do so to keep things a bit more organized
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//Draw our triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);


		glUseProgram(rectShaderProgram);
		glBindVertexArray(rectVAOid);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//Indexed Drawing
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		glBindVertexArray(0);

		//check and call events
		glfwPollEvents();

		//swap front and back buffer
		glfwSwapBuffers(window);
	}

	glDeleteBuffers(1, &vertex_bufffer_obj);
	glDeleteVertexArrays(1, &vertex_array_obj);

	glDeleteBuffers(1, &rectVBOid);
	glDeleteBuffers(1, &rectEBOid);
	glDeleteVertexArrays(1, &rectVAOid);

	//8.Close and release things
	glfwTerminate();
	return 0;
}

