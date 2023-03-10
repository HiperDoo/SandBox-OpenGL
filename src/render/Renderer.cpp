#include "Renderer.hpp"

Camera camera(screen_width, screen_height, 45.0f, glm::vec3(0.0f, 0.0f, 2.0f));

void renderSkyBox() {
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

void renderPiramid(const float rotation) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(
        model,
        glm::radians(rotation),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    piramid.texture.bind();
    piramid.shader.bind();
    piramid.shader.setUniformMat4f(piramid.u_camera, camera.matrix);
    //piramid.shader.setUniformMat4f(piramid.u_model, model);
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

void renderSword() {
    glEnable(GL_CULL_FACE);
    sword.render();
    glDisable(GL_CULL_FACE);
}