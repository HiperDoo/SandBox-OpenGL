#include "App.hpp"

// Usar GPU dedicada en caso de presentarse (Windows).
#ifdef _WIN32
#include <windows.h>
extern "C" __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
extern "C" __declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00000001;
#endif

int main(void) {
    std::ios_base::sync_with_stdio(false);
    
    // TODO: Mejorar el sistema de errores
    int exit_code{EXIT_FAILURE};

    try {
        #ifndef _WIN32
        try {
            // Usar GPU dedicada en caso de presentarse (Linux).
            char env[] = "DRI_PRIME=1";
            putenv(env);
        } catch (...) {
            cmd::console_print(cmd::client, cmd::warn,
                "Falla al elegir GPU dedicada del sistema (Error: DRI_PRIME=1)."
            );
        }
        #endif

        init_GLFW();
        run_program();

        cmd::console_print(cmd::client, cmd::debug,
            "Programa finalizado de forma exitosa :>"
        );

        return EXIT_SUCCESS;
    } catch (const std::system_error& e) {
        cmd::console_print(cmd::client, cmd::error,
            "Ha ocurrido un error: {}.",
            e.what()
        );

        exit_code = EXIT_FAILURE;
    } catch (const std::exception& e) {
        cmd::console_print(cmd::client, cmd::error,
            "Ha ocurrido un error: {}.",
            e.what()
        );

        exit_code = EXIT_FAILURE;
    } catch (const int e) {
        exit_code = e;
    } catch (...) {
        cmd::console_print(cmd::client, cmd::error,
            "Ha ocurrido un error desconocido (posiblemente de un agente de 3ros)."
        );

        exit_code = EXIT_FAILURE;
    }

    shut_down();

    cmd::console_print(cmd::client, cmd::debug,
        "Presione \"Enter\" para salir..."
    );
    getchar();

    return exit_code;
}