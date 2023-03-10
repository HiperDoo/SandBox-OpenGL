#include "Shader.hpp"

//=====>>> Constructor y Destructor
Shader::Shader() : program_id(0) {}
Shader::~Shader() { if (program_id) glDeleteProgram(program_id); }

//=====>>> Funciones Privadas
GLint Shader::compileShader(const char* file_path, GLenum type) {
    GLint shader_id = glCreateShader(type);
    glShaderSource(shader_id, 1, &shader_buff.data, nullptr);
    glCompileShader(shader_id);
    
    GLint result{0};
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
    if (!result) {
        GLchar InfoLog[256];
        glGetShaderInfoLog(shader_id, sizeof(InfoLog), NULL, InfoLog);
        printf("%s", InfoLog); // TODO: limpiar e incorporar mensaje en el cmd::console
        glDeleteShader(shader_id);
        cmd::console_print(cmd::opengl, cmd::error,
            "Falla al compilar shader (archivo: '{}').", file_path);
        throw 1;
    }

    return shader_id;
}

//=====>>> Funciones
void Shader::initShader(const char* vertex_path, const char* fragment_path,
    const unsigned int offset) {
    // Copia de los archivos vertex y fragment en un buffer para luego ser compilados
    shader_buff.load_from_file(vertex_path, offset);
    GLint vs = compileShader(vertex_path, GL_VERTEX_SHADER);
    shader_buff.load_from_file(fragment_path, offset);
    GLint fs = compileShader(fragment_path, GL_FRAGMENT_SHADER);

    program_id = glCreateProgram();
    
    // Se enlazan ambos programas en uno solo, un shader completo
    glAttachShader(program_id, vs);
    glAttachShader(program_id, fs);
    glLinkProgram(program_id);
    glValidateProgram(program_id);

    glDeleteShader(vs);
    glDeleteShader(fs);
}

void Shader::bind() const {
    glUseProgram(program_id);
}

GLint Shader::getUniformLocation(const char* name) const {
    GLint location = glGetUniformLocation(program_id, name);
    if (location == -1) {
        cmd::console_print(cmd::opengl, cmd::error,
            "No se ha encontrado el uniforme '{}'.", name);
        throw 1;
    }
    return location;
}

// Uniforms con bind
void Shader::setUniform1i(const GLint location, int value) const {
    glProgramUniform1i(program_id, location, value);
}

void Shader::setUniformVec3f(const GLint location, const glm::vec3& value) const {
    glProgramUniform3fv(program_id, location, 1, &value[0]);
}

void Shader::setUniformVec4f(const GLint location, const glm::vec4& value) const {
    glProgramUniform4fv(program_id, location, 1, &value[0]);
}

void Shader::setUniformMat4f(const GLint location, const glm::mat4& value) const {
    glProgramUniformMatrix4fv(program_id, location, 1, GL_FALSE, &value[0][0]);
}