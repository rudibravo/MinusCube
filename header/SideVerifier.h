/* 
 * File:   SideVerifier.h
 * Author: rudi
 *
 * Created on 20 de Novembro de 2009, 12:41
 */

#ifndef _SIDEVERIFIER_H
#define	_SIDEVERIFIER_H

#include <OgreRay.h>
#include <OgreSceneNode.h>

class SideVerifier {
public:
    SideVerifier(Ogre::SceneNode* parent, std::string name);
    virtual ~SideVerifier();

    Ogre::SceneNode* mGroup;

    bool verify();

private:
    std::vector<Ogre::Ray*> mRays;
    Ogre::SceneNode* mVerifier0;
    Ogre::SceneNode* mVerifier1;
    Ogre::SceneNode* mVerifier2;
    Ogre::SceneNode* mVerifier3;

    Ogre::SceneNode* createSceneNode(Ogre::SceneNode* parent, std::string param,
            Ogre::Vector3 defaultTranslate);

    Ogre::MovableObject* getClosestObject(Ogre::SceneNode* verifier);
};

#endif	/* _SIDEVERIFIER_H */

