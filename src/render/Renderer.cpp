#include "Renderer.hpp"

Camera camera(glm::vec3(0.0f, 0.0f, 2.0f), 45.0f);

/*void renderSkyBox() {
    glm::mat4 view = glm::mat4(glm::mat3(camera.view));

    glDepthFunc(GL_LEQUAL);
    skybox.texture.bind();
    skybox.shader.bind();
    skybox.shader.setUniformMat4f(skybox.u_view, view);
    skybox.shader.setUniformMat4f(skybox.u_projection, camera.projection);
    skybox.vertexObj.render();
    glDepthFunc(GL_LESS);
}

void renderCppImg() {
    cpp_img.texture.bind();
    cpp_img.shader.bind();
    cpp_img.shader.setUniformMat4f(
        cpp_img.u_camera, camera.matrix
    );
    glEnable(GL_BLEND);
    cpp_img.vertexObj.render();
    glDisable(GL_BLEND);
}

void renderPiramid() {
    piramid.texture.bind();
    piramid.shader.bind();
    piramid.shader.setUniformMat4f(piramid.u_camera, camera.matrix);
    piramid.shader.setUniformVec3f(piramid.u_cameraPos, camera.position);
    piramid.vertexObj.render();
}

void renderFloor() {
    w_floor.shader.setUniformVec3f(w_floor.u_cameraPos, camera.position);
    w_floor.shader.setUniformMat4f(w_floor.u_camera, camera.matrix);
    w_floor.shader.bind();
    w_floor.texture.bind(0);
    w_floor.specular.bind(1);
    w_floor.vertexObj.render();
}

void renderLight() {
    light.shader.setUniformMat4f(light.u_camera, camera.matrix);
    light.shader.bind();
    light.vertexObj.render();
}

void renderSword(const float rotation) {
    glm::mat4 matrix(1.0f);
    matrix = glm::translate(matrix, objects.m_UVs[0].position);
    matrix = glm::rotate(matrix, glm::radians(rotation), objects.m_UVs[0].rotation);
    matrix = glm::scale(matrix, objects.m_UVs[0].scale);
    objects.m_UVs[0].matrix = matrix;

    glEnable(GL_CULL_FACE);
    objects.render();
    glDisable(GL_CULL_FACE);
}*/

void renderer(const float rotation) {
    ////////// Sky Box
    glm::mat4 view = glm::mat4(glm::mat3(camera.view));
    skybox.texture.bind();
    skybox.shader.bind();
    skybox.shader.setUniformMat4f(skybox.u_view, view);
    skybox.shader.setUniformMat4f(skybox.u_projection, camera.projection);
    glDepthFunc(GL_LEQUAL);
    skybox.vertexObj.render();
    glDepthFunc(GL_LESS);


    ////////// Cpp Image
    cpp_img.texture.bind();
    cpp_img.shader.bind();
    cpp_img.shader.setUniformMat4f(cpp_img.u_camera, camera.matrix);
    glEnable(GL_BLEND);
    cpp_img.vertexObj.render();
    glDisable(GL_BLEND);


    ////////// Piramid
    piramid.texture.bind();
    piramid.shader.bind();
    piramid.shader.setUniformMat4f(piramid.u_camera, camera.matrix);
    piramid.shader.setUniformVec3f(piramid.u_cameraPos, camera.position);
    piramid.vertexObj.render();


    ////////// Floor
    w_floor.shader.setUniformVec3f(w_floor.u_cameraPos, camera.position);
    w_floor.shader.setUniformMat4f(w_floor.u_camera, camera.matrix);
    w_floor.shader.bind();
    w_floor.texture.bind(0);
    w_floor.specular.bind(1);
    w_floor.vertexObj.render();


    ////////// Light
    light.shader.setUniformMat4f(light.u_camera, camera.matrix);
    light.shader.bind();
    light.vertexObj.render();


    ////////// Model
    glm::mat4 matrix(1.0f);
    matrix = glm::translate(matrix, objects.m_UVs[0].position);
    matrix = glm::rotate(matrix, glm::radians(rotation), objects.m_UVs[0].rotation);
    matrix = glm::scale(matrix, objects.m_UVs[0].scale);
    objects.m_UVs[0].matrix = matrix;

    glEnable(GL_CULL_FACE);
    objects.render();
    glDisable(GL_CULL_FACE);

    ground.render();
}