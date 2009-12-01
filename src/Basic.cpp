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

using namespace Ogre;

template<> Basic* Ogre::Singleton<class Basic>::ms_Singleton = 0;

void Basic::windowClosed(Ogre::RenderWindow* rw)
{
    //std::cout << "BBBB" << std::endl;
    m_pMouse = 0;
    m_bShutDownOgre = true;
}

Basic::Basic()
{
    m_MoveSpeed = 0.1;
    m_RotateSpeed = 0.3;

    m_bShutDownOgre = false;
    m_iNumScreenShots = 0;

    m_pRoot = 0;
    m_pSceneMgr = 0;
    m_pRenderWnd = 0;
    m_pCamera = 0;
    m_pViewport = 0;
    m_pLog = 0;
    m_pTimer = 0;

    m_pInputMgr = 0;
    m_pKeyboard = 0;
    m_pMouse = 0;

    m_pDebugOverlay = 0;
    m_pInfoOverlay = 0;
}

void Basic::initOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener, OIS::MouseListener *pMouseListener)
{
    new Ogre::LogManager();

    m_pLog = Ogre::LogManager::getSingleton().createLog("OgreLogfile.log", true, true, false);
    //m_pLog->setDebugOutputEnabled(true);

    m_pRoot = new Ogre::Root();

    m_pRoot->showConfigDialog();
    m_pRenderWnd = m_pRoot->initialise(true, wndTitle);

    m_pSceneMgr = m_pRoot->createSceneManager(ST_GENERIC, "SceneManager");
    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    m_pCamera = m_pSceneMgr->createCamera("Camera");
    m_pCamera->setPosition(Vector3(0, 0, 15));
    m_pCamera->lookAt(Vector3(0, 0, 0));
    m_pCamera->setNearClipDistance(1);

    m_pViewport = m_pRenderWnd->addViewport(m_pCamera);
    m_pViewport->setBackgroundColour(ColourValue(0.8, 0.7, 0.6, 1.0));

    m_pCamera->setAspectRatio(Real(m_pViewport->getActualWidth()) / Real(m_pViewport->getActualHeight()));

    m_pViewport->setCamera(m_pCamera);

    unsigned long hWnd = 0;
    OIS::ParamList paramList;
    m_pRenderWnd->getCustomAttribute("WINDOW", &hWnd);

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

    m_pInputMgr = OIS::InputManager::createInputSystem(paramList);

    m_pKeyboard = static_cast<OIS::Keyboard*> (m_pInputMgr->createInputObject(OIS::OISKeyboard, true));
    m_pMouse = static_cast<OIS::Mouse*> (m_pInputMgr->createInputObject(OIS::OISMouse, true));

    m_pMouse->getMouseState().height = m_pRenderWnd->getHeight();
    m_pMouse->getMouseState().width = m_pRenderWnd->getWidth();

    Ogre::WindowEventUtilities::addWindowEventListener(m_pRenderWnd, this);

    if (pKeyListener == 0)
        m_pKeyboard->setEventCallback(this);
    else
        m_pKeyboard->setEventCallback(pKeyListener);

    if (pMouseListener == 0)
        m_pMouse->setEventCallback(this);
    else
        m_pMouse->setEventCallback(pMouseListener);

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

    m_pTimer = new Ogre::Timer();
    m_pTimer->reset();

    m_pDebugOverlay = OverlayManager::getSingleton().getByName("Core/DebugOverlay");
    //m_pDebugOverlay->show();

    m_pRenderWnd->setActive(true);
}

Basic::~Basic()
{
    delete m_pKeyboard;
    if (m_pMouse)
        delete m_pMouse;

    OIS::InputManager::destroyInputSystem(m_pInputMgr);

    delete m_pRoot;
}

bool Basic::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    m_pLog->logMessage("Basic::keyPressed");

    if (m_pKeyboard->isKeyDown(OIS::KC_ESCAPE))
    {
        m_bShutDownOgre = true;
        return true;
    }

    if (m_pKeyboard->isKeyDown(OIS::KC_SYSRQ))
    {
        std::ostringstream ss;
        ss << "screenshot_" << ++m_iNumScreenShots << ".png";
        m_pRenderWnd->writeContentsToFile(ss.str());
        return true;
    }

    if (m_pKeyboard->isKeyDown(OIS::KC_M))
    {
        static int mode = 0;

        if (mode == 2)
        {
            m_pCamera->setPolygonMode(PM_SOLID);
            mode = 0;
        }
        else if (mode == 0)
        {
            m_pCamera->setPolygonMode(PM_WIREFRAME);
            mode = 1;
        }
        else if (mode == 1)
        {
            m_pCamera->setPolygonMode(PM_POINTS);
            mode = 2;
        }
    }

    if (m_pKeyboard->isKeyDown(OIS::KC_O))
    {
        if (m_pDebugOverlay)
        {
            if (!m_pDebugOverlay->isVisible())
                m_pDebugOverlay->show();
            else
                m_pDebugOverlay->hide();
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
    //m_pCamera->yaw(Degree(evt.state.X.rel * -0.1));
    //m_pCamera->pitch(Degree(evt.state.Y.rel * -0.1));

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

    const RenderTarget::FrameStats& stats = m_pRenderWnd->getStatistics();
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
    if (m_pKeyboard->isKeyDown(OIS::KC_LSHIFT)) m_pCamera->moveRelative(m_TranslateVector);
    m_pCamera->moveRelative(m_TranslateVector / 10);
}

void Basic::getInput()
{
    /*if(m_pKeyboard->isKeyDown(OIS::KC_A))
    {
            m_TranslateVector.x = -m_MoveScale;
    }

    if(m_pKeyboard->isKeyDown(OIS::KC_D))
    {
            m_TranslateVector.x = m_MoveScale;
    }

    if(m_pKeyboard->isKeyDown(OIS::KC_W))
    {
            m_TranslateVector.z = -m_MoveScale;
    }

    if(m_pKeyboard->isKeyDown(OIS::KC_S))
    {
            m_TranslateVector.z = m_MoveScale;
    }

    if(m_pKeyboard->isKeyDown(OIS::KC_LEFT))
    {
            m_pCamera->yaw(m_RotScale);
    }

    if(m_pKeyboard->isKeyDown(OIS::KC_RIGHT))
    {
            m_pCamera->yaw(-m_RotScale);
    }

    if(m_pKeyboard->isKeyDown(OIS::KC_UP))
    {
            m_pCamera->pitch(m_RotScale);
    }

    if(m_pKeyboard->isKeyDown(OIS::KC_DOWN))
    {
            m_pCamera->pitch(-m_RotScale);
    }*/
}

Ogre::MovableObject* Basic::queryClosestObject(Ogre::Ray* ray)
{
    Ogre::RaySceneQuery* raySceneQuery = Basic::getSingletonPtr()->m_pSceneMgr->createRayQuery(*ray);

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
