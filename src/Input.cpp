#include "Input.h"
#include <iostream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

static inline bool FPSMode = false;

bool Input::IsFPSMode() {
    return FPSMode;
}

void Input::UpdateCursorMode()
{
    static bool prevRightPressed = false;
    bool rightPressed = glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

    if (rightPressed && !prevRightPressed) {
        FPSMode = !FPSMode;
        if (FPSMode) {
            glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else {
            glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        
        firstMouse = true;
    }

    prevRightPressed = rightPressed;


}

void Input::Initialize(GLFWwindow* window)
{
    Window = window;
    glfwSetCursorPosCallback(window, MouseCallback);

}

bool Input::IsKeyPressed(int key)
{
    return glfwGetKey(Window, key) == GLFW_PRESS;
}

bool Input::IsMouseButtonPressed(int button)
{
    return glfwGetMouseButton(Window, button) == GLFW_PRESS;
}

double Input::GetMouseX() { return LastX; }
double Input::GetMouseY() { return LastY; }
double Input::GetDeltaX() { return DeltaX; }
double Input::GetDeltaY() { return DeltaY; }

void Input::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (ImGui::GetIO().WantCaptureMouse)
        return;
    if (!FPSMode) return;

    if (firstMouse) {
        LastX = xpos;
        LastY = ypos;
        firstMouse = false;
    }

    double dx = xpos - LastX;
    double dy = LastY - ypos; // in glfw , 0,0 e in stanga sus

    DeltaX += dx;
    DeltaY += dy;

    LastX = xpos;
    LastY = ypos;

}

void Input::ResetMouseDelta()
{
    DeltaX = 0.0;
    DeltaY = 0.0;
}