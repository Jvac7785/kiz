#define GLFW_INCLUDE_NONE

GLFWwindow *window;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    keys[key] = action != GLFW_RELEASE;
}

int window_init(int width, int height, char *title)
{
    if(!glfwInit())
    {
        printf("Failed to init glfw\n");
        return -1;
    }
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if(!window)
    {
        printf("Failed to create a window\n");
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "failed glew\n");
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);
    return 1;
}

void window_update(void (*update)(float, game_memory*), game_memory *memory)
{
    double last = glfwGetTime();

    static float delta = 1.0f;

    glClearColor(0.2f, 0.4f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    (*update)(delta, memory);

    glfwSwapBuffers(window);
    glfwPollEvents();
    double now = glfwGetTime();
    delta = (float)(now - last);
}

int window_should_close()
{
    return glfwWindowShouldClose(window);
}
