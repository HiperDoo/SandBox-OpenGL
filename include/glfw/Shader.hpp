#pragma once
#include "Global.hpp"

class Shader {
private:
	GLint compileShader(const char* file_path, GLenum type);

public:
    GLint program_id;

	Shader();
	~Shader();

    void initShader(const char* vertex_path, const char* fragment_path,
        const unsigned int offset = 0);
    void bind() const;
    GLint getUniformLocation(const char* name) const;

	void setUniform1i(const GLint location, int value) const;
    void setUniformVec3f(const GLint location, const glm::vec3& value) const;
    void setUniformVec4f(const GLint location, const glm::vec4& value) const;
    void setUniformMat4f(const GLint location, const glm::mat4& value) const;
};