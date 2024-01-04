#include "Window.hpp"

WindowManager* WindowManager::window_manager{nullptr};

//=====>>> Constructor y Destructor
WindowManager::WindowManager() :
    // Variables para el manejo de fotogramas y DT
    // ===========================================
    now_clk(0.0f), last_clk(0.0f), lag_elapsed(0.0f),
    title_fps_timer(0.0f), fps_counter(0), dt(0.0f),
    
    // Variables para la ventana de OpenGL
    // ===================================
    glfw(nullptr), title{0}, title_size(0), width(0), height(0),
    half_width(0), half_height(0), screen_width(0), screen_height(0),

    // Variables para el manejo de la cámara
    // =====================================
    worldUp{0.0f, 1.0f, 0.0f}, front{0.0f, 0.0f, 1.0f}, right{-1.0f, 0.0f, 0.0f}, up{0.0f, 1.0f, 0.0f},
    pitch(0), yaw(90), roll(0), speed(2.5f), sensitivity(100.0f), zoom(45.0f),
    aspect_ratio(0), fov_deg(glm::radians(45.0f)), first_click(GL_TRUE),
    projection(0.0f), view(0.0f), matrix(0.0f), position(0.0f),

    // Variables para el control del mouse
    // ===================================
    offsetX(0), offsetY(0), lastX(0), lastY(0), posX(0), posY(0),
    scrollX(0), scrollY(0), first_move(true)
{}

WindowManager::~WindowManager() {}

// =====>>> Funciones para la ventana de OpenGL
// ============================================
void WindowManager::window_init(const char* title, const uint32_t width, const uint32_t height) {
    this->width = width;
    this->height = height;
    this->half_width = width / 2;
    this->half_height = height / 2;

    // Retorna el número de caracteres del título (sin contar '/0').
    title_size = strnlen(title, MAX_TITLE_SZIE);
    if (title_size == MAX_TITLE_SZIE) {
        throw 1;
    }
    strncpy(this->title, title, title_size);

    // Inicialización de GLFW.
    if (glfwInit() != GLFW_TRUE) {
        cmd::console_print(cmd::opengl, cmd::error,
            "No se ha logrado inicializar GLFW."
        );
        throw EXIT_FAILURE;
    }

    // Parámetros de sugerencia para la ventana a crear (es posible que una plataforma adopte diferentes parámetros).
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifndef _WIN32
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    #endif

    // Creación de la ventana OpenGL.
    glfw = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!glfw) {
        cmd::console_print(cmd::opengl, cmd::error,
            "No se ha logrado crear la ventana."
        );
        glfwTerminate();
        throw EXIT_FAILURE;
    }
    glfwSetWindowUserPointer(glfw, this);
    glfwMakeContextCurrent(glfw);

    // Inicialización de GLEW (siempre después de inicializar GLFW).
    if (glewInit() != GLEW_OK) {
        cmd::console_print(cmd::opengl, cmd::error,
            "No se ha logrado inicializar GLEW."
        );
        throw EXIT_FAILURE;
    }

    // Definir la tasa de refresco de la ventana (en este caso sin limites ya que nosotros nos encargaremos de eso manualmente).
    glfwSwapInterval(1);

    // Define el tamaño y punto del área donde se renderiza nuestro contexto.
    glViewport(0, 0, width, height);
    camera_update_AspectRatio();

    // Obtener el tamaño del monitor (para el modo fullscreen).
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);
    this->screen_width = vidmode->width;
    this->screen_height = vidmode->height;

    // Definir las funciones a llamar según cada evento que pueda producirse (callbacks).
    window_manager = static_cast<WindowManager*>(glfwGetWindowUserPointer(glfw));
    glfwSetErrorCallback(ErrorCallback);
    glfwSetFramebufferSizeCallback(glfw, FramebufferSizeCallback);
    glfwSetCursorPosCallback(glfw, CursorPosCallback);
    //glfwSetScrollCallback(glfw, MouseScrollCallback);

    // Activación de la función Blend para así renderizar imágenes/texturas con transparencia.
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Respetar la posición de profundidad de cada objeto renderizado.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // Evitar renderizar triángulos que no miran hacia la cámara.
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);
}

void WindowManager::window_update_clock() {
    now_clk = glfwGetTime();
    const double delta_time{now_clk - last_clk};
    last_clk = now_clk;

    dt = static_cast<float>(delta_time);
    lag_elapsed += delta_time;
    title_fps_timer += delta_time;

    fps_counter++;
    if (title_fps_timer >= 1.0f) {
        sprintf(title + title_size, "%.1f", (1.0f / title_fps_timer) * fps_counter);
        glfwSetWindowTitle(glfw, title);

        title_fps_timer = 0.0f;
        fps_counter = 0;
    }
}

// =====>>> Funciones para el manejo de la cámara
// ==============================================
void WindowManager::camera_init(const glm::vec3& pos, const GLfloat fovDeg) {
    position = pos;
    fov_deg = glm::radians(fovDeg);
}

void WindowManager::camera_inputs() {
    if ((glfwGetMouseButton(glfw, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)) {
        glfwSetInputMode(glfw, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        if (first_click) {
            glfwSetCursorPos(glfw, half_width, half_height);
            first_click = GL_FALSE;
        }

        double mouseX, mouseY;
        glfwGetCursorPos(glfw, &mouseX, &mouseY);

        yaw   += static_cast<GLfloat>(mouseX - half_width) / width * sensitivity;
        pitch += static_cast<GLfloat>(half_height - mouseY) / height * sensitivity;

        pitch = pitch > 89.0f ? 89.0f : pitch;
        pitch = pitch < -89.0f ? -89.0f : pitch;

        yaw = yaw > 360.0f ? 0.0f : yaw;
        yaw = yaw < 0.0f ? 360.0f : yaw;
        
        front.x = cos(radians(yaw)) * cos(radians(pitch));
        front.y = sin(radians(pitch));
        front.z = sin(radians(yaw)) * cos(radians(pitch));
        front = glm::normalize(front);

        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));

        glfwSetCursorPos(glfw, half_width, half_height);
    } else if (glfwGetMouseButton(glfw, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        glfwSetInputMode(glfw, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        first_click = GL_TRUE;
    }

    zoom -= mouse::scrollY;
    mouse::scrollY = 0;
    zoom = zoom < 1 ? 1 : zoom;
    zoom = zoom > 45 ? 45 : zoom;
    
    const float velocity{speed * dt};
    if (glfwGetKey(glfw, GLFW_KEY_W) == GLFW_PRESS)             // Delante
        position += front * velocity;
    else if (glfwGetKey(glfw, GLFW_KEY_S) == GLFW_PRESS)        // Atrás
        position -= front * velocity;
        
    if (glfwGetKey(glfw, GLFW_KEY_D) == GLFW_PRESS)             // Derecha
        position += right * velocity;
    if (glfwGetKey(glfw, GLFW_KEY_A) == GLFW_PRESS)             // Izquierda
        position -= right * velocity;

    if (glfwGetKey(glfw, GLFW_KEY_SPACE) == GLFW_PRESS)         // Arriba
        position += up * velocity;
    if (glfwGetKey(glfw, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)    // Abajo
        position -= up * velocity;
    if (glfwGetKey(glfw, GLFW_KEY_TAB) == GLFW_PRESS)           // Rápido
        speed = 10.0f;
    else if (glfwGetKey(glfw, GLFW_KEY_D) == GLFW_RELEASE)      // Normal
        speed = 2.5f;
}

void WindowManager::camera_update_Matrix() {
    view = glm::lookAt(position, position + front, up);
    projection = glm::perspective(
        radians(zoom), aspect_ratio,
        NEAR_PLANE, FAR_PLANE
    );

    matrix = projection * view;
}

// =====>>> Funciones para el control del mouse
// ============================================