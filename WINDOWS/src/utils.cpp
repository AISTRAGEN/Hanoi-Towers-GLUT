//utils.cpp
#include <algorithm>
#include <time.h>
#include <stdlib.h>
#include <gl/glut.h>
#include "../inc/utils.h"

namespace GraphUtils
{
    // Визначення кольорів:
    float diffWhite[] = { 1.0f, 1.0f, 1.0f };
    float ambiWhite[] = { 0.8f, 0.8f, 0.8f };
    float specWhite[] = { 1.0f, 1.0f, 1.0f };

    float diffBlue[] = { 0.0f, 0.0f, 0.6f };
    float ambiBlue[] = { 0.1f, 0.1f, 0.2f };
    float specBlue[] = { 0.2f, 0.2f, 0.8f };

    float diffGray[] = { 0.6f, 0.6f, 0.6f };
    float ambiGray[] = { 0.2f, 0.2f, 0.2f };
    float specGray[] = { 0.8f, 0.8f, 0.8f };

    float diffRed[] = { 0.6f, 0.0f, 0.0f };
    float ambiRed[] = { 0.2f, 0.1f, 0.1f };
    float specRed[] = { 0.8f, 0.2f, 0.2f };

    float diffYellow[] = { 0.9f, 0.9f, 0 };
    float ambiYellow[] = { 0.2f, 0.2f, 0.1f };
    float specYellow[] = { 1.0f, 1.0f, 0.2f };

    float diffGreen[] = { 0, 0.5f, 0 };
    float ambiGreen[] = { 0.1f, 0.2f, 0.1f };
    float specGreen[] = { 0.2f, 0.7f, 0.2f };

    float diffDarkBlue[] = { 0.9f, 0.2f, 0 };
    float ambiDarkBlue[] = { 0.2f, 0.2f, 0.2f };
    float specDarkBlue[] = { 0.1f, 0.1f, 0.8f };

    float diffLightBlue[] = { 0, 0.6f, 0.9f };
    float ambiLightBlue[] = { 0.2f, 0.2f, 0.2f };
    float specLightBlue[] = { 0.8f, 0.8f, 0.8f };

    float diffViolet[] = { 0.5f, 0, 0.5f };
    float ambiViolet[] = { 0.2f, 0.2f, 0.2f };
    float specViolet[] = { 0.8f, 0.8f, 0.8f };

    float diffBrown[] = { 0.6f, 0.3f, 0.1f };
    float ambiBrown[] = { 0.2f, 0.1f, 0.05f };
    float specBrown[] = { 0.7f, 0.4f, 0.2f };


    // Малювання паралелепіпеда
    void parallelepiped(float length, float width, float height)
    {
        glBegin(GL_QUAD_STRIP);
       
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-length / 2, -width / 2, -height / 2);
        glVertex3f(-length / 2, -width / 2, height / 2);
        glVertex3f(-length / 2, width / 2, -height / 2);
        glVertex3f(-length / 2, width / 2, height / 2);

        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(length / 2, width / 2, -height / 2);
        glVertex3f(length / 2, width / 2, height / 2);

  
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(length / 2, -width / 2, -height / 2);
        glVertex3f(length / 2, -width / 2, height / 2);

       
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(-length / 2, -width / 2, -height / 2);
        glVertex3f(-length / 2, -width / 2, height / 2);
        glEnd();

        glBegin(GL_QUADS);
       
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-length / 2, -width / 2, height / 2);
        glVertex3f(-length / 2, width / 2, height / 2);
        glVertex3f(length / 2, width / 2, height / 2);
        glVertex3f(length / 2, -width / 2, height / 2);

        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(-length / 2, -width / 2, -height / 2);
        glVertex3f(-length / 2, width / 2, -height / 2);
        glVertex3f(length / 2, width / 2, -height / 2);
        glVertex3f(length / 2, -width / 2, -height / 2);
        glEnd();
    }

    // Відображення рядка тексту вказаним шрифтом у вказаній позиції
    void drawString(void* font, const char* text, float x, float y)
    {
        if (!text) 
        {
            return;
        }
        // Встановлення позиції тексту:
        glRasterPos2f(x, y);
        while (*text)
        {
            // Рядок виводиться посимвольно:
            glutBitmapCharacter(font, *text);
            text++;
        }
    }
}