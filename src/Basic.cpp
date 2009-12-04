/* 
 * File:   Basic.cpp
 * Author: rudi
 * 
 * Created on 19 de Novembro de 2009, 11:11
 */



#include "Basic.h"

#include <OgreFrameListener.h>
#include <OgreSceneQuery.h>
#include <OgreSubMesh.h>
#include <OgreWindowEventUtilities.h>

#include <CEGUISchemeManager.h>

using namespace Ogre;

template<> Basic* Ogre::Singleton<class Basic>::ms_Singleton = 0;

void Basic::windowClosed(Ogre::RenderWindow* rw)
{
    //std::cout << "BBBB" << std::endl;
    mMouse = 0;
    m_bShutDownOgre = true;
}

Basic::Basic()
{
    m_MoveSpeed = 0.1;
    m_RotateSpeed = 0.3;

    m_bShutDownOgre = false;
    m_iNumScreenShots = 0;

    mRoot = 0;
    mSceneMgr = 0;
    mRenderWnd = 0;
    mCamera = 0;
    mViewport = 0;
    mLog = 0;
    mTimer = 0;

    mInputMgr = 0;
    mKeyboard = 0;
    mMouse = 0;

    mDebugOverlay = 0;
    mInfoOverlay = 0;
}

void Basic::initOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener, OIS::MouseListener *pMouseListener)
{
    new Ogre::LogManager();

    mLog = Ogre::LogManager::getSingleton().createLog("OgreLogfile.log", true, true, false);
    //mLog->setDebugOutputEnabled(true);

    mRoot = new Ogre::Root();

    mRoot->showConfigDialog();
    mRenderWnd = mRoot->initialise(true, wndTitle);

    mSceneMgr = mRoot->createSceneManager(ST_GENERIC, "SceneManager");
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    mCamera = mSceneMgr->createCamera("Camera");
    mCamera->setPosition(Vector3(0, 0, 15));
    mCamera->lookAt(Vector3(0, 0, 0));
    mCamera->setNearClipDistance(1);

    mViewport = mRenderWnd->addViewport(mCamera);
    mViewport->setBackgroundColour(ColourValue(0.8, 0.7, 0.6, 1.0));

    mCamera->setAspectRatio(Real(mViewport->getActualWidth()) / Real(mViewport->getActualHeight()));

    mViewport->setCamera(mCamera);

    unsigned long hWnd = 0;
    OIS::ParamList paramList;
    mRenderWnd->getCustomAttribute("WINDOW", &hWnd);

    paramList.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd)));

#if defined OIS_WIN32_PLATFORM
    paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
    paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
    paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
    paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
#elif defined OIS_LINUX_PLATFORM
    paramList.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
    paramList.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
    paramList.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
    paramList.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
#endif

    mInputMgr = OIS::InputManager::createInputSystem(paramList);

    mKeyboard = static_cast<OIS::Keyboard*> (mInputMgr->createInputObject(OIS::OISKeyboard, true));
    mMouse = static_cast<OIS::Mouse*> (mInputMgr->createInputObject(OIS::OISMouse, true));

    mMouse->getMouseState().height = mRenderWnd->getHeight();
    mMouse->getMouseState().width = mRenderWnd->getWidth();

    Ogre::WindowEventUtilities::addWindowEventListener(mRenderWnd, this);

    if (pKeyListener == 0)
        mKeyboard->setEventCallback(this);
    else
        mKeyboard->setEventCallback(pKeyListener);

    if (pMouseListener == 0)
        mMouse->setEventCallback(this);
    else
        mMouse->setEventCallback(pMouseListener);

    Ogre::String secName, typeName, archName;
    Ogre::ConfigFile cf;
    cf.load("resources.cfg");

    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(0);
    Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    mTimer = new Ogre::Timer();
    mTimer->reset();

    mDebugOverlay = OverlayManager::getSingleton().getByName("Core/DebugOverlay");
    //mDebugOverlay->show();

    mRenderWnd->setActive(true);
}

void Basic::initCEGUI()
{
    mOgreCeguiRenderer = new CEGUI::OgreCEGUIRenderer(Basic::getSingletonPtr()->mRenderWnd, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, Basic::getSingletonPtr()->mSceneMgr);
    mSystem = new CEGUI::System(mOgreCeguiRenderer);

    CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLookSkin.scheme");

    mSystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
    mSystem->setDefaultFont((CEGUI::utf8*)"BlueHighway-12");
}

Basic::~Basic()
{
    delete mKeyboard;
    if (mMouse)
        delete mMouse;

    OIS::InputManager::destroyInputSystem(mInputMgr);

    delete mRoot;
}

bool Basic::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    mLog->logMessage("Basic::keyPressed");

    if (mKeyboard->isKeyDown(OIS::KC_ESCAPE))
    {
        m_bShutDownOgre = true;
        return true;
    }

    if (mKeyboard->isKeyDown(OIS::KC_SYSRQ))
    {
        std::ostringstream ss;
        ss << "screenshot_" << ++m_iNumScreenShots << ".png";
        mRenderWnd->writeContentsToFile(ss.str());
        return true;
    }

    if (mKeyboard->isKeyDown(OIS::KC_M))
    {
        static int mode = 0;

        if (mode == 2)
        {
            mCamera->setPolygonMode(PM_SOLID);
            mode = 0;
        }
        else if (mode == 0)
        {
            mCamera->setPolygonMode(PM_WIREFRAME);
            mode = 1;
        }
        else if (mode == 1)
        {
            mCamera->setPolygonMode(PM_POINTS);
            mode = 2;
        }
    }

    if (mKeyboard->isKeyDown(OIS::KC_O))
    {
        if (mDebugOverlay)
        {
            if (!mDebugOverlay->isVisible())
                mDebugOverlay->show();
            else
                mDebugOverlay->hide();
        }
    }

    return true;
}

bool Basic::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    return true;
}

bool Basic::mouseMoved(const OIS::MouseEvent &evt)
{
    //mCamera->yaw(Degree(evt.state.X.rel * -0.1));
    //mCamera->pitch(Degree(evt.state.Y.rel * -0.1));

    return true;
}

bool Basic::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    return true;
}

bool Basic::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    return true;
}

void Basic::updateOgre(double timeSinceLastFrame)
{
    m_MoveScale = m_MoveSpeed * timeSinceLastFrame;
    m_RotScale = m_RotateSpeed * timeSinceLastFrame;

    m_TranslateVector = Vector3::ZERO;

    getInput();
    moveCamera();

    updateStats();
}

void Basic::updateStats()
{
    static String currFps = "Current FPS: ";
    static String avgFps = "Average FPS: ";
    static String bestFps = "Best FPS: ";
    static String worstFps = "Worst FPS: ";
    static String tris = "Triangle Count: ";
    static String batches = "Batch Count: ";

    OverlayElement* guiAvg = OverlayManager::getSingleton().getOverlayElement("Core/AverageFps");
    OverlayElement* guiCurr = OverlayManager::getSingleton().getOverlayElement("Core/CurrFps");
    OverlayElement* guiBest = OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
    OverlayElement* guiWorst = OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");

    const RenderTarget::FrameStats& stats = mRenderWnd->getStatistics();
    guiAvg->setCaption(avgFps + StringConverter::toString(stats.avgFPS));
    guiCurr->setCaption(currFps + StringConverter::toString(stats.lastFPS));
    guiBest->setCaption(bestFps + StringConverter::toString(stats.bestFPS)
                        + " " + StringConverter::toString(stats.bestFrameTime) + " ms");
    guiWorst->setCaption(worstFps + StringConverter::toString(stats.worstFPS)
                         + " " + StringConverter::toString(stats.worstFrameTime) + " ms");

    OverlayElement* guiTris = OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
    guiTris->setCaption(tris + StringConverter::toString(stats.triangleCount));

    OverlayElement* guiBatches = OverlayManager::getSingleton().getOverlayElement("Core/NumBatches");
    guiBatches->setCaption(batches + StringConverter::toString(stats.batchCount));

    OverlayElement* guiDbg = OverlayManager::getSingleton().getOverlayElement("Core/DebugText");
    guiDbg->setCaption("");
}

void Basic::moveCamera()
{
    if (mKeyboard->isKeyDown(OIS::KC_LSHIFT)) mCamera->moveRelative(m_TranslateVector);
    mCamera->moveRelative(m_TranslateVector / 10);
}

void Basic::getInput()
{
    /*if(mKeyboard->isKeyDown(OIS::KC_A))
    {
            m_TranslateVector.x = -m_MoveScale;
    }

    if(mKeyboard->isKeyDown(OIS::KC_D))
    {
            m_TranslateVector.x = m_MoveScale;
    }

    if(mKeyboard->isKeyDown(OIS::KC_W))
    {
            m_TranslateVector.z = -m_MoveScale;
    }

    if(mKeyboard->isKeyDown(OIS::KC_S))
    {
            m_TranslateVector.z = m_MoveScale;
    }

    if(mKeyboard->isKeyDown(OIS::KC_LEFT))
    {
            mCamera->yaw(m_RotScale);
    }

    if(mKeyboard->isKeyDown(OIS::KC_RIGHT))
    {
            mCamera->yaw(-m_RotScale);
    }

    if(mKeyboard->isKeyDown(OIS::KC_UP))
    {
            mCamera->pitch(m_RotScale);
    }

    if(mKeyboard->isKeyDown(OIS::KC_DOWN))
    {
            mCamera->pitch(-m_RotScale);
    }*/
}

Ogre::MovableObject* Basic::queryClosestObject(Ogre::Ray* ray)
{
    Ogre::RaySceneQuery* raySceneQuery = Basic::getSingletonPtr()->mSceneMgr->createRayQuery(*ray);

    raySceneQuery->setSortByDistance(true);
    raySceneQuery->execute();

    Ogre::MovableObject *closestObject = NULL;
    //Ogre::Real closestDistance = 100000;


    //Ogre::RaySceneQueryResult::iterator rayIterator = result.begin();

    // at this point we have raycast to a series of different objects bounding boxes.
    // we need to test these different objects to see which is the first polygon hit.
    // there are some minor optimizations (distance based) that mean we wont have to
    // check all of the objects most of the time, but the worst case scenario is that
    // we need to test every triangle of every object.
    Ogre::Real closest_distance = -1.0f;
    Ogre::Vector3 closest_result;
    Ogre::RaySceneQueryResult &query_result = raySceneQuery->getLastResults();

    for (size_t qr_idx = 0; qr_idx < query_result.size(); qr_idx++)
    {
        // stop checking if we have found a raycast hit that is closer
        // than all remaining entities
        if ((closest_distance >= 0.0f) &&
                (closest_distance < query_result[qr_idx].distance))
        {
            break;
        }

        // only check this result if its a hit against an entity
        if ((query_result[qr_idx].movable != NULL) &&
                (query_result[qr_idx].movable->getMovableType().compare("Entity") == 0))
        {
            // get the entity to check
            Ogre::Entity *pentity = static_cast<Ogre::Entity*> (query_result[qr_idx].movable);

            // mesh data to retrieve
            size_t vertex_count;
            size_t index_count;
            Ogre::Vector3 *vertices;
            unsigned long *indices;

            // get the mesh information
            GetMeshInformation(pentity->getMesh(), vertex_count, vertices, index_count, indices,
                               pentity->getParentNode()->getWorldPosition(),
                               pentity->getParentNode()->getWorldOrientation(),
                               pentity->getParentNode()->_getDerivedScale());

            // test for hitting individual triangles on the mesh
            bool new_closest_found = false;
            for (int i = 0; i < static_cast<int> (index_count); i += 3)
            {
                // check for a hit against this triangle
                std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(*ray, vertices[indices[i]],
                                                                         vertices[indices[i + 1]], vertices[indices[i + 2]], true, false);

                // if it was a hit check if its the closest

                if (hit.first)
                {
                    if ((closest_distance < 0.0f) ||
                            (hit.second < closest_distance))
                    {
                        // this is the closest so far, save it off
                        closest_distance = hit.second;
                        new_closest_found = true;
                    }
                }
            }

            // free the verticies and indicies memory
            delete[] vertices;
            delete[] indices;

            // if we found a new closest raycast for this object, update the
            // closest_result before moving on to the next object.
            if (new_closest_found)
            {
                closest_result = ray->getPoint(closest_distance);
                closestObject = query_result[qr_idx].movable;
            }
        }
    }

    raySceneQuery->clearResults();

    return closestObject;
}

// Get the mesh information for the given mesh.
// Code found in Wiki: www.ogre3d.org/wiki/index.php/RetrieveVertexData

void Basic::GetMeshInformation(const Ogre::MeshPtr mesh,
                               size_t &vertex_count,
                               Ogre::Vector3* &vertices,
                               size_t &index_count,
                               unsigned long* &indices,
                               const Ogre::Vector3 &position,
                               const Ogre::Quaternion &orient,
                               const Ogre::Vector3 &scale)
{
    bool added_shared = false;
    size_t current_offset = 0;
    size_t shared_offset = 0;
    size_t next_offset = 0;
    size_t index_offset = 0;

    vertex_count = index_count = 0;

    // Calculate how many vertices and indices we're going to need
    for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);

        // We only need to add the shared vertices once
        if (submesh->useSharedVertices)
        {
            if (!added_shared)
            {
                vertex_count += mesh->sharedVertexData->vertexCount;
                added_shared = true;
            }
        }
        else
        {
            vertex_count += submesh->vertexData->vertexCount;
        }

        // Add the indices
        index_count += submesh->indexData->indexCount;
    }

    // Allocate space for the vertices and indices
    vertices = new Ogre::Vector3[vertex_count];
    indices = new unsigned long[index_count];

    added_shared = false;

    // Run through the submeshes again, adding the data into the arrays
    for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);

        Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

        if ((!submesh->useSharedVertices) || (submesh->useSharedVertices && !added_shared))
        {
            if (submesh->useSharedVertices)
            {
                added_shared = true;
                shared_offset = current_offset;
            }

            const Ogre::VertexElement* posElem =
                    vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

            Ogre::HardwareVertexBufferSharedPtr vbuf =
                    vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

            unsigned char* vertex =
                    static_cast<unsigned char*> (vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

            // There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
            //  as second argument. So make it float, to avoid trouble when Ogre::Real will
            //  be comiled/typedefed as double:
            //      Ogre::Real* pReal;
            float* pReal;

            for (size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
            {
                posElem->baseVertexPointerToElement(vertex, &pReal);

                Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);

                vertices[current_offset + j] = (orient * (pt * scale)) + position;
            }

            vbuf->unlock();
            next_offset += vertex_data->vertexCount;
        }


        Ogre::IndexData* index_data = submesh->indexData;
        size_t numTris = index_data->indexCount / 3;
        Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

        bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

        unsigned long* pLong = static_cast<unsigned long*> (ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        unsigned short* pShort = reinterpret_cast<unsigned short*> (pLong);


        size_t offset = (submesh->useSharedVertices) ? shared_offset : current_offset;

        if (use32bitindexes)
        {
            for (size_t k = 0; k < numTris * 3; ++k)
            {
                indices[index_offset++] = pLong[k] + static_cast<unsigned long> (offset);
            }
        }
        else
        {
            for (size_t k = 0; k < numTris * 3; ++k)
            {
                indices[index_offset++] = static_cast<unsigned long> (pShort[k]) +
                        static_cast<unsigned long> (offset);
            }
        }

        ibuf->unlock();
        current_offset = next_offset;
    }
}
