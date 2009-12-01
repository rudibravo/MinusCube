/* 
 * File:   main.cpp
 * Author: rudi
 *
 * Created on 19 de Novembro de 2009, 11:14
 */

#include "Game.h"


#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"



INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else

int main(int argc, char **argv)
#endif
{
    try
    {
        Game game;
        game.startGame();
        std::cout << "?" << std::endl;
    }
    catch (std::exception& e)
    {
#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBoxA(NULL, e.what(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        fprintf(stderr, "An exception has occurred\n");
#endif
    }

    return 0;
}

