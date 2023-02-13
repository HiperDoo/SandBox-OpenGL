#ifndef SHADER_HPP
#define SHADER_HPP

#include "Global.hpp"

class Shader {
private:
    //=====>>> Funciones Privadas
	GLint compileShader(const char* file_path, GLenum type);

public:
    //=====>>> Variables Publicas
    GLint program_id;

    //=====>>> Constructor y Destructor
	Shader();
	~Shader();

	//=====>>> Funciones
    void initShader(const char* vertex_path, const char* fragment_path);
    void bind() const;
    GLint getUniformLocation(const char* name) const;

    // Uniforms con bind
	void setUniform1i(const GLint location, int value) const;
    void setUniformVec3f(const GLint location, const glm::vec3& value) const;
    void setUniformVec4f(const GLint location, const glm::vec4& value) const;
    void setUniformMat4f(const GLint location, const glm::mat4& value) const;
};

#endif