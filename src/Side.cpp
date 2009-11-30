/* 
 * File:   Side.cpp
 * Author: rudi
 * 
 * Created on 19 de Novembro de 2009, 11:13
 */

#include "Side.h"

#include <OgreManualObject.h>

/*
Static data members are not part of objects of a given class type;
they are separate objects. As a result, the declaration of a static
data member is not considered a definition. The data member is declared
in class scope, but definition is performed at file scope.
These static members have external linkage.
 */
Ogre::ManualObject* Side::s_whitePlane;
Ogre::ManualObject* Side::s_redPlane;

Side::Side(Ogre::String name, Color color, Ogre::SceneNode* cubeNode) :
m_cubeNode(cubeNode)
{
    if (s_whitePlane == NULL)
    {
        s_whitePlane = createPlaneMesh("white_plane", "Material.001");
        s_whitePlane->convertToMesh("whiteplane");
    }
    if (s_redPlane == NULL)
    {
        s_redPlane = createPlaneMesh("red_plane", "Material");
        s_redPlane->convertToMesh("redplane");
    }

    std::string entity;
    switch (color)
    {
    case WHITE:
        entity = "whiteplane";
        break;
    case RED:
        entity = "redplane";
        break;
    }
    m_planeEntity = Basic::getSingletonPtr()->m_pSceneMgr->createEntity(name, entity);
    m_planeEntity->setUserObject(new ColorProperty(color));
    m_planeNode = m_cubeNode->createChildSceneNode(name);
    m_planeNode->attachObject(m_planeEntity);
}

Side::~Side()
{
}

Ogre::ManualObject* Side::createPlaneMesh(Ogre::String name, Ogre::String matName)
{

    Ogre::ManualObject* plane = new Ogre::ManualObject(name);
    plane->begin(matName);

    plane->position(-SIDE_SIZE, -SIDE_SIZE, 0.0);
    plane->textureCoord(1, 0);
    plane->position(SIDE_SIZE, SIDE_SIZE, 0.0);
    plane->textureCoord(0, 1);
    plane->position(-SIDE_SIZE, SIDE_SIZE, 0.0);
    plane->textureCoord(1, 1);
    plane->position(SIDE_SIZE, -SIDE_SIZE, 0.0);
    plane->textureCoord(0, 0);

    plane->triangle(0, 1, 2);
    plane->triangle(3, 1, 0);
    plane->end();

    return plane;

}


