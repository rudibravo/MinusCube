/* 
 * File:   Piece.h
 * Author: rudi
 *
 * Created on 19 de Novembro de 2009, 11:13
 */

#ifndef _PIECE_H
#define	_PIECE_H


#include "Basic.h"
#include "Side.h"

enum Move {
    UP = 0, DOWN = 1, RIGHT = 2, LEFT = 3, FRONT = 4, BACK = 5
};

class Game;

class Piece {
public:

    int m_X;
    int m_Y;
    int m_Z;

    float m_animX;
    float m_animY;
    float m_animZ;

    int m_originalX;
    int m_originalY;
    int m_originalZ;

    float m_faceSize;

    Ogre::SceneNode* m_pCubeNode;
    //	Ogre::Entity*				m_pCubeEntity;
    Ogre::SceneNode* m_pParentNode;
    Game* m_game;

    /*
    0 = UP
    1 = DOWN
    2 = RIGHT
    3 = LEFT
    4 = FRONT
    5 = BACK
     */
    std::vector<Side*> m_sides;

    Piece(Ogre::SceneNode* parent, int x, int y, int z, Game* g,
            Color cUp, Color cDown, Color cFront, Color cBack, Color cRight, Color cLeft);
    ~Piece();

    Ogre::SceneNode* getNode();

    void move(Move m);
    void step(float timeSinceLastFrame);
    bool hasAnimation();
};

#endif	/* _PIECE_H */

