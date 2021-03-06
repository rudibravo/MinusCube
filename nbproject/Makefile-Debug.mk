#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Debug
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/Game.o \
	${OBJECTDIR}/src/SideVerifier.o \
	${OBJECTDIR}/src/main.o \
	${OBJECTDIR}/src/Side.o \
	${OBJECTDIR}/src/Piece.o \
	${OBJECTDIR}/src/Basic.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/usr/lib -lOIS -lCEGUIOgreRenderer -lOgreMain -lCEGUIBase

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Debug.mk dist/Debug/GNU-Linux-x86/minuscube

dist/Debug/GNU-Linux-x86/minuscube: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/GNU-Linux-x86
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/minuscube ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/src/Game.o: nbproject/Makefile-${CND_CONF}.mk src/Game.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -DOGRE_GUI_GLX -DOGRE_CONFIG_LITTLE_ENDIAN -I/usr/include/OGRE -I/usr/include/OIS -I/usr/include -I./header -I/usr/include/CEGUI -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Game.o src/Game.cpp

${OBJECTDIR}/src/SideVerifier.o: nbproject/Makefile-${CND_CONF}.mk src/SideVerifier.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -DOGRE_GUI_GLX -DOGRE_CONFIG_LITTLE_ENDIAN -I/usr/include/OGRE -I/usr/include/OIS -I/usr/include -I./header -I/usr/include/CEGUI -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/SideVerifier.o src/SideVerifier.cpp

${OBJECTDIR}/src/main.o: nbproject/Makefile-${CND_CONF}.mk src/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -DOGRE_GUI_GLX -DOGRE_CONFIG_LITTLE_ENDIAN -I/usr/include/OGRE -I/usr/include/OIS -I/usr/include -I./header -I/usr/include/CEGUI -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/main.o src/main.cpp

${OBJECTDIR}/src/Side.o: nbproject/Makefile-${CND_CONF}.mk src/Side.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -DOGRE_GUI_GLX -DOGRE_CONFIG_LITTLE_ENDIAN -I/usr/include/OGRE -I/usr/include/OIS -I/usr/include -I./header -I/usr/include/CEGUI -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Side.o src/Side.cpp

${OBJECTDIR}/src/Piece.o: nbproject/Makefile-${CND_CONF}.mk src/Piece.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -DOGRE_GUI_GLX -DOGRE_CONFIG_LITTLE_ENDIAN -I/usr/include/OGRE -I/usr/include/OIS -I/usr/include -I./header -I/usr/include/CEGUI -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Piece.o src/Piece.cpp

${OBJECTDIR}/src/Basic.o: nbproject/Makefile-${CND_CONF}.mk src/Basic.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -DOGRE_GUI_GLX -DOGRE_CONFIG_LITTLE_ENDIAN -I/usr/include/OGRE -I/usr/include/OIS -I/usr/include -I./header -I/usr/include/CEGUI -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Basic.o src/Basic.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Debug
	${RM} dist/Debug/GNU-Linux-x86/minuscube

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
