#include <iostream>
#include<glm/glm.hpp>
#include <vector>
#include <windows.h>
#include <glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "../../Basic Rendering/Camera/Camera.h"


class Line {

public:

    int shaderProgram;
    unsigned int VBO, VAO;
    std::vector<float> vertices;
    glm::vec3 startPoint;
    glm::vec3 endPoint;
    glm::mat4 MVP = glm::mat4(1.0);
    glm::vec3 lineColor;
    float defaultLineWidth;
    Line() {
        const char* vertexShaderSource = "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "uniform mat4 MVP;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = MVP * vec4(aPos, 1.0);\n"
            "}\0";
        const char* fragmentShaderSource = "#version 330 core\n"
            "out vec4 FragColor;\n"
            "uniform vec3 color;\n"
            "void main()\n"
            "{\n"
            "   FragColor = vec4(color, 1.0f);\n"
            "}\n\0";

        // vertex shader
        int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        // check for shader compile errors

        // fragment shader
        int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        // check for shader compile errors

        // link shaders
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        // check for linking errors

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        vertices = {
             startPoint.x, startPoint.y, startPoint.z,
             endPoint.x, endPoint.y, endPoint.z,

        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

    }

    int setMVP(glm::mat4 mvp) {
        MVP = mvp;
        return 0;
    }

    int setColor(glm::vec3 color) {
        lineColor = color;
        return 0;
    }

    int setWidth(float width) {
        defaultLineWidth = width;
        return 0;
    }

    int draw(Camera cam, glm::vec3 start, glm::vec3 end) {

        //if (axis == "x") {
        //    endPoint = location + glm::vec3(1, 0, 0);
        //}
        //else if (axis == "y") {
        //    endPoint = location + glm::vec3(0, 1, 0);
        //}
        //else if (axis == "z") {
        //    endPoint = location + glm::vec3(0, 0, 1);
        //}

        vertices = {
             start.x, start.y, start.z,
             end.x, end.y, end.z,

        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);


        glDisable(GL_DEPTH_TEST);

        glUseProgram(shaderProgram);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "MVP"), 1, GL_FALSE, &MVP[0][0]);
        glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, &lineColor[0]);

        glBindVertexArray(VAO);

        float distanceToLine = glm::distance(glm::vec3((start.x + end.x) / 2, (start.y + end.y) / 2, (start.z + end.z) / 2), cam.Position);

        // Adjust the line width based on the distance
        float lineWidth = (distanceToLine < 2.5f) ? defaultLineWidth : defaultLineWidth * (2.5f / distanceToLine);

        glLineWidth(lineWidth); // Set the line width

        glDrawArrays(GL_LINES, 0, 2);

        // Re-enable the depth test
        glEnable(GL_DEPTH_TEST);
        return 0;
    }

    ~Line() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(shaderProgram);
    }
};