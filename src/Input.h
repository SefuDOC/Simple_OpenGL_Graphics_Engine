#pragma once
#include <GLFW/glfw3.h>

class Input {
public:

    static void Initialize(GLFWwindow* window);

    static bool IsKeyPressed(int key);

    static bool IsMouseButtonPressed(int button);
    static double GetMouseX();
    static double GetMouseY();
    static double GetDeltaX();
    static double GetDeltaY();

    static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
    
    static void ResetMouseDelta();
    static void UpdateCursorMode();
    static bool IsFPSMode();

private:
    static inline GLFWwindow* Window = nullptr;
    static inline double LastX = 0.0;
    static inline double LastY = 0.0;
    static inline double DeltaX = 0.0;
    static inline double DeltaY = 0.0;
    static inline bool firstMouse = true;
};