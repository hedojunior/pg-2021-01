/* Hello Triangle - código adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle 
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gr'afico - Jogos Digitais - Unisinos
 * Versão inicial: 7/4/2017
 * Última atualização em 15/08/2020
 *
 */
 
#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>


// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupShader();
int setupForVertices(GLfloat vertices[]);
int setupForVerticesWithIndices(GLfloat vertices[], GLuint indices[]);
int setupFloor();
int setupLeftTreeTop();
int setupLeftTreeLog();
int setupHouseStructure();
int setupHouseDoor();
int setupHouseCeiling();
int setupRightTreeTop();
int setupRightTreeLog();

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 400, HEIGHT = 300;

// Código fonte do Vertex Shader (em GLSL): ainda hardcoded
const GLchar* vertexShaderSource = "#version 430\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";

//Códifo fonte do Fragment Shader (em GLSL): ainda hardcoded
const GLchar* fragmentShaderSource = "#version 430\n"
"uniform vec4 inputColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = inputColor;\n"
"}\n\0";

// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Triangulo!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	// Compilando e buildando o programa de shader
	GLuint shaderID = setupShader();

	//Buffers
	GLuint floorVAO = setupFloor();

	GLuint leftTreeTopVAO = setupLeftTreeTop();
	/*GLuint leftTreeLogVAO = setupLeftTreeLog();

	GLuint houseStructureVAO = setupHouseStructure();
	GLuint houseDoorVAO = setupHouseDoor();
	GLuint houseCeilingVAO = setupHouseCeiling();

	GLuint rightTreeTopVAO = setupRightTreeTop();
	GLuint rightTreeLogVAO = setupRightTreeLog();*/

	// Enviando a cor desejada (vec4) para o fragment shader
	// Utilizamos a variáveis do tipo uniform em GLSL para armazenar esse tipo de info
	// que não está nos buffers
	GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");
	assert(colorLoc > -1);
	glUseProgram(shaderID);
	

	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window)) 
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(1, 1, 1, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(3);

		//drawcalls
		// Chão
		glUniform4f(colorLoc, 0.1f, 0.5f, 0.0f, 1.0f);
		glUseProgram(shaderID);
		glBindVertexArray(floorVAO);
		glDrawArrays(GL_LINES, 0, 2);
		glBindVertexArray(0);

		glUniform4f(colorLoc, 0.1f, 0.5f, 0.0f, 1.0f);
		glUseProgram(shaderID);
		glBindVertexArray(leftTreeTopVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);


		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &floorVAO);
	glDeleteVertexArrays(1, &leftTreeTopVAO);
	
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

//Esta função está basntante hardcoded - objetivo é compilar e "buildar" um programa de
// shader simples e único neste exemplo de código
// O código fonte do vertex e fragment shader está nos arrays vertexShaderSource e
// fragmentShader source no iniçio deste arquivo
// A função retorna o identificador do programa de shader
int setupShader()
{
	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Checando erros de compilação (exibição via log no terminal)
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Checando erros de compilação (exibição via log no terminal)
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Linkando os shaders e criando o identificador do programa de shader
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Checando por erros de linkagem
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

int setupForVertices(GLfloat vertices[])
{
	GLuint VBO, VAO;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return VAO;
}

int setupForVerticesWithIndices(GLfloat vertices[], GLuint indices[])
{
	GLuint VBO, VAO, EBO;

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

	return VAO;
}

int setupFloor() 
{
	GLfloat vertices[] = 
	{
		-1, 0, 0,
		1, 0, 0
	};

	return setupForVertices(vertices);
}

int setupLeftTreeTop() 
{
	GLfloat vertices[] = 
	{
		-0.4, 0.2, 0,
		-0.3, 0.8, 0,
		-0.2, 0.2, 0
	};

	return setupForVertices(vertices);
}

int setupLeftTreeLog() 
{
	GLfloat vertices[] = 
	{
		-0.35, 0, 0,
		-0.35, 0.2, 0,
		-0.25, 0.2, 0,
		-0.25, 0, 0
	};

	GLuint indices[] = 
	{
		0, 1, 2,
		0, 2, 3
	};

	return setupForVerticesWithIndices(vertices, indices);
}

int setupHouseStructure() 
{
	GLfloat vertices[] = 
	{
		0.25, 0, 0,
		0.25, 0.4, 0,
		0.5, 0.4, 0,
		0.5, 0, 0
	};

	return setupForVertices(vertices);
}

int setupHouseDoor() 
{
	GLfloat vertices[] = 
	{
		0.35, 0, 0,
		0.35, 0.18, 0,
		0.40, 0.18, 0,
		0.40, 0, 0
	};

	GLuint indices[] = 
	{
		0, 1, 2,
		0, 2, 3
	};

	return setupForVerticesWithIndices(vertices, indices);
}

int setupHouseCeiling() 
{
	GLfloat vertices[] = 
	{
		0.25, 0.4, 0,
		0.375, 0.6, 0,
		0.5, 0.4, 0
	};

	return setupForVertices(vertices);
}

int setupRightTreeTop() 
{
	GLfloat vertices[] = 
	{
		0.6, 0.4 , 0,
		0.7, 0.9, 0,
		0.8, 0.4, 0
	};

	return setupForVertices(vertices);
}

int setupRightTreeLog() 
{
	GLfloat vertices[] = 
	{
		0.65, 0, 0,
		0.65, 0.4, 0,
		0.70, 0.4, 0,
		0.70, 0, 0
	};

	GLuint indices[] = 
	{
		0, 1, 2,
		0, 2, 3
	};

	return setupForVerticesWithIndices(vertices, indices);
}

