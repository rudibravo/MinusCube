/* 
 * File:   Side.h
 * Author: rudi
 *
 * Created on 19 de Novembro de 2009, 11:13
 */

#ifndef _SIDE_H
#define	_SIDE_H

#include "Basic.h"

#define SIDE_SIZE 1.0f

enum Color {
    RED, WHITE
};

class ColorProperty : public Ogre::UserDefinedObject {
public:
    Color color;

    ColorProperty(Color c) : color(c) { }
};

class Side {
public:
    Color m_color;
    Ogre::Entity* m_planeEntity;
    Ogre::SceneNode* m_planeNode;
    Ogre::SceneNode* m_cubeNode;

    static Ogre::ManualObject* s_whitePlane;
    static Ogre::ManualObject* s_redPlane;

    Side(Ogre::String name, Color color, Ogre::SceneNode* cubeNode);
    ~Side();

    Ogre::ManualObject* createPlaneMesh(Ogre::String name, Ogre::String matName);
};

#endif	/* _SIDE_H */

