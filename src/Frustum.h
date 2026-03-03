#pragma once
#include <glm/glm.hpp>

class Frustum {
public:
    // https://stackoverflow.com/questions/12836967/extracting-view-frustum-planes-gribb-hartmann-method
    //  normal.xyz, d
    glm::vec4 planes[6]; // 0 = stanga , 1 = dreapta , 2 = jos, 3 = sus, 4 = near, 5 = far, macarena

    void ExtractPlanes(const glm::mat4& VP) {
        // stanga
        planes[0] = glm::vec4(
            VP[0][3] + VP[0][0],
            VP[1][3] + VP[1][0],
            VP[2][3] + VP[2][0],
            VP[3][3] + VP[3][0]
        );
        // dreapta
        planes[1] = glm::vec4(
            VP[0][3] - VP[0][0],
            VP[1][3] - VP[1][0],
            VP[2][3] - VP[2][0],
            VP[3][3] - VP[3][0]
        );
        // jos
        planes[2] = glm::vec4(
            VP[0][3] + VP[0][1],
            VP[1][3] + VP[1][1],
            VP[2][3] + VP[2][1],
            VP[3][3] + VP[3][1]
        );
        // sus
        planes[3] = glm::vec4(
            VP[0][3] - VP[0][1],
            VP[1][3] - VP[1][1],
            VP[2][3] - VP[2][1],
            VP[3][3] - VP[3][1]
        );
        // near
        planes[4] = glm::vec4(
            VP[0][3] + VP[0][2],
            VP[1][3] + VP[1][2],
            VP[2][3] + VP[2][2],
            VP[3][3] + VP[3][2]
        );
        // far
        planes[5] = glm::vec4(
            VP[0][3] - VP[0][2],
            VP[1][3] - VP[1][2],
            VP[2][3] - VP[2][2],
            VP[3][3] - VP[3][2]
        );

        // normalizam fiecare plan
        for (int i = 0; i < 6; i++) {
            float length = glm::length(glm::vec3(planes[i]));
            planes[i] /= length;
        }
    }

    // testul pentru sfera
    bool IsSphereInside(const glm::vec3& center, float radius) const {
        for (int i = 0; i < 6; i++) {
            float distance = glm::dot(glm::vec3(planes[i]), center) + planes[i].w;
            // ecuatia planului ax + by + cz + d = 0; unde (a,b,c) normala planului si d e distanta
            // dot efectiv realizeaza ax + by + cz; 
            // daca ax + by + cz + d > 0 => punctul e in fata planului; < 0 => e in spate
            // acum pentru centru, calculam distanta de la centru la plan si daca centru + raza < 0 atunci obiectul este clar in afara;
            // de mentionat ca distance este o distanta cu semn in functie de pozitia fata de plan ( e ciudat sa zici distanta negativa :D)
            // daca gasim un plan in care sfera este in afara => return false; e in afara fustrumului
            if (distance + radius < 0)
                return false; // e in afara
        }
        return true; // se vede cel putin un degetel
    }
};