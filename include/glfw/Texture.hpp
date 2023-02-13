#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "Global.hpp"

template <GLenum T>
class Texture {
private:
	//=====>>> Funciones Privadas
	void setSizeFilter(const GLenum min_filter, const GLenum mag_filter) const;
	void setWrapFilter(const GLenum filter) const;

public:
	//=====>>> Variables Publicas
	GLuint program_id;

	//=====>>> Constructor y Destructor
	Texture();
	~Texture();

	//=====>>> Funciones
	void bind() const;
	void bind(GLenum slot) const;

	void loadImage(const char* file_path, const GLenum internat_format, const GLenum format,
		const GLenum min_filter, const GLenum mag_filter, const GLenum filter);
	void loadImageMipMap(const char* file_path, const GLsizei depth, const GLenum slot,
		const GLenum min_filter, const GLenum mag_filter, const GLenum filter);
	void loadImagesCubeMap(const char** file_paths, const GLenum min_filter,
		const GLenum mag_filter, const GLenum filter);
};

using Texture2D = Texture<GL_TEXTURE_2D>;
using Texture2DArray = Texture<GL_TEXTURE_2D_ARRAY>;
using TextureCubeMap = Texture<GL_TEXTURE_CUBE_MAP>;

#endif