/* 
 * File:   Basic.h
 * Author: rudi
 *
 * Created on 19 de Novembro de 2009, 11:11
 */

#ifndef _BASIC_H
#define	_BASIC_H

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreOverlay.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OgreStringConverter.h>
#include <OgreMaterialManager.h>
#include <OgreWindowEventUtilities.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <OgreCEGUIRenderer.h>
#include <CEGUISystem.h>

class Basic : public Ogre::Singleton<Basic>, OIS::KeyListener, OIS::MouseListener, Ogre::WindowEventListener {
public:
    Basic();
    ~Basic();

    void initOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener = 0, OIS::MouseListener *pMouseListener = 0);
    void initCEGUI();
    void shutdownOgre();
    void updateOgre(double timeSinceLastFrame);
    void updateStats();
    void moveCamera();
    void getInput();

    bool isOgreToBeShutDown()const {
        return m_bShutDownOgre;
    }

    bool keyPressed(const OIS::KeyEvent &keyEventRef);
    bool keyReleased(const OIS::KeyEvent &keyEventRef);

    bool mouseMoved(const OIS::MouseEvent &evt);
    bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

    void windowClosed(Ogre::RenderWindow* rw);

    Ogre::MovableObject* queryClosestObject(Ogre::Ray* ray);


    void GetMeshInformation(const Ogre::MeshPtr mesh,
            size_t &vertex_count,
            Ogre::Vector3* &vertices,
            size_t &index_count,
            unsigned long* &indices,
            const Ogre::Vector3 &position,
            const Ogre::Quaternion &orient,
            const Ogre::Vector3 &scale);

    Ogre::Root* mRoot;
    Ogre::SceneManager* mSceneMgr;
    Ogre::RenderWindow* mRenderWnd;
    Ogre::Camera* mCamera;
    Ogre::Viewport* mViewport;
    Ogre::Log* mLog;
    Ogre::Timer* mTimer;

    OIS::InputManager* mInputMgr;
    OIS::Keyboard* mKeyboard;
    OIS::Mouse* mMouse;

    CEGUI::OgreCEGUIRenderer* mOgreCeguiRenderer;
    CEGUI::System* mSystem;

private:
    Basic(const Basic&);
    Basic & operator=(const Basic&);

    Ogre::Overlay* mDebugOverlay;
    Ogre::Overlay* mInfoOverlay;
    int m_iNumScreenShots;

    bool m_bShutDownOgre;

    Ogre::Vector3 m_TranslateVector;
    Ogre::Real m_MoveSpeed;
    Ogre::Degree m_RotateSpeed;
    float m_MoveScale;
    Ogre::Degree m_RotScale;
};

#endif	/* _BASIC_H */

