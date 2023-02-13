#ifndef VERTEXOBJECT_HPP
#define VERTEXOBJECT_HPP

#include "Global.hpp"

#define ATTR_POSITION 0
#define ATTR_TEXCOORD 1
#define ATTR_NORMAL 2

class VertexCommon {
protected:
	//=====>>> Variables Privadas
	GLuint vbo_id, vao_id;
	GLsizei attr_length;

	//=====>>> Funciones Privadas
	GLsizei attribLength() const { return 0; }
	template <class... A>
	GLsizei attribLength(GLuint, GLsizei attr_size, A... args) const { return attr_size + attribLength(args...); }

	void attrImpl(GLsizei) {}
	template <class... A>
	void attrImpl(GLsizei offset, GLuint attr_id, GLsizei attr_size, A... args) {
		glEnableVertexArrayAttrib(vao_id, attr_id);
		glVertexArrayAttribFormat(vao_id, attr_id, attr_size, GL_FLOAT, GL_FALSE, offset * sizeof(GLfloat));
		glVertexArrayAttribBinding(vao_id, attr_id, 0);
		attrImpl(offset + attr_size, args...);
	}

public:
	//=====>>> Variables Publicas
	GLsizei elements;

	//=====>>> Constructor y Destructor
	VertexCommon() : vbo_id(0), vao_id(0), attr_length(0), elements(0) {}
	~VertexCommon() { glDeleteBuffers(1, &vbo_id); glDeleteVertexArrays(1, &vao_id); }
};

class VertexObj
	: public VertexCommon {
private:
	//=====>>> Variables Privadas
	GLsizei array_size;

public:
	//=====>>> Constructor y Destructor
	VertexObj();
	~VertexObj();

	//=====>>> Funciones
	void initObject(const float* array, const GLsizei num_elm);

	template<class... A>
	void setAttributes(A... args) {
		attr_length = attribLength(args...);
		attrImpl(0, args...);
		glVertexArrayVertexBuffer(vao_id, 0, vbo_id, 0, attr_length * sizeof(GLfloat));

		elements = array_size / attr_length;
	}

	void render() const;
};

class VertexObjIndex
	: public VertexCommon {
private:
	//=====>>> Variables Privadas
	GLuint ebo_id;

public:
	//=====>>> Constructor y Destructor
	VertexObjIndex();
	~VertexObjIndex();

	//=====>>> Funciones
	void initObject(const float* vertex_array, const GLsizei vertex_num,
		const GLuint* indi_array, const GLsizei indi_num);

	template<class... A>
	void setAttributes(A... args) {
		attr_length = attribLength(args...);
		attrImpl(0, args...);
		glVertexArrayVertexBuffer(vao_id, 0, vbo_id, 0, attr_length * sizeof(GLfloat));
	}

	void render() const;
};

#endif