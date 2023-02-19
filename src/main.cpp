#include "App.hpp"

// Si existe una GPU dedicada, usarla (Windows)
#ifdef _WIN32
#include <windows.h>
extern "C" __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
extern "C" __declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00000001;
#endif

GLFWwindow* window{nullptr};
int screen_width{720};
int screen_height{480};

io::File_Buffer<32 * ONE_MB> vertex_buff;
io::File_Buffer<16 * ONE_KB> shader_buff;

int main(void) {
    std::ios_base::sync_with_stdio(false);
    
    // TODO: Mejorar el sistema de errores
    int error_code{EXIT_FAILURE};

    try {
        // Si existe una GPU dedicada, usarla (Linux)
        #ifndef _WIN32
        char env[] = "DRI_PRIME=1";
        putenv(env);
        #endif

        init_GLFW();
        run_program();
        error_code = EXIT_SUCCESS;
        //TODO: Posiblemente aqui
    } catch (const std::system_error& e) {
        cmd::console_print(cmd::server, cmd::error,
            "Ha ocurrido un error: {}.", e.what());
        error_code = EXIT_FAILURE;
    } catch (const std::exception& e) {
        cmd::console_print(cmd::server, cmd::error,
            "Ha ocurrido un error: {}.", e.what());
        error_code = EXIT_FAILURE;
    } catch (const int e) {
        error_code = e;
    } catch (...) {
        cmd::console_print(cmd::server, cmd::error,
            "Ha ocurrido un error desconocido (posiblemente de un agente de 3ros)");
        error_code = EXIT_FAILURE;
    }

    shut_down();

    // TODO: Mostrar este mensaje cuando se cierre la aplicacion de forma erronea
    cmd::console_print(cmd::server, cmd::info,
        "Presione Enter para salir...");
    getchar();

    return error_code;
}