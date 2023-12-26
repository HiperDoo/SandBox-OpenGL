#include "VertexObject.hpp"

//=====>>> Constructor y Destructor
VertexObj::VertexObj() : array_size(0) {}
VertexObj::~VertexObj() {}

//=====>>> Funciones
void VertexObj::initObject(const float* data, const GLsizei data_size) {
    array_size = data_size / sizeof(float);

    glCreateVertexArrays(1, &vao_id);
    glCreateBuffers(1, &vbo_id);

    glNamedBufferData(vbo_id, data_size, data, GL_STATIC_DRAW);
}

void VertexObj::render() const {
    glBindVertexArray(vao_id);
    glDrawArrays(GL_TRIANGLES, 0, elements);
}

void VertexObj::render_dots() const {
    glBindVertexArray(vao_id);
    glDrawArrays(GL_POINTS, 0, elements);
}

//=====>>> Constructor y Destructor
VertexObjIndex::VertexObjIndex() : ebo_id(0) {}
VertexObjIndex::~VertexObjIndex() { glDeleteBuffers(1, &ebo_id); }

//=====>>> Funciones
void VertexObjIndex::initObject(const float* vertex_array, const GLsizei vertex_array_size,
	const GLuint* indi_array, const GLsizei indi_array_size) {
    elements = indi_array_size;
    glCreateVertexArrays(1, &vao_id);
    glCreateBuffers(1, &vbo_id);
    glCreateBuffers(1, &ebo_id);
    glVertexArrayElementBuffer(vao_id, ebo_id);

    glNamedBufferData(vbo_id, vertex_array_size, vertex_array, GL_STATIC_DRAW);

    glNamedBufferData(ebo_id, indi_array_size, indi_array, GL_STATIC_DRAW);
}

void VertexObjIndex::render() const {
    glBindVertexArray(vao_id);
    glDrawElements(GL_TRIANGLES, elements, GL_UNSIGNED_INT, nullptr);
}