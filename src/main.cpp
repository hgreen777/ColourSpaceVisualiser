
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <IMGUI/imgui.h>
#include <IMGUI/imgui_impl_glfw.h>
#include <IMGUI/imgui_impl_opengl3.h>

#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

int  success;
char infoLog[512];

int iHEIGHT = 720;
int iWIDTH = 1520;

enum COLOUROPTIONS {
    _RGB,
    _HSV,
    _LAB
};

struct RGB {
    float r;
    float g;
    float b;
};

struct LAB {
    float L; // 0 -> 1
    float a; // -0.5 -> 0.5
    float b; // -0.5 -> 0.5
};

struct HSV {
    float h;
    float s;
    float v;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
RGB oklab_to_srgb(LAB c);
RGB hsv_to_srgb(HSV c);

int main()
{
    // glfw Initialisation and window creation
    // ---------------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(iWIDTH,iHEIGHT, "Colour Space Visualiser", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // GLAD Initialisation
    // -------------------
    if (!gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress )) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // Setting GLFW States
    // -------------------
    glViewport(0,0,iWIDTH,iHEIGHT); // Sets bottom left coordinate to (0,0)

    // GLFW window & input setup
    // -------------------------
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  // Allows for dynamic screen size.

    /*
    ==================================

    IMGUI INITIALISATION

    ==================================
    */
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");


    /*
    ====================================

    INITIALISE COLOURS

    ====================================
    */
    RGB rgb;
    rgb.r = 0.1f;
    rgb.g = 0.1f;
    rgb.b = 0.1f;
    LAB lab;
    HSV hsv;

    // Render Loop
    while (!glfwWindowShouldClose(window)) {

        // Reset Screen 
        // ------------
        glClearColor(rgb.r, rgb.g, rgb.b, 1.0f); // Fill the color buffer.
        glClear(GL_COLOR_BUFFER_BIT);

        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cout << "OpenGL error: " << err << std::endl;
            return 0;
        }

        // Render GUI
        // ----------
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Colour Sliders");
        ImGui::Text("Select Colour Space");

        /*
        =================================

        Colour Space Switch & Calculations

        =================================
        */
        static int selected_colourspace = _RGB;

        if (ImGui::RadioButton("RGB", selected_colourspace == _RGB))
        {
            selected_colourspace = _RGB;
        }
        if (ImGui::RadioButton("HSV", selected_colourspace == _HSV))
        {
            selected_colourspace = _HSV;
        }
        if (ImGui::RadioButton("LAB", selected_colourspace == _LAB))
        {
            selected_colourspace = _LAB;
        }

        switch (selected_colourspace)
        {
        case _RGB:
            ImGui::SliderFloat("R - Red",   &rgb.r, 0.0f, 1.0f);
            ImGui::SliderFloat("G - Green", &rgb.g, 0.0f, 1.0f);
            ImGui::SliderFloat("B - Blue",  &rgb.b, 0.0f, 1.0f);
            break;

        case _HSV:
            ImGui::SliderFloat("H - Hue",        &hsv.h, 0.0f, 360.0f);
            ImGui::SliderFloat("S - Saturation", &hsv.s, 0.0f, 1.0f);
            ImGui::SliderFloat("V - Value",      &hsv.v, 0.0f, 1.0f);

            rgb = hsv_to_srgb(hsv);
            break;

        case _LAB:
            ImGui::SliderFloat("L - Brightness",                   &lab.L,  0.0f, 1.0f);
            ImGui::SliderFloat("a - how green/red the color is",   &lab.a, -0.5f, 0.5f);
            ImGui::SliderFloat("b - how blue/yellow the color is", &lab.b, -0.5f, 0.5f);

            rgb = oklab_to_srgb(lab);
            break;
        }

        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap Buffers and check/call events
        // ----------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Sleep for 10ms -> no need to push out thousands of fps 
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}

// Allow for window resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    iWIDTH = width;
    iHEIGHT = height;
    glViewport(0,0,width,height);
}

RGB oklab_to_srgb(LAB c) 
{
    float l_ = c.L + 0.3963377774f * c.a + 0.2158037573f * c.b;
    float m_ = c.L - 0.1055613458f * c.a - 0.0638541728f * c.b;
    float s_ = c.L - 0.0894841775f * c.a - 1.2914855480f * c.b;

    float l = l_*l_*l_;
    float m = m_*m_*m_;
    float s = s_*s_*s_;

    return {
		+4.0767416621f * l - 3.3077115913f * m + 0.2309699292f * s,
		-1.2684380046f * l + 2.6097574011f * m - 0.3413193965f * s,
		-0.0041960863f * l - 0.7034186147f * m + 1.7076147010f * s,
    };
}

RGB hsv_to_srgb(HSV c)
{
    RGB out;
    
    if (c.s == 0)
    {
        out.r = c.v;
        out.g = c.v;
        out.b = c.v;

        return out;
    }

    c.h = std::fmod(c.h, 360.0f);
    float hSector = c.h / 60.0f;
    int i = hSector;
    float f = hSector - i; // Difference (remainder)

    float p,q,t;

    p = c.v * (1 - c.s);
    q = c.v * (1 - c.s * f);
    t = c.v * (1 - c.s * (1 - f));

    switch (i)
    {
        case 0:
            out.r = c.v;
            out.g = t;
            out.b = p;
            break;
        case 1:
            out.r = q;
            out.g = c.v;
            out.b = p;
            break;
        case 2:
            out.r = p;
            out.g = c.v;
            out.b = t;
            break;
        case 3:
            out.r = p;
            out.g = q;
            out.b = c.v;
            break;
        case 4:
            out.r = t;
            out.g = p;
            out.b = c.v;
            break;
        case 5:
            out.r = c.v;
            out.g = p;
            out.b = q;
            break;
    }

    return out;
}