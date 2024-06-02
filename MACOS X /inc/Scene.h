//Scene.h
#ifndef Scene_h
#define Scene_h

#include "Shape.h"
#include "Disk.h"
#include <vector>

namespace DiskGame
{

    const int DISK_COUNT = 3; // Кількість дисків у грі
    const float DISK_HEIGHT = 0.1; // Висота кожного диска

    class Scene
    {
        std::vector<Shape*> shapes; // "гнучкий" масив указівників на елементи гри
        int button;
        float angleX, angleY;   // поточний кут повороту сцени
        float mouseX, mouseY, mouseZ;   // поточні координати
        float width, height;     // Розміри вікна
        float distZ;     // відстань по осі Z до сцени
        bool finish;    // ознака того, що гру завершено
        Disk* disks[DISK_COUNT];     // масив указівників на диски
        float xStep, yStep;  // відстань між окремими стрижнями
        int time;   // поточний час у секуднах
        int fields[3][DISK_COUNT];   // масив, у якому відображається розміщення дисків:
        int xFrom, zFrom;   // індекси стрижня, з якого починається пересування
        int xTo, zTo;   // індекси стрижня, яким закінчується пересування
        bool diskLifted; // ознака того, що диск піднято
        int moveCount; //кількість кроків

    public:
        Scene(float xStep);
        ~Scene();
        void on_paint();
        void on_size(int width, int height);
        void on_mouse(int button, int state, int x, int y);
        void on_motion(int x, int y);
        void on_special(int key, int x, int y);
        void on_timer(int value);
        int getMoveCount() const;

        Disk* getFirstDisk(int x, int& currentHeight);
    private:
        void initialize();
        void allocateDisks();
        bool moveDisk(int xFrom, int xTo);
        void upDisk(int x);
        void downDisk();
        bool findNearest(int x, int z, int& x1);
        void resetArr();
        float allocX(int i);
        void checkWinCondition();
     
    };

}

#endif