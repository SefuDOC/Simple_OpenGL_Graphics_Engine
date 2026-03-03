#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Input.h"
#include "Mesh.h"
#include "Shader.h"
#include "ObjLoader.h"
#include "Scene.h"
#include "SceneLoader.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "EventBus.h"
#include "ScriptSystem.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

void InitImGui(GLFWwindow* window)
{

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true); // conectare la sistemul de input
    ImGui_ImplOpenGL3_Init("#version 330"); // ce versiune pt shader
}

void BeginImGuiFrame()
{
    ImGui_ImplOpenGL3_NewFrame(); // se pregatesc chestii
    ImGui_ImplGlfw_NewFrame(); // sa foloseasca imputul de la glfw pe acest frame
    ImGui::NewFrame(); // creeaza un nou ui context pt frame ul curent
}

void RenderImGui()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ShutdownImGui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}


int WindowWidth = 1280;
int WindowHeight = 720;

int main() {



    if (!glfwInit()) {
        std::cerr << "Error: GLFW could not initialize!!!\n";
        return -1;
    }
    // initializeaza biblioteca glfw

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // opengl 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // core ascunde lucrurile vechi, compat le expune



    GLFWwindow* window = glfwCreateWindow(1280, 720, "Vroom Engine", nullptr, nullptr);
    if (!window) {
        std::cerr << "Error: Could not create window!!!\n";
        glfwTerminate();
        return -1;
    }
    //creare fereastra si context asociat ferestrei ( nu e inca activ contextul)

    glfwMakeContextCurrent(window);
    // leaga contextul opengl al ferestrei la threadul curent
    // basically opengl asociaza un context activ doar unui singur thread la un moment dat de timp, iar noi setam aici ca acest thread sa fie acel context activ



    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Error: GLAD could not initialize!!!\n";
        return -1;
    }
    // cauta si leaga functiile opengl disponibile dinamic pe placa video ( fiecare placa are drivere diferite ,etc si are glad grija de compatibilitate)
    // de aceea cand dai hover peste o functie gl, apare ca expands to glad_ ( glad practic e un wrapper peste)

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //pentru redimensionare

    glViewport(0, 0, 1280, 720);
    //fereastra de desenare brochocho

    glClearColor(0.f, 0.f, 0.35f, 1.0f);
    //seteaza clear values 

    Input::Initialize(window);

    //pentru gui (aparent e important sa fie dupa initialize inputul meu altfel se strica si se da override inputul meu peste logica din imgui
    InitImGui(window);

    Camera camera;

    glm::vec3 lightDirection = glm::normalize(glm::vec3(1, 0, 0));
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    //Mesh geckoMesh = ObjLoader::Load("assets/models/Gecko/Gecko.obj");
    //Mesh guyMesh = ObjLoader::Load("assets/models/FinalBaseMesh.obj");

    Shader shader("Shaders/light.VS.glsl", "Shaders/light.FS.glsl");

    Scene scene;
    //SceneNode* geckoNode = scene.root.CreateChild(&geckoMesh);
    SceneLoader::LoadSceneFromXML("assets/scenes/scene.xml", scene);
    scene.BuildNameIndex();

    ScriptSystem::Init();
    ScriptSystem::SetScene(&scene);
    ScriptSystem::LoadScript("assets/scripts/main.lua");
    ScriptSystem::AttachNodeScripts(&scene.root);

    EventBus::Push(Event{ "OnStart", {} });

    float lastFrame = 0.0f;
    float deltaTime = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processInput(window);
        Input::UpdateCursorMode();

        /*if (Input::IsKeyPressed(GLFW_KEY_W))
            std::cout << "Apasat W\n";*/

        ScriptSystem::UpdateAllScripts(deltaTime);
        EventBus::DispatchAll(ScriptSystem::HandleEvent);

        BeginImGuiFrame();
        ImGuiIO& io = ImGui::GetIO(); // iau configurarile
        if (Input::IsFPSMode()) // daca sunt in modul camera, vreau sa nu pot apasa pe buton; asadar modific valorile citite de la mouse din imgui
        {
            
            io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);;
            io.MouseDrawCursor = false;

            io.MouseDown[0] = false;
            io.MouseDown[1] = false;        
            io.WantCaptureMouse = false;
        }
        ImGui::Begin("Engine Stats");
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Separator();
        if (ImGui::Button("Randomize Light Direction")) // buton care atunci cand e apasat schimba directia luminii
        {
            lightDirection = glm::normalize(glm::vec3(
                (float(rand()) / RAND_MAX) * 2.0f - 1.0f,
                (float(rand()) / RAND_MAX) * 2.0f - 1.0f,
                (float(rand()) / RAND_MAX) * 2.0f - 1.0f
            ));
        }
        ImGui::End();

        camera.Update(deltaTime);
        Input::ResetMouseDelta();


        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
            static_cast<float>(WindowWidth) / static_cast<float>(WindowHeight),
            0.1f, 100.0f);


        shader.Use();

        GLuint viewLoc = glGetUniformLocation(shader.ID(), "View");
        GLuint projLoc = glGetUniformLocation(shader.ID(), "Projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


        //ilum params
        GLuint viewPosLoc = glGetUniformLocation(shader.ID(), "viewPos");
        glm::vec3 camPos = camera.GetPosition();
        glUniform3fv(viewPosLoc, 1, glm::value_ptr(camPos));

        GLuint lightDirLoc = glGetUniformLocation(shader.ID(), "lightDirection");
        GLuint lightColorLoc = glGetUniformLocation(shader.ID(), "lightColor");
        glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDirection));
        glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

        GLuint baseColorLoc = glGetUniformLocation(shader.ID(), "uBaseColor");

        scene.Draw(shader, view , projection);

        RenderImGui();

        glfwSwapBuffers(window);
        // basically double buffering

        glfwPollEvents();
        // verifica evenimentele de la sistemul de operare ( mouse, tastatura, redimensionare, inchidere ,etc) si le pune in coada interna de evenimente de la glfw
        // mai exista si glfwWaitEvents care blocheaza executia pana cand apare un eveniment nou

    }
    ShutdownImGui();

    ScriptSystem::Shutdown();

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        EventBus::Push(Event{ "OnKeyDown", { (int)GLFW_KEY_SPACE } });
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    WindowWidth = width;
    WindowHeight = height;
}