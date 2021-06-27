#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <time.h>

#define DATA_SIZE 108

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);

        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int BeginOpenGL()
{
    GLFWwindow* window;
    unsigned int width = 1280;
    unsigned int height = 720;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(width, height, "open-gl-c++", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "GLEW Error" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    srand(time(NULL));
    float colors[DATA_SIZE];
    for (int i = 0; i < DATA_SIZE; i++) {
        colors[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);;
    }

    float vertex_data[DATA_SIZE] = { -26, -42, 144, 16, -42, 144, 26, -42, 144, 62, -42, 144, -14, -34, 144, 4, -34, 144, 34, -34, 144, 10, -11, 144, -6, 4, 144, 15, 7, 144, -2, 23, 144, -27, 48, 144, 4, 46, 144, 22, 46, 144, -52, 51, 144, -21, 51, 144, -3, 51, 144, 31, 51, 144, -26, -42, 154, 16, -42, 154, 26, -42, 154, 62, -42, 154, -14, -34, 154, 4, -34, 154, 34, -34, 154, 10, -11, 154, -6, 4, 154, 15, 7, 154, -2, 23, 154, -27, 48, 154, 4, 46, 154, 22, 46, 154, -52, 51, 154, -21, 51, 154, -3, 51, 154, 31, 51, 154 };

    unsigned int indices_og[DATA_SIZE] = { 0, 1, 2, 3, 0, 4, 1, 5, 2, 6, 4, 8, 5, 7, 6, 7, 3, 9, 8, 14, 9, 13, 10, 11, 10, 12, 11, 15, 12, 16, 13, 17, 14, 15, 16, 17, 18, 19, 20, 21, 18, 22, 19, 23, 20, 24, 22, 26, 23, 25, 24, 25, 21, 27, 26, 32, 27, 31, 28, 29, 28, 30, 29, 33, 30, 34, 31, 35, 32, 33, 34, 35, 0, 18, 1, 19, 2, 20, 3, 21, 4, 22, 5, 23, 6, 24, 7, 25, 8, 26, 9, 27, 10, 28, 11, 29, 12, 30, 13, 31, 14, 32, 15, 33, 16, 34, 17, 35 };

    unsigned int indices[204] =
    {
        0, 4, 5,
        5, 1, 0,
        8, 5, 4,
        8, 7, 5,
        2, 6, 3,
        7, 3, 6,
        7, 9, 3,

        8, 9, 7,
        8, 10, 9,

        14, 15, 11,
        14, 11, 10,
        14, 10, 8,
        16, 17, 13,
        16, 13, 12,
        12, 13, 9,
        9, 10, 12,

        19, 22, 18,
        23, 22, 19,
        23, 26, 22,
        25, 26, 23,
        21, 24, 20,
        21, 27, 24,
        27, 25, 24,

        27, 26, 25,
        28, 26, 27,

        26, 28, 29,
        29, 32, 26,
        29, 33, 32,
        31, 34, 35,
        34, 31, 30,
        30, 31, 27,
        30, 27, 28,


        17, 35, 31,
        31, 13, 17,
        13, 31, 27,

        27, 9, 13,
        9, 27, 21,
        21, 3, 9,

        3, 21, 20,
        20, 2, 3,

        2, 20, 24,
        24, 6, 2,
        6, 24, 25,
        25, 7, 6,
        7, 25, 23,
        23, 5, 7,
        5, 23, 19,
        19, 1, 5,

        1, 19, 18,
        18, 0, 1,

        18, 0, 4,
        4, 22, 18,
        8, 26, 22,
        22, 4, 8,
        14, 32, 26,
        26, 8, 14,

        14, 15, 33,
        33, 32, 14,
        15, 33, 29,
        29, 11, 15,
        11, 29, 28,
        28, 10, 11,
        16, 34, 30,
        30, 12, 16,
        12, 30, 28,
        28, 10, 12,

        16, 17, 35,
        35, 34, 16
    };

    /* Main VAO, VBO and EBO */

    unsigned int vao[5];
    unsigned int vbo[5];
    unsigned int ebo[5];

    glGenVertexArrays(5, vao);
    glGenBuffers(5, vbo);
    glGenBuffers(5, ebo);

    glBindVertexArray(vao[0]);

    /* positions */
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (GLvoid*)(0));

    /* colors */
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (GLvoid*)(0));

    glBindVertexArray(0);


    glBindVertexArray(vao[1]);

    /* positions */
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_og), indices_og, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (GLvoid*)(0));

    /* colors */
    glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (GLvoid*)(0));

    glBindVertexArray(0);

    /* Shader logic */

    const std::string vertex_source = R"(
    #version 330 core

    layout(location = 0) in vec3 in_position;
    layout(location = 1) in vec3 in_color;

    uniform mat4 projection;
    uniform mat4 view;
    uniform mat4 model;
    out vec3 v_color;

    void main()
    {
       gl_Position = projection * view * model * vec4(in_position, 1.0f);
       v_color = in_color;
    };

    )";

    const std::string fragment_source = R"(
    #version 330 core

    in vec3 v_color;
    out vec4 out_color;

    void main()
    {
	    vec3 test_color = vec3(0.16f, 0.3f, 0.5f);
	    out_color = vec4(v_color, 1.0f);
	    //out_color = vec4(test_color, 1.0f);
    };

    )";

    unsigned int shader = CreateShader(vertex_source, fragment_source);
    glUseProgram(shader);

    /* Locations & Uniforms */

    int model_loc = glGetUniformLocation(shader, "model");
    int projection_loc = glGetUniformLocation(shader, "projection");
    int view_loc = glGetUniformLocation(shader, "view");

    /* Viewport & camera */

    glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)width / (float)height, 0.1f, 600.f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, -350.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, &view[0][0]);

    /* Clear bindings */

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* Other */

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    float step = 0.0f;
    float n = 2.6333333f;

    /* Main loop */

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        step = 0.24 * glfwGetTime();

        glm::mat4 rot_x = glm::rotate(glm::mat4(1.0f), -2 * n * step, glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 rot_y = glm::rotate(glm::mat4(1.0f), -6 * n * step, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 rot_z = glm::rotate(glm::mat4(1.0f), -n * step, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -149.0f));

        glm::mat4 model = rot_z * rot_y * rot_x * translate;

        glBindVertexArray(vao[0]);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, &model[0][0]);
        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, nullptr);

        glm::mat4 model2 = glm::translate(glm::mat4(1.0f), glm::vec3(140.0f, 0.0f, 0.0f)) * model;

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, &model2[0][0]);
        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, nullptr);

        glBindVertexArray(vao[1]);

        glm::mat4 model3 = glm::translate(glm::mat4(1.0f), glm::vec3(-140.0f, 0.0f, 0.0f)) * model;

        glUniformMatrix4fv(model_loc, 1, GL_FALSE, &model3[0][0]);
        glDrawElements(GL_LINES, sizeof(indices_og), GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glfwTerminate();

    return 0;

}

int main(void)
{
    BeginOpenGL();

    return 0;
}
