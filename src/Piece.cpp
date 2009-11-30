/* 
 * File:   Piece.cpp
 * Author: rudi
 * 
 * Created on 19 de Novembro de 2009, 11:13
 */

#include "Piece.h"
#include "Game.h"

#include <OgreMath.h>

Piece::Piece(Ogre::SceneNode* parent, int x, int y, int z, Game* g,
             Color cUp, Color cDown, Color cFront, Color cBack, Color cRight, Color cLeft) :
m_X(x), m_Y(y), m_Z(z), m_originalX(x), m_originalY(y), m_originalZ(z), m_pParentNode(parent), m_game(g)
{
    m_animX = 0;
    m_animY = 0;
    m_animZ = 0;

    char buff[20];
    sprintf(buff, "Cube%d%d%dC", x, y, z);

    m_pCubeNode = m_pParentNode->createChildSceneNode(buff);

    char buff2[20];
    sprintf(buff2, "Cube%d%d%dF", x, y, z);
    Side* s0 = new Side(buff2, cFront, m_pCubeNode);
    m_sides.push_back(s0);
    s0->m_planeNode->translate(Ogre::Vector3(0, 0, SIDE_SIZE));

    char buff3[20];
    sprintf(buff3, "Cube%d%d%dD", x, y, z);
    Side* s1 = new Side(buff3, cDown, m_pCubeNode);
    m_sides.push_back(s1);
    s1->m_planeNode->pitch(Ogre::Radian(Ogre::Math::PI / 2));
    s1->m_planeNode->translate(Ogre::Vector3(0, -SIDE_SIZE, 0));

    char buff4[20];
    sprintf(buff4, "Cube%d%d%dU", x, y, z);
    Side* s2 = new Side(buff4, cUp, m_pCubeNode);
    m_sides.push_back(s2);
    s2->m_planeNode->pitch(Ogre::Radian((3 * Ogre::Math::PI) / 2));
    //s2->m_planeNode->yaw(Ogre::Radian(Ogre::Math::PI));
    s2->m_planeNode->translate(Ogre::Vector3(0, SIDE_SIZE, 0));

    char buff5[20];
    sprintf(buff5, "Cube%d%d%dB", x, y, z);
    Side* s3 = new Side(buff5, cBack, m_pCubeNode);
    m_sides.push_back(s3);
    s3->m_planeNode->yaw(Ogre::Radian((Ogre::Math::PI)));
    s3->m_planeNode->translate(Ogre::Vector3(0, 0, -SIDE_SIZE));

    char buff6[20];
    sprintf(buff6, "Cube%d%d%dR", x, y, z);
    Side* s4 = new Side(buff6, cRight, m_pCubeNode);
    m_sides.push_back(s4);
    s4->m_planeNode->yaw(Ogre::Radian(Ogre::Math::PI / 2));
    s4->m_planeNode->translate(Ogre::Vector3(SIDE_SIZE, 0, 0));

    char buff7[20];
    sprintf(buff7, "Cube%d%d%dL", x, y, z);
    Side* s5 = new Side(buff7, cLeft, m_pCubeNode);
    m_sides.push_back(s5);
    s5->m_planeNode->yaw(Ogre::Radian((3 * Ogre::Math::PI) / 2));
    s5->m_planeNode->translate(Ogre::Vector3(-SIDE_SIZE, 0, 0));

    m_pCubeNode->setPosition(m_pCubeNode->getPosition()+ (1.05f * Ogre::Vector3(x, y, z)));
}

Piece::~Piece()
{
}

Ogre::SceneNode* Piece::getNode()
{
    return m_pCubeNode;
}

void Piece::move(Move m)
{
    switch (m)
    {
    case BACK:
        m_Z--;
        m_animZ = 2.1f * -1;
        break;
    case FRONT:
        m_Z++;
        m_animZ = 2.1f * 1;
        break;
    case UP:
        m_Y++;
        m_animY = 2.1f * 1;
        break;
    case DOWN:
        m_Y--;
        m_animY = 2.1f * -1;
        break;
    case RIGHT:
        m_X++;
        m_animX = 2.1f * 1;
        break;
    case LEFT:
        m_X--;
        m_animX = 2.1f * -1;
        break;
    default:
        break;
    }
}

bool Piece::hasAnimation()
{
    return m_animX != 0 || m_animY != 0 || m_animZ != 0;
}

void Piece::step(float timeSinceLastFrame)
{
    bool hasAni = hasAnimation();
    float step = 0.008f * timeSinceLastFrame;
    if (m_animX > 0)
    {
        float animX = m_animX - step;
        if (animX > 0)
            m_animX = animX;
        else
        {
            step = m_animX;
            m_animX = 0;
        }
        m_pCubeNode->setPosition(m_pCubeNode->getPosition()+ (Ogre::Vector3(step, 0, 0)));
    }
    else if (m_animY > 0)
    {
        float animY = m_animY - step;
        if (animY > 0)
            m_animY = animY;
        else
        {
            step = m_animY;
            m_animY = 0;
        }
        m_pCubeNode->setPosition(m_pCubeNode->getPosition()+ (Ogre::Vector3(0, step, 0)));
    }
    else if (m_animZ > 0)
    {
        float animZ = m_animZ - step;
        if (animZ > 0)
            m_animZ = animZ;
        else
        {
            step = m_animZ;
            m_animZ = 0;
        }
        m_pCubeNode->setPosition(m_pCubeNode->getPosition()+ (Ogre::Vector3(0, 0, step)));
    }
    else if (m_animX < 0)
    {
        float animX = m_animX + step;
        if (animX < 0)
            m_animX = animX;
        else
        {
            step = -m_animX;
            m_animX = 0;
        }
        m_pCubeNode->setPosition(m_pCubeNode->getPosition()+ (Ogre::Vector3(-step, 0, 0)));
    }
    else if (m_animY < 0)
    {
        float animY = m_animY + step;
        if (animY < 0)
            m_animY = animY;
        else
        {
            step = -m_animY;
            m_animY = 0;
        }
        m_pCubeNode->setPosition(m_pCubeNode->getPosition()+ (Ogre::Vector3(0, -step, 0)));
    }
    else if (m_animZ < 0)
    {
        float animZ = m_animZ + step;
        if (animZ < 0)
            m_animZ = animZ;
        else
        {
            step = -m_animZ;
            m_animZ = 0;
        }
        m_pCubeNode->setPosition(m_pCubeNode->getPosition()+ (Ogre::Vector3(0, 0, -step)));

    }
    if (hasAni && !hasAnimation())
    {
        m_game->finishAnimation();
    }
}


