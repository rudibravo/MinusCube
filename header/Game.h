/* 
 * File:   Game.h
 * Author: rudi
 *
 * Created on 19 de Novembro de 2009, 11:12
 */

#ifndef _GAME_H
#define	_GAME_H


#include "Basic.h"

#include "Piece.h"
#include "SideVerifier.h"

class Game : public OIS::KeyListener, OIS::MouseListener {
public:
    Game();
    ~Game();

    void startGame();
    void scramble();
    bool move(int piece);

    void createPieces();

    bool keyPressed(const OIS::KeyEvent &keyEventRef);
    bool keyReleased(const OIS::KeyEvent &keyEventRef);

    bool mouseMoved(const OIS::MouseEvent &evt);
    bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

    void finishAnimation();

    Ogre::MovableObject* getNode(int mouseScreenX, int mouseScreenY);

    bool isFinishedState();

private:
    void setupGameScene();
    void runGame();
    void moveNext();

    Ogre::SceneNode* m_pCubeNode;

    bool m_bShutdown;
    bool m_bMousePressed;
    bool m_hackPressed;
    bool m_isMoving;

    /*
      6-----7
     /|    /|
    2-----3 |
    | 4---|-5
    |/    |/
    0-----1

     */
    std::vector<Piece*> m_pieces;
    std::vector<SideVerifier*> m_sideVerifiers;
    int m_blankIdx;
    int m_lastBlankIdx;
    bool m_isScrambling;
    int m_scrambleCount;
    int m_maxScramble;
};

#endif	/* _GAME_H */

