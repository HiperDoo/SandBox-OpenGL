#pragma once
#include "Global.hpp"

#define ATTR_POSITION	0
#define ATTR_TEXCOORD	1
#define ATTR_NORMAL		2

/*
 * Variables y funciones que siempre deben de estar presentes en cada objeto de vértices.
 *
 * El VBO (vertex buffer objects) es el ID que apunta a la zona de memoria donde se almacenan los
 * datos de cada vértice del objeto a renderizar. Puede haber más de un VBO en un objeto a renderizar,
 * pero en este ejemplo no se requiere guardar las posiciones, texcoords, etc, en VBOs diferentes ya
 * que no estamos actualizando su contenido después de haberlos inicializados (por ejemplo, si se desea
 * actualizar constantemente los atributos de posiciones, es recomendado tener un VBO exclusivo para ello,
 * ya que la memoria de posiciones esta junta en un solo bloque).
 * 
 * Mientras que el VAO (vertex array object) es el encargado de definir cómo se acceden a los elementos
 * dentro de los VBOs (por ejemplo, tu objeto a renderizar puede requerir posiciones tridimensionales
 * mientras que tu textura sea bidimensional).
 */
class VertexCommon {
protected:
	GLuint vbo_id, vao_id;

	// Retorna la suma de todos los parámetros pares (ej. attribLength(1, 2, 3, 4) == 6).
	GLsizei attribLength() const { return 0; }
	template <class... A>
	GLsizei attribLength(GLuint, GLsizei attr_size, A... args) const { return attr_size + attribLength(args...); }

	// Implementación de todos los atributos definidos.
	void attrImpl(GLsizei) {}
	template <class... A>
	void attrImpl(GLsizei offset, GLuint attr_id, GLsizei attr_size, A... args) {
		// Selecciona qué atributo (pos, tex, nor, etc) vamos a configurar.
		glEnableVertexArrayAttrib(vao_id, attr_id);
		// Específica la organización que tendrá este atributo dentro de todo el bloque de datos.
		glVertexArrayAttribFormat(vao_id, attr_id, attr_size, GL_FLOAT, GL_FALSE, offset * sizeof(GLfloat));
		// Específica a que VBO se aplicarán estos cambios (en este caso, solo trabajamos con un VBO por VAO,
		// por lo que siempre será 0).
		glVertexArrayAttribBinding(vao_id, attr_id, 0);
		attrImpl(offset + attr_size, args...);
	}

public:
	GLsizei elements;

	VertexCommon() : vbo_id(0), vao_id(0), elements(0) {}
	~VertexCommon() { glDeleteBuffers(1, &vbo_id); glDeleteVertexArrays(1, &vao_id); }
};


class VertexObj : public VertexCommon {
private:
	GLsizei array_size;

public:
	VertexObj();
	~VertexObj();

	/*
	 * @brief Inicialización del objeto de vértices.
	 *
	 * @param data 		Buffer donde se encuentran todos datos de vértices del objeto ya ordenados según sus atributos.
	 * 
	 * @param data_size Tamaño del buffer de datos (bytes).
	 */
	void initObject(const float* data, const GLsizei data_size);

	/*
	 * @brief Define los atributos del objeto de vértices.
	 *
	 * @param arg_1 Índice del atributo.
	 * 
	 * @param arg_2 Cantidad de datos que tendrá este atributo.
	 */
	template<class... A>
	void setAttributes(A... args) {
		const GLsizei attr_length = attribLength(args...);
		attrImpl(0, args...);
		glVertexArrayVertexBuffer(vao_id, 0, vbo_id, 0, attr_length * sizeof(GLfloat));

		elements = array_size / attr_length;
	}

	// Une el VAO correspondiente y renderiza.
	void render() const;
};


class VertexObjIndex : public VertexCommon {
private:
	GLuint ebo_id;

public:
	VertexObjIndex();
	~VertexObjIndex();

	/*
	 * @brief Inicialización del objeto de vértices.
	 *
	 * @param vertex_array 			Buffer donde se encuentran todos datos de vértices del objeto ya ordenados según sus atributos.
	 * 
	 * @param vertex_array_size 	Tamaño del buffer de datos (bytes).
	 * 
	 * @param indi_array 			Buffer donde se encuentran todos datos de índices del objeto ya ordenados según sus atributos.
	 * 
	 * @param indi_array_size 		Tamaño del buffer de datos (bytes).
	 */
	void initObject(const float* vertex_array, const GLsizei vertex_array_size,
		const GLuint* indi_array, const GLsizei indi_array_size);

	/*
	 * @brief Define los atributos del objeto de vértices.
	 *
	 * @param arg_1 Índice del atributo.
	 * 
	 * @param arg_2 Cantidad de datos que tendrá este atributo.
	 */
	template<class... A>
	void setAttributes(A... args) {
		const GLsizei attr_length = attribLength(args...);
		attrImpl(0, args...);
		glVertexArrayVertexBuffer(vao_id, 0, vbo_id, 0, attr_length * sizeof(GLfloat));
	}

	// Une el VAO correspondiente y renderiza.
	void render() const;
};