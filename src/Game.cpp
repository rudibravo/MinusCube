/* 
 * File:   Game.cpp
 * Author: rudi
 * 
 * Created on 19 de Novembro de 2009, 11:12
 */


#include <vector>
#include "Game.h"

#include <OgreLight.h>
#include <OgreWindowEventUtilities.h>

#include <OgreRay.h>

#include <OgreCEGUIRenderer.h>
#include <CEGUISystem.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>


Game::Game()
{
    m_pCubeNode = 0;
    m_maxScramble = 25;
    m_scrambleCount = 0;
    m_isScrambling = false;
    m_isMoving = false;
}

Game::~Game()
{
    delete Basic::getSingletonPtr();
}

void Game::startGame()
{
    new Basic();
    Basic::getSingletonPtr()->initOgre("Demo v1.0", this, this);

    m_bShutdown = false;
    m_bMousePressed = false;

    Basic::getSingletonPtr()->m_pLog->logMessage("Demo initialized!");

    setupGameScene();
    runGame();
}

// O original com os NULLs
//void Game::createPieces()
//{
//    m_pieces.reserve(8);
//    Piece* p0 = new Piece(m_pCubeNode, -1, -1, 1, this, NULL, WHITE, NULL, RED, NULL, RED);
//    m_pieces.push_back(p0);
//
//    Piece* p1 = new Piece(m_pCubeNode, 1, -1, 1, this, NULL, WHITE, RED, NULL, WHITE, NULL);
//    m_pieces.push_back(p1);
//
//    Piece* p2 = new Piece(m_pCubeNode, -1, 1, 1, this, WHITE, NULL, RED, NULL, NULL, RED);
//    m_pieces.push_back(p2);
//
//    Piece* p3 = new Piece(m_pCubeNode, 1, 1, 1, this, WHITE, WHITE, RED, RED, WHITE, RED);
//    m_pieces.push_back(p3);
//
//    Piece* p4 = new Piece(m_pCubeNode, -1, -1, -1, this, NULL, WHITE, NULL, RED, NULL, RED);
//    m_pieces.push_back(p4);
//
//    Piece* p5 = new Piece(m_pCubeNode, 1, -1, -1, this, WHITE, WHITE, RED, RED, WHITE, RED);
//    m_pieces.push_back(p5);
//
//    Piece* p6 = new Piece(m_pCubeNode, -1, 1, -1, this, WHITE, NULL, NULL, RED, WHITE, RED);
//    m_pieces.push_back(p6);
//
//    m_pieces.push_back(NULL);
//}

void Game::createPieces()
{
    m_pieces.reserve(8);
    Piece* p0 = new Piece(m_pCubeNode, -1, -1, 1, this, WHITE, WHITE, RED, WHITE, RED, RED);
    m_pieces.push_back(p0);

    Piece* p1 = new Piece(m_pCubeNode, 1, -1, 1, this, RED, WHITE, RED, RED, WHITE, WHITE);
    m_pieces.push_back(p1);

    Piece* p2 = new Piece(m_pCubeNode, -1, 1, 1, this, WHITE, WHITE, RED, WHITE, RED, RED);
    m_pieces.push_back(p2);

    Piece* p3 = new Piece(m_pCubeNode, 1, 1, 1, this, WHITE, WHITE, RED, RED, WHITE, RED);
    m_pieces.push_back(p3);

    Piece* p4 = new Piece(m_pCubeNode, -1, -1, -1, this, WHITE, WHITE, WHITE, RED, RED, RED);
    m_pieces.push_back(p4);

    Piece* p5 = new Piece(m_pCubeNode, 1, -1, -1, this, WHITE, WHITE, RED, RED, WHITE, RED);
    m_pieces.push_back(p5);

    Piece* p6 = new Piece(m_pCubeNode, -1, 1, -1, this, WHITE, WHITE, RED, RED, WHITE, RED);
    m_pieces.push_back(p6);

    m_pieces.push_back(NULL);
}

void Game::setupGameScene()
{
    Basic::getSingletonPtr()->m_pSceneMgr->setSkyBox(true, "Examples/StormySkyBox");

    Ogre::Light* ligth1 = Basic::getSingletonPtr()->m_pSceneMgr->createLight("Light");
    ligth1->setPosition(0, -20, 20);
    ligth1->setType(Ogre::Light::LT_DIRECTIONAL);
    ligth1->setDirection(0, 1, -1);
    ligth1->setDiffuseColour(0.5, 0.5, 0.5);

    Ogre::Light* ligth2 = Basic::getSingletonPtr()->m_pSceneMgr->createLight("Light2");
    ligth2->setPosition(0, 20, 20);
    ligth2->setType(Ogre::Light::LT_DIRECTIONAL);
    ligth2->setDirection(0, -1, -1);
    ligth2->setDiffuseColour(0.8, 0.8, 0.8);

//    Ogre::Light* ligth3 = Basic::getSingletonPtr()->m_pSceneMgr->createLight("Light3");
//    ligth3->setPosition(-20, 0, 20);
//    ligth3->setType(Ogre::Light::LT_DIRECTIONAL);
//    ligth3->setDirection(1, 0, -1);
//    ligth3->setDiffuseColour(0.2, 0.2, 0.2);
//
//    Ogre::Light* ligth4 = Basic::getSingletonPtr()->m_pSceneMgr->createLight("Light4");
//    ligth4->setPosition(20, 0, 20);
//    ligth4->setType(Ogre::Light::LT_DIRECTIONAL);
//    ligth4->setDirection(-1, 0, -1);
//    ligth4->setDiffuseColour(0.3, 0.3, 0.3);

//    ligth3->setDirection(0, -0.5, -1.25);
    //Basic::getSingletonPtr()->m_pSceneMgr->createLight("Light2")->setPosition(0,0,-100);
    //Basic::getSingletonPtr()->m_pSceneMgr->createLight("Light3")->setPosition(0,0,100);


    /*m_pCubeEntity = Basic::getSingletonPtr()->m_pSceneMgr->createEntity("Cube", "Cube.mesh");
    m_pCubeNode = Basic::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("CubeNode");
    m_pCubeNode->attachObject(m_pCubeEntity);*/

    m_pCubeNode = Basic::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("CubeNode");
    //	createCubeMesh("Cube", "Material")->convertToMesh("Cubemesh");

    createPieces();

    m_pCubeNode->pitch(Ogre::Radian(Ogre::Math::PI/12));
    m_pCubeNode->yaw(Ogre::Radian(Ogre::Math::PI/3));

    m_blankIdx = 7;

    SideVerifier* sv0 = new SideVerifier(m_pCubeNode, "sv0");
    m_sideVerifiers.push_back(sv0);
    sv0->mGroup->yaw(Ogre::Radian(Ogre::Math::PI));
    sv0->mGroup->translate(Ogre::Vector3(0, 0, -3));

    SideVerifier* sv1 = new SideVerifier(m_pCubeNode, "sv1");
    m_sideVerifiers.push_back(sv1);
    sv1->mGroup->translate(Ogre::Vector3(0, 0, 3));

    SideVerifier* sv2 = new SideVerifier(m_pCubeNode, "sv2");
    m_sideVerifiers.push_back(sv2);
    sv2->mGroup->yaw(Ogre::Radian(Ogre::Math::PI / 2));
    sv2->mGroup->translate(Ogre::Vector3(3, 0, 0));

    SideVerifier* sv3 = new SideVerifier(m_pCubeNode, "sv3");
    m_sideVerifiers.push_back(sv3);
    sv3->mGroup->yaw(Ogre::Radian((3 * Ogre::Math::PI) / 2));
    sv3->mGroup->translate(Ogre::Vector3(-3, 0, 0));

    SideVerifier* sv4 = new SideVerifier(m_pCubeNode, "sv4");
    m_sideVerifiers.push_back(sv4);
    sv4->mGroup->pitch(Ogre::Radian((3 * Ogre::Math::PI) / 2));
    sv4->mGroup->translate(Ogre::Vector3(0, 3, 0));

    SideVerifier* sv5 = new SideVerifier(m_pCubeNode, "sv5");
    m_sideVerifiers.push_back(sv5);
    sv5->mGroup->pitch(Ogre::Radian(Ogre::Math::PI / 2));
    sv5->mGroup->translate(Ogre::Vector3(0, -3, 0));

    scramble();

//    CEGUI::OgreCEGUIRenderer* mOgreCeguiRenderer = new CEGUI::OgreCEGUIRenderer(Basic::getSingletonPtr()->m_pRenderWnd, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, Basic::getSingletonPtr()->m_pSceneMgr);
//    CEGUI::System* mSystem = new CEGUI::System(mOgreCeguiRenderer);
//    mSystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
//    mSystem->setDefaultFont((CEGUI::utf8*)"BlueHighway-12");
//
//
//    CEGUI::WindowManager *win = CEGUI::WindowManager::getSingletonPtr();
//    CEGUI::Window *sheet = win->createWindow("DefaultGUISheet", "CEGUIDemo/Sheet");
//    CEGUI::Window *quit = win->createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
//    quit->setText("Quit");
//    quit->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
//    sheet->addChildWindow(quit);
//    mSystem->setGUISheet(sheet);



}

bool Game::isFinishedState()
{
    bool done = true;
    for (std::vector<SideVerifier*>::iterator it = m_sideVerifiers.begin();
            it != m_sideVerifiers.end(); it++)
    {
        done = done && (*it)->verify();
    }
    return done;
}

void Game::scramble()
{
    m_isScrambling = true;
    moveNext();
}

bool Game::move(int piece)
{
    if (m_isMoving)
    {
        return false;
    }
    bool move = false;
    Move m;
    if (m_blankIdx - piece == 4) //BACK
    {
        m = BACK;
        move = true;
    }
    else if (m_blankIdx - piece == -4) //FRONT
    {
        m = FRONT;
        move = true;
    }
    else if (m_blankIdx - piece == -2 && (m_blankIdx == 0 || m_blankIdx == 1 ||
            m_blankIdx == 4 || m_blankIdx == 5)) //DOWN
    {
        m = DOWN;
        move = true;
    }
    else if (m_blankIdx - piece == 2 && (m_blankIdx == 2 || m_blankIdx == 3 ||
            m_blankIdx == 6 || m_blankIdx == 7)) //UP
    {
        m = UP;
        move = true;
    }
    else if (m_blankIdx - piece == 1 && piece % 2 == 0) //RIGHT
    {
        m = RIGHT;
        move = true;
    }
    else if (m_blankIdx - piece == -1 && piece % 2 == 1) //LEFT
    {
        m = LEFT;
        move = true;
    }

    if (move)
    {
        m_isMoving = true;
        m_pieces[m_blankIdx] = m_pieces[piece];
        m_pieces[m_blankIdx]->move(m);
        m_pieces[piece] = NULL;
        m_blankIdx = piece;
    }
    return move;
}

void Game::moveNext()
{
    bool m = false;
    do
    {
        int i = rand() % 8;
        int lastBlank = m_blankIdx;
        if (i != m_lastBlankIdx)
        {
            m = move(i);
            if (m)
            {
                m_lastBlankIdx = lastBlank;
            }
        }
    }
    while (!m);

}

void Game::finishAnimation()
{
    m_isMoving = false;
    if (m_isScrambling)
    {
        m_scrambleCount++;
        if (m_scrambleCount < m_maxScramble)
        {
            moveNext();
        }
        else
        {
            m_isScrambling = false;
            m_scrambleCount = 0;
        }
    }
    else {
        std::cout << isFinishedState() << std::endl;
    }
}

void Game::runGame()
{
    Basic::getSingletonPtr()->m_pLog->logMessage("Start main loop...");

    double timeSinceLastFrame = 0;
    double startTime = 0;

    Basic::getSingletonPtr()->m_pRenderWnd->resetStatistics();

    while (!m_bShutdown && !Basic::getSingletonPtr()->isOgreToBeShutDown())
    {
        if (Basic::getSingletonPtr()->m_pRenderWnd->isClosed())m_bShutdown = true;

        Ogre::WindowEventUtilities::messagePump();
        
        if (Basic::getSingletonPtr()->m_pRenderWnd->isActive())
        {
            startTime = Basic::getSingletonPtr()->m_pTimer->getMillisecondsCPU();

            Basic::getSingletonPtr()->m_pKeyboard->capture();
            Basic::getSingletonPtr()->m_pMouse->capture();

            for (uint i = 0; i < m_pieces.size(); i++)
            {
                if (m_pieces[i])
                {
                    m_pieces[i]->step(timeSinceLastFrame);
                }
            }

            Basic::getSingletonPtr()->updateOgre(timeSinceLastFrame);
            Basic::getSingletonPtr()->m_pRoot->renderOneFrame();

            timeSinceLastFrame = Basic::getSingletonPtr()->m_pTimer->getMillisecondsCPU() - startTime;
        }
        else
        {
            //Sleep(1000);
        }
    }

    Basic::getSingletonPtr()->m_pLog->logMessage("Main loop quit");
    Basic::getSingletonPtr()->m_pLog->logMessage("Shutdown OGRE...");
}

bool Game::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    Basic::getSingletonPtr()->keyPressed(keyEventRef);

    if (Basic::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_F))
    {
        
    }

    return true;
}

bool Game::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    Basic::getSingletonPtr()->keyReleased(keyEventRef);

    return true;
}

bool Game::mouseMoved(const OIS::MouseEvent &evt)
{
    //m_pCamera->yaw(Degree(evt.state.X.rel * -0.1));
    //m_pCamera->pitch(Degree(evt.state.Y.rel * -0.1));

    if (m_bMousePressed)
    {
        Ogre::Quaternion q(Ogre::Degree(evt.state.X.rel * 0.3), Ogre::Vector3::UNIT_Y);
        Ogre::Quaternion r(Ogre::Degree(evt.state.Y.rel * 0.3), Ogre::Vector3::UNIT_X);
        m_pCubeNode->rotate(q * r, Ogre::Node::TS_WORLD);
    }


    return true;
}

bool Game::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if (evt.state.buttonDown(OIS::MB_Right))
    {
        m_bMousePressed = true;
    }
    else
    {
        if (m_isScrambling)
            return true;
        Ogre::MovableObject* node = getNode(evt.state.X.abs, evt.state.Y.abs);
        if (node)
        {
            std::string nodeName(node->getName().c_str());
            nodeName.resize(nodeName.size() - 1);
            std::vector<Piece*>::iterator it = m_pieces.begin();
            int c = 0;
            for (; it != m_pieces.end(); it++)
            {
                if ((*it))
                {
                    std::string pieceName((*it)->getNode()->getName().c_str());
                    pieceName.resize(pieceName.size() - 1);
                    if (!strcmp(pieceName.c_str(), nodeName.c_str()))
                    {
                        move(c);
                        break;
                    }
                }
                c++;
            }
        }
    }
    return true;
}

bool Game::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if (!evt.state.buttonDown(OIS::MB_Right))
    {
        m_bMousePressed = false;
    }
    else
    {
    }
    return true;
}

Ogre::MovableObject* Game::getNode(int mouseScreenX, int mouseScreenY)
{
    float x = ((float) mouseScreenX / Basic::getSingletonPtr()->
            m_pRenderWnd->getWidth());
    float y = ((float) mouseScreenY / Basic::getSingletonPtr()->
            m_pRenderWnd->getHeight());

    Ogre::Ray mouseRay = Basic::getSingletonPtr()->m_pSceneMgr->
            getCamera("Camera")->getCameraToViewportRay(x, y);

    Ogre::MovableObject *closestObject = Basic::getSingletonPtr()->
            queryClosestObject(&mouseRay);

    return closestObject;
}
