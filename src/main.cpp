#include "App.hpp"

GLFWwindow* window{nullptr};
int screen_width{720};
int screen_height{480};

io::File_Buffer<32 * ONE_MB> vertex_buff;
io::File_Buffer<16 * ONE_KB> shader_buff;

int main(void) {
    std::ios_base::sync_with_stdio(false);

    try {
        init_GLFW();
        run_program();
    } catch (const std::system_error& e) {
        cmd::console_print(cmd::server, cmd::error,
            "Ha ocurrido un error: {}.", e.what());
        return EXIT_FAILURE;
    } catch (const std::exception& e) {
        cmd::console_print(cmd::server, cmd::error,
            "Ha ocurrido un error: {}.", e.what());
        return EXIT_FAILURE;
    } catch (const int e) {
        return e;
    } catch (...) {
        cmd::console_print(cmd::server, cmd::error,
            "Ha ocurrido un error desconocido (posiblemente de un agente de 3ros)");
        return EXIT_FAILURE;
    }

    shut_down();

    return EXIT_SUCCESS;
}