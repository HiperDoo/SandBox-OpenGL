#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

//=====>>> Constructor y Destructor
template <GLenum T>
Texture<T>::Texture() : program_id(0) {}

template <GLenum T>
Texture<T>::~Texture() { glDeleteTextures(1, &program_id); }

//=====>>> Funciones Privadas
template <GLenum T>
void Texture<T>::setSizeFilter(const GLenum min_filter, const GLenum mag_filter) const {
	// Encargado de definir cual de los dos filtros (GL_NEAREST y GL_LINEAR) se aplicara
	// ya sea al momento de escalar hacia arriba o hacia abajo.
	glTextureParameteri(program_id, GL_TEXTURE_MIN_FILTER, min_filter);
	glTextureParameteri(program_id, GL_TEXTURE_MAG_FILTER, mag_filter);
}

template <GLenum T>
void Texture<T>::setWrapFilter(const GLenum filter) const {
	// Encargado de definir el comportamiento del renderizado de la textura cuando
	// este excede su zona (0,0 - 1,1). Afecta al rededor de la textura y no internamente.
	if (T == GL_TEXTURE_2D) {
		glTextureParameteri(program_id, GL_TEXTURE_WRAP_S, filter);
		glTextureParameteri(program_id, GL_TEXTURE_WRAP_T, filter);
	} else if (T == GL_TEXTURE_2D_ARRAY || T == GL_TEXTURE_CUBE_MAP) {
		glTextureParameteri(program_id, GL_TEXTURE_WRAP_S, filter);
		glTextureParameteri(program_id, GL_TEXTURE_WRAP_T, filter);
		glTextureParameteri(program_id, GL_TEXTURE_WRAP_R, filter);
	}
}

//=====>>> Funciones
template <GLenum T>
void Texture<T>::bind() const {
	glBindTexture(T, program_id);
}

template <GLenum T>
void Texture<T>::bind(GLenum slot) const {
	glBindTextureUnit(slot, program_id);
}

template <GLenum T>
void Texture<T>::loadImage(const char* file_path, const GLenum internat_format, const GLenum format,
	const GLenum min_filter, const GLenum mag_filter, const GLenum filter) {
	unsigned char* buffer{nullptr};
	int width{0}, height{0}, BPP{0};

	stbi_set_flip_vertically_on_load(1);
	buffer = stbi_load(file_path, &width, &height, &BPP, 4);
	if (!buffer) {
		cmd::console_print(cmd::server, cmd::error,
            "Falla al abrir imagen (archivo: '{}').", file_path);
		throw 1;
	}

    glCreateTextures(T, 1, &program_id);

	// glTextureStorage#D cumple el mismo proposito que glTexImage#D, solo que con un tamano
	// fijo y estatico (mas eficiente cuando se trata de tamanos ^2).
	if (T == GL_TEXTURE_2D) {
		glTextureStorage2D(program_id, 1, internat_format, width, height);
		glTextureSubImage2D(program_id, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, buffer);
	} else if (T == GL_TEXTURE_2D_ARRAY) {
		glTextureStorage3D(program_id, 1, internat_format, width, height, 0);
		glTextureSubImage3D(program_id, 0, 0, 0, 0, width, height, 0, format, GL_UNSIGNED_BYTE, buffer);
	}
	setSizeFilter(min_filter, mag_filter);
	setWrapFilter(filter);

	stbi_image_free(buffer);
}

template <GLenum T>
void Texture<T>::loadImageMipMap(const char* file_path, const GLsizei depth, const GLenum slot,
	const GLenum min_filter, const GLenum mag_filter, const GLenum filter) {
	unsigned char* buffer{nullptr};
	int width{0}, height{0}, BPP{0};

	stbi_set_flip_vertically_on_load(1);
	buffer = stbi_load(file_path, &width, &height, &BPP, 4);
	if (!buffer) {
		cmd::console_print(cmd::server, cmd::error,
            "Falla al abrir imagen (archivo: '{}').", file_path);
		throw 1;
	}

    GLsizei levels{1};
	// TODO: describir exactamente que hace que cosa
    height /= depth;
    //TODO: Averiguar que raios hace esto???
    while ((width | height) >> levels) levels++;
    //while ((width | height | depth) >> levels) levels++;

    glCreateTextures(T, 1, &program_id);

	if (T == GL_TEXTURE_2D) {
		glTextureStorage2D(program_id, levels, GL_RGBA8, width, height);
		glTextureSubImage2D(program_id, 0, 0, 0, width, height, slot, GL_UNSIGNED_BYTE, buffer);
	} else if (T == GL_TEXTURE_2D_ARRAY) {
		glTextureStorage3D(program_id, levels, GL_RGBA8, width, height, depth);
		glTextureSubImage3D(program_id, 0, 0, 0, 0, width, height, depth, slot, GL_UNSIGNED_BYTE, buffer);
	}
	setSizeFilter(min_filter, mag_filter);
	setWrapFilter(filter);
	glGenerateTextureMipmap(program_id);

	stbi_image_free(buffer);
}

template <GLenum T>
void Texture<T>::loadImagesCubeMap(const char** file_paths, const GLenum min_filter,
	const GLenum mag_filter, const GLenum filter) {
	unsigned char* buffer{nullptr};
	int width{0}, height{0}, BPP{0};
	stbi_info(file_paths[0], &width, &height, &BPP);
	
    glCreateTextures(T, 1, &program_id);
	glTextureStorage2D(program_id, 1, GL_RGB8, width, height);

	stbi_set_flip_vertically_on_load(true);
	for (int i = 0; i < 6; i++) {
		int curr_width{0}, curr_height{0};
		buffer = stbi_load(file_paths[i], &curr_width, &curr_height, &BPP, 0);
		stbi_set_flip_vertically_on_load(false);
		if (!buffer) {
			cmd::console_print(cmd::server, cmd::error,
				"Falla al abrir imagen (archivo: '{}').", file_paths[i]);
			throw 1;
		}
		if (width != curr_width && height != curr_height) {
			cmd::console_print(cmd::server, cmd::error,
				"Todas los tamanos de las imagenes del CubeMap deben ser iguales.");
			throw 1;
		}

		glTextureSubImage3D(
			program_id,
			0, 0, 0, i,
			width, height, 1,
			GL_RGB, GL_UNSIGNED_BYTE,
			buffer
		);
		stbi_image_free(buffer);
	}

	setSizeFilter(min_filter, mag_filter);
	setWrapFilter(filter);
}

template class Texture<GL_TEXTURE_2D>;
template class Texture<GL_TEXTURE_2D_ARRAY>;
template class Texture<GL_TEXTURE_CUBE_MAP>;