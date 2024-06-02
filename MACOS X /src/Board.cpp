//Board.cpp
#include <GLUT/glut.h>
#include "../inc/Board.h"
#include "../inc/utils.h"

namespace DiskGame
{

    void Board::draw()
    {
        glMaterialfv(GL_FRONT, GL_AMBIENT, getAmbiColor());
        glMaterialfv(GL_FRONT, GL_DIFFUSE, getDiffColor());
        glMaterialfv(GL_FRONT, GL_SPECULAR, getSpecColor());
        glMaterialf(GL_FRONT, GL_SHININESS, GraphUtils::shininess);
        // ����� ������� ������� � ����
        // (���������� ����� ������� ������� ��� ���������� ������������):
        glPushMatrix();
        glTranslatef(getXCenter(), getYCenter(), getZCenter());
        GraphUtils::parallelepiped(getXSize(), getYSize(), getZSize());
        // ³��������� ������� ������� � �����:
        glPopMatrix();
    }

}