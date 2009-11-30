/* 
 * File:   SideVerifier.cpp
 * Author: rudi
 * 
 * Created on 20 de Novembro de 2009, 12:41
 */

#include "SideVerifier.h"
#include "Side.h"

#include <OgreManualObject.h>

SideVerifier::SideVerifier(Ogre::SceneNode* parent, std::string name)
{
    mGroup = parent->createChildSceneNode(name + "group");

    mVerifier0 = createSceneNode(mGroup, name + "0", Ogre::Vector3(-SIDE_SIZE, -SIDE_SIZE, 0));
    mVerifier1 = createSceneNode(mGroup, name + "1", Ogre::Vector3(-SIDE_SIZE, SIDE_SIZE, 0));
    mVerifier2 = createSceneNode(mGroup, name + "2", Ogre::Vector3(SIDE_SIZE, -SIDE_SIZE, 0));
    mVerifier3 = createSceneNode(mGroup, name + "3", Ogre::Vector3(SIDE_SIZE, SIDE_SIZE, 0));
}

SideVerifier::~SideVerifier()
{
}

Ogre::SceneNode* SideVerifier::createSceneNode(Ogre::SceneNode* parent, std::string name,
                                               Ogre::Vector3 defaultTranslate)
{
    Ogre::SceneNode* node = parent->createChildSceneNode(name);
    node->translate(defaultTranslate);

    Ogre::ManualObject* myManualObject = new Ogre::ManualObject("manual1" + name);

    myManualObject->begin("manual1Material", Ogre::RenderOperation::OT_LINE_LIST);
    myManualObject->position(0, 0, 0);
    myManualObject->position(0, 0, -1);
    myManualObject->end();

    //DEBUG
//    node->attachObject(myManualObject);

    return node;
}

bool SideVerifier::verify()
{
    Ogre::MovableObject *closestObject0 = getClosestObject(mVerifier0);
    Ogre::MovableObject *closestObject1 = getClosestObject(mVerifier1);
    Ogre::MovableObject *closestObject2 = getClosestObject(mVerifier2);
    Ogre::MovableObject *closestObject3 = getClosestObject(mVerifier3);

    if (closestObject0 && closestObject1 && closestObject2 && closestObject3)
    {
        ColorProperty* c0 = dynamic_cast<ColorProperty*> (closestObject0->getUserObject());
        ColorProperty* c1 = dynamic_cast<ColorProperty*> (closestObject1->getUserObject());
        ColorProperty* c2 = dynamic_cast<ColorProperty*> (closestObject2->getUserObject());
        ColorProperty* c3 = dynamic_cast<ColorProperty*> (closestObject3->getUserObject());

        return (c0->color == c1->color && c0->color == c2->color && c0->color == c3->color);
    }
    else
    {
        std::cout << "ERROR" << std::endl;
        std::cout << closestObject0 << std::endl;
        std::cout << closestObject1 << std::endl;
        std::cout << closestObject2 << std::endl;
        std::cout << closestObject3 << std::endl;
    }
    return false;
}

Ogre::MovableObject* SideVerifier::getClosestObject(Ogre::SceneNode* verifier)
{
    Ogre::Vector3 v = (verifier->getWorldPosition() + (verifier->getWorldOrientation() * Ogre::Vector3(0, 0, -10)));
    Ogre::Ray ray(verifier->getWorldPosition(), v);
    Ogre::MovableObject *closestObject = Basic::getSingletonPtr()->
            queryClosestObject(&ray);
    return closestObject;
}
