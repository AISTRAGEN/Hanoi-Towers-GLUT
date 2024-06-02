//Scene.cpp
#define _CRT_SECURE_NO_WARNINGS
#include <GLUT/glut.h>
#include <stdio.h>
#include <iostream>
#include "../inc/Scene.h"
#include "../inc/utils.h"
#include "../inc/Board.h"
#include "../inc/Stick.h"

namespace DiskGame
{
    using namespace GraphUtils;

    Scene::Scene(float xStep) : moveCount(0)
    {
        this->xStep = xStep;

        shapes.push_back(new Board(0.0, 0.0, 0.0, 3 * xStep, 0.1, 2 * xStep, diffGray, ambiGray, specGray));
        for (int i = 0; i < DISK_COUNT-1; i++)
        {
            double test = allocX(i);
            shapes.push_back(new Stick(allocX(i), 0.2, 0, 0.1, 0.5, 0.1, diffBrown, ambiBrown, specBrown));
        }
        shapes.push_back(new Stick(allocX(2), 0.2, 0, 0.1, 0.5, 0.1, diffBrown, ambiBrown, specBrown));

        shapes.push_back(disks[0] = new Disk(allocX(0), 0.1, 0, 0.4, 0.1, 0.4, diffRed, ambiRed, specRed, 0.05));
        shapes.push_back(disks[1] = new Disk(allocX(0), 0.2, 0, 0.3, 0.1, 0.3, diffDarkBlue, ambiDarkBlue, specDarkBlue, 0.05));
        shapes.push_back(disks[2] = new Disk(allocX(0), 0.3, 0, 0.2, 0.1, 0.2, diffGreen, ambiGreen, specGreen, 0.05));

        initialize();
    }

    Scene::~Scene()
    {
        for (int i = 0; i < shapes.size(); i++)
        {
            delete shapes[i];
        }
    }

    // Скидання масиву розташування дисків
    void Scene::resetArr()
    {
        for (int i = 0; i < DISK_COUNT; i++)
        {
            fields[0][i] = i + 1;
        }

        for (int i = 1; i < 3; i++)
        {
            for (int j = 0; j < DISK_COUNT; j++)
            {
                fields[i][j] = 0;
            }
        }
    }

    // Розрахунок координати X для стрижня
    float Scene::allocX(int i)
    {
        return  xStep * i - (DISK_COUNT - 1) * xStep  / 2;
    }

    // Розміщення дисків на початкових позиціях
    void Scene::allocateDisks()
    {
        for (int i = 0; i < DISK_COUNT; i++)
        {
            disks[fields[0][i] - 1]->setCoords(allocX(0), (i+1) * DISK_HEIGHT, 0);
        }
    }

    // Переміщення диска з одного стрижня на інший
    bool Scene::moveDisk(int xFrom, int xTo)
    {
        int temp;

        if (xFrom < 0 || xFrom > DISK_COUNT || getFirstDisk(xFrom, temp) == nullptr)
        {
            return false;
        }

        if (xTo < 0 || xTo > DISK_COUNT)
        {
            return false;
        }

        if (xFrom == xTo)
        {
            return false;
        }

        int currentHeight;
        Disk* disk_to_move = getFirstDisk(xFrom, currentHeight);

        int newHeight;
        Disk* disk_to_place_atop = getFirstDisk(xTo, newHeight);

        if (newHeight > -1)
        {
            if (fields[xFrom][currentHeight] < fields[xTo][newHeight])
            {
                return false;
            }
        }

        int diskIdentifier = fields[xFrom][currentHeight];

        disk_to_move->setXCenter(allocX(xTo));
        fields[xFrom][currentHeight] = 0;
        fields[xTo][newHeight + 1] = diskIdentifier;


        moveCount++;


        checkWinCondition();

        return true;
    }

    // Отримання першого диска на стрижні
    Disk* Scene::getFirstDisk(int x_, int& currentHeight)
    {
        if (x_ < 0 || x_ > 2)
        {
            return nullptr;
        }

        for (int i = DISK_COUNT-1; i >= 0; i--)
        {
            if (fields[x_][i] > 0)
            {
                currentHeight = i;
                return disks[fields[x_][i] - 1];
            }
        }
        currentHeight = -1;
        return nullptr;
    }

    // Підняття диска
    void Scene::upDisk(int x)
    {
        int temp;
        if(x < 0 || x > 2)
        {
            return;
        }
        Disk* first = getFirstDisk(x, temp);
        if (first != nullptr)
        {
            first->setYCenter(DISK_HEIGHT * (DISK_COUNT + 2));
        }
    }

    // Опускання диска
    void Scene::downDisk()
    {
        for (int i = 0; i < 3; i++)
        {
            int counter = 0;
            for (int j = 0; j < DISK_COUNT; j++)
            {
                if (fields[i][j] > 0)
                {
                    disks[fields[i][j] - 1]->setYCenter((counter+1) * DISK_HEIGHT);
                    counter++;
                }
            }
        }

        // Проверка на завершение игры
        checkWinCondition();
    }

    // Ініціалізація сцени
    void Scene::initialize()
    {
        resetArr();       
        allocateDisks(); 
        distZ = -2;
        angleX = 0;
        angleY = 10;
        time = 0;
        moveCount = 0;
        finish = false;
    }

    // Пошук найближчого стрижня
    bool Scene::findNearest(int x, int y, int& x1)
    {
        int viewport[4];
        double mvMatrix[16], projMatrix[16];
        double minDist = 2000;  // Ініціалізація мінімальної відстані
        int iMin = -1;

        for (int i = 0; i < 3; i++)
        {
            double wx = allocX(i);
            double wy = 0.1;
            double wz = 0;

            glGetIntegerv(GL_VIEWPORT, viewport);
            glGetDoublev(GL_MODELVIEW_MATRIX, mvMatrix);
            glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);

            double dx, dy, dz;
            gluProject(wx, wy, wz, mvMatrix, projMatrix, viewport, &dx, &dy, &dz);
            dy = viewport[3] - dy - 1;  // Переворот координати y

            double d = (x - dx) * (x - dx) + (y - dy) * (y - dy);  // Розрахунок квадрату відстані

            if (d < minDist)
            {
                minDist = d;
                iMin = i;
            }
        }


        if (minDist < 1000)
        {
            x1 = iMin;
            return true;
        } else {
            return false;
        }

    }

    // Отримання кількості кроків
    int Scene::getMoveCount() const
    {
        return moveCount;
    }

    // Перевірка умови виграшу
    void Scene::checkWinCondition()
    {
        int sum2 = 0;
        int sum3 = 0;
        for (int q = 0; q < DISK_COUNT; q++)
        {
            sum2 += fields[1][q];
            sum3 += fields[2][q];
        }

        if (sum2 == (DISK_COUNT * (DISK_COUNT + 1)) / 2 || sum3 == (DISK_COUNT * (DISK_COUNT + 1)) / 2)
        {
            finish = true;
        }
    }

    // Обробка відображення сцени
    void Scene::on_paint()
    {
        char text[128];

        if (finish)
        {
            sprintf(text, "Game over. Time: %d sec. Counts: %d   F2 - Restart game   Esc - Exit", time, moveCount);
        }
        else
        {
            sprintf(text, "F2 - Restart game   Esc - Exit              Time: %d sec. Counts: %d", time, moveCount);
        }
        glViewport(0, 0, width, height);

        float lightAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        float lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        float lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        float lightPosition[] = { 1.0f, 1.0f, 1.0f, 0.0f };

        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
        //Колір сцени після тго як виграв
        if (finish)
        {
            glClearColor(0, 0, 0.8, 0); //колір сцени після виграшу
        }
        else
        {
            glClearColor(0, 0.5, 0.5, 0);  //колір сцени
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPushMatrix();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glOrtho(0, 1, 0, 1, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glColor3f(1, 1, 0);
        drawString(GLUT_BITMAP_TIMES_ROMAN_24, text, 0.01, 0.95);
        glPopMatrix();

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        gluPerspective(60, width / height, 1, 100);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0, 0, distZ);

        glRotatef(angleX, 0.0f, 1.0f, 0.0f); 
        glRotatef(angleY, 1.0f, 0.0f, 0.0f);
        glEnable(GL_DEPTH_TEST);

        glEnable(GL_LIGHTING);

        glEnable(GL_LIGHT0);
                
        for (int i = 0; i < shapes.size(); i++)
        {
            shapes[i]->draw();
        }

        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glFlush();

        glutSwapBuffers();
    }

    // Обробка зміни розміру вікна
    void Scene::on_size(int width, int height)
    {
        this->width = width;
        if (height == 0)
            height = 1;
        this->height = height;
    }

    // Обробка натискання кнопок миші
    void Scene::on_mouse(int button, int state, int x, int y)
    {
        mouseX = x;
        mouseY = y;

        if (state == GLUT_UP)
        {
            this->button = -1;
            return;
        }

        this->button = button;

        if (finish)
        {
            return;
        }

        if (button == GLUT_LEFT_BUTTON)
        {
            if (!diskLifted && findNearest(x, y, xFrom))
            {
                upDisk(xFrom);
                diskLifted = true;
            }
            else if (diskLifted && findNearest(x, y, xTo))
            {
                moveDisk(xFrom, xTo);
                downDisk();
                diskLifted = false;
            }
        }
    }

    // Обробка переміщення миші
    void Scene::on_motion(int x, int y)
    {
        switch (button)
        {
            case GLUT_LEFT_BUTTON:
                // Ничего не делаем при перемещении с зажатой левой кнопкой мыши
                    break;
            case GLUT_RIGHT_BUTTON:
                angleX += x - mouseX;
            angleY += y - mouseY;
            mouseX = x;
            mouseY = y;
                    break;
        }
    }

    // Обробка натискання спеціальних клавіш(клавіатура)
    void Scene::on_special(int key, int x, int y)
    {
        switch (key)
        {
        case GLUT_KEY_UP:
            if (distZ > -1.7)
            {
                break;
            }
            distZ += 0.1;
            break;
        case GLUT_KEY_DOWN:
            distZ -= 0.1;
            break;
        case GLUT_KEY_F2:
            initialize();
            break;
        }
    }

    int tick = 0;

    // Обробка таймера
    void Scene::on_timer(int value)
    {
        tick++;
        if (tick >= 40)
        {
            if (!finish) 
            {
                time++;
            }
            tick = 0;   
        }
        on_paint();
    }

}