#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <sstream>

// Cube vertices (positions and normals)
GLfloat vertices[] = {
    // Positions          // Normals
    -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f, // Front face
     0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f, // Back face
     0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f, // Left face
    -0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f, // Right face
     0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f, // Bottom face
     0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f, // Top face
     0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f
};

// Camera and cube position
glm::vec3 cubePos(0.0f, 0.0f, 0.0f); // Cube position
glm::vec3 cameraPos(0.0f, 0.0f, 3.0f); // Camera position
glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f); // Camera target (view direction)

float deltaTime = 0.0f, lastFrame = 0.0f;

GLFWwindow* window;
GLuint shaderProgram, VAO, VBO;

void processInput(GLFWwindow* window) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    float moveSpeed = 2.5f * deltaTime; // Speed of movement

    // Get camera direction vectors
    glm::vec3 front = glm::normalize(cameraTarget - cameraPos); // Direction the camera is facing
    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f))); // Right vector
    glm::vec3 up = glm::normalize(glm::cross(right, front)); // Up vector

    // Handle WASD input
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cubePos += front * moveSpeed; // Move forward along camera's view direction
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cubePos -= front * moveSpeed; // Move backward along camera's view direction
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cubePos -= right * moveSpeed; // Move left along camera's right vector
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cubePos += right * moveSpeed; // Move right along camera's right vector
}
// FPS variables
float fps = 0.0f;
int frameCount = 0;
float lastTime = 0.0f;
void displayFPS() {
    // Calculate FPS
    frameCount++;
    float currentTime = glfwGetTime();
    if (currentTime - lastTime >= 1.0f) {
        fps = frameCount;
        frameCount = 0;
        lastTime = currentTime;
    }

    std::stringstream ss;
    ss << "FPS: " << fps;
    std::string fpsStr = ss.str();

    // Here you would render the FPS text in OpenGL using a texture or bitmap font method
    // For now, let's just print it to the console
    std::cout << fpsStr << std::endl;
}
GLuint compileShader(const char* source, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    // Check for compilation errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed: " << infoLog << std::endl;
    }

    return shader;
}

GLuint createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {
    GLuint vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Check for linking errors
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Program linking failed: " << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

int main() {
    if (!glfwInit()) {
        std::cerr << "GLFW initialization failed!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    window = glfwCreateWindow(800, 600, "PlayerMovement", nullptr, nullptr);
    if (!window) {
        std::cerr << "Window creation failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
        });

    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW initialization failed!" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aNormal;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    out vec3 FragPos;
    out vec3 Normal;
    void main() {
        FragPos = vec3(model * vec4(aPos, 1.0));
        Normal = mat3(transpose(inverse(model))) * aNormal;
        gl_Position = projection * view * vec4(FragPos, 1.0);
    })";

    const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    in vec3 FragPos;
    in vec3 Normal;
    uniform vec3 lightPos;
    uniform vec3 viewPos;
    void main() {
        // Phong shading model
        vec3 ambient = 0.1 * vec3(1.0, 1.0, 1.0);
        vec3 lightDir = normalize(lightPos - FragPos);
        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = 0.5 * spec * vec3(1.0, 1.0, 1.0);
        FragColor = vec4(ambient + diffuse + specular, 1.0);
    })";

    shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);

    // Create VBO and VAO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Main rendering loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 model = glm::translate(glm::mat4(1.0f), cubePos);

        glUseProgram(shaderProgram);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // Draw the cube
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices for cube
        glBindVertexArray(0);
        displayFPS();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
