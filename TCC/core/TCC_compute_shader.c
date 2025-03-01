#include <TCC/TCC_compute_shader.h>

//#include <stdio.h>

#define array_size 1024

const GLchar* computeshadersource = "#version 430 core\n"
"layout(local_size_x = 256) in;\n"
"layout(std430, binding = 0) buffer input {\n"
"float data[];\n"
"};\n"
"layout(std430, binding = 1) buffer output {\n"
"float result[];\n"
"};\n"
"void main() {\n"
"uint id = gl_globalinvocationid.x;\n"
"result[0] += data[id];"
"}\0";

GLuint computeshader, shaderprogram;
GLuint inputbuffer, outputbuffer;

float totalsum = 0.0f;

void initOpenGL(float* data) {
    // ������������� glew
    glewInit();

    // �������� ������ ��� ������� ������
    glGenBuffers(1, &inputbuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, inputbuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * array_size, data, GL_STATIC_DRAW);

    // �������� ������ ��� ����������
    glGenBuffers(1, &outputbuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, outputbuffer);
    float initialresult = 0.0f;
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float), &initialresult, GL_STATIC_DRAW);

    // �������� � ���������� �������
    computeshader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(computeshader, 1, &computeshadersource, NULL);
    glCompileShader(computeshader);

    // �������� �� ������ ����������
    GLint success;
    glGetShaderiv(computeshader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infolog[512];
        glGetShaderInfoLog(computeshader, 512, NULL, infolog);
        //fprintf(stderr, "error::shader::compile_failed\n%s\n", infolog);
    }

    // �������� ��������� ��������
    shaderprogram = glCreateProgram();
    glAttachShader(shaderprogram, computeshader);
    glLinkProgram(shaderprogram);

    // ��������� ������� � �������
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, inputbuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, outputbuffer);
}
//
void calculateSum() {
    // ������ ��������������� �������
    glUseProgram(shaderprogram);

    // ���������� �������� ������� (���������� ��������� / ������ ������)
    glDispatchCompute((array_size + 255) / 256, 1, 1); // ���������� �����

    // ���������� ���������� �� ������
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, outputbuffer);
    float* resultptr = (float*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
    if (resultptr) {
        totalsum = *resultptr; // ��������� ����������
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    }
}

int test() {
    // ������������� ������� ������
    printf("aboba: %d\n", 1);
    float data[array_size];
    for (int i = 0; i < array_size; i++) {
        data[i] = rand() % 100; // ���������� ���������� �������
    }

    initOpenGL(data);

    //calculateSum();

    //// ����������� ����������
    ////printf("total sum: %f\n", totalsum);

    //// ������������ ��������
    //glDeleteProgram(shaderprogram);
    //glDeleteShader(computeshader);
    //glDeleteBuffers(1, &inputbuffer);
    //glDeleteBuffers(1, &outputbuffer);

    return 0;
}
