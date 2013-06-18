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
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Alarm.o \
	${OBJECTDIR}/ClientSocket.o \
	${OBJECTDIR}/Configurator.o \
	${OBJECTDIR}/CountryLookup.o \
	${OBJECTDIR}/DiskCache.o \
	${OBJECTDIR}/Display.o \
	${OBJECTDIR}/HttpHandler.o \
	${OBJECTDIR}/HttpRequest.o \
	${OBJECTDIR}/HttpResponse.o \
	${OBJECTDIR}/IC_MCP3008.o \
	${OBJECTDIR}/IClientHandler.o \
	${OBJECTDIR}/PageFunctions.o \
	${OBJECTDIR}/Pages.o \
	${OBJECTDIR}/Relay.o \
	${OBJECTDIR}/RelayBoard.o \
	${OBJECTDIR}/RelayConditionFunctions.o \
	${OBJECTDIR}/Sensors.o \
	${OBJECTDIR}/ServiceController.o \
	${OBJECTDIR}/Templates.o \
	${OBJECTDIR}/Time.o \
	${OBJECTDIR}/Utils.o \
	${OBJECTDIR}/WebHttp.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-std=gnu++0x -lpthread -L /usr/lib/includes -lpqxx
CXXFLAGS=-std=gnu++0x -lpthread -L /usr/lib/includes -lpqxx

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/code

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/code: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/code ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/Alarm.o: Alarm.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -std=gnu++0x -lpthread -L /usr/lib/includes -lpqxx -MMD -MP -MF $@.d -o ${OBJECTDIR}/Alarm.o Alarm.cpp

${OBJECTDIR}/ClientSocket.o: ClientSocket.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -std=gnu++0x -lpthread -L /usr/lib/includes -lpqxx -MMD -MP -MF $@.d -o ${OBJECTDIR}/ClientSocket.o ClientSocket.cpp

${OBJECTDIR}/Configurator.o: Configurator.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -std=gnu++0x -lpthread -L /usr/lib/includes -lpqxx -MMD -MP -MF $@.d -o ${OBJECTDIR}/Configurator.o Configurator.cpp

${OBJECTDIR}/CountryLookup.o: CountryLookup.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -std=gnu++0x -lpthread -L /usr/lib/includes -lpqxx -MMD -MP -MF $@.d -o ${OBJECTDIR}/CountryLookup.o CountryLookup.cpp

${OBJECTDIR}/DiskCache.o: DiskCache.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -std=gnu++0x -lpthread -L /usr/lib/includes -lpqxx -MMD -MP -MF $@.d -o ${OBJECTDIR}/DiskCache.o DiskCache.cpp

${OBJECTDIR}/Display.o: Display.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -std=gnu++0x -lpthread -L /usr/lib/includes -lpqxx -MMD -MP -MF $@.d -o ${OBJECTDIR}/Display.o Display.cpp

${OBJECTDIR}/HttpHandler.o: HttpHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -std=gnu++0x -lpthread -L /usr/lib/includes -lpqxx -MMD -MP -MF $@.d -o ${OBJECTDIR}/HttpHandler.o HttpHandler.cpp

${OBJECTDIR}/HttpRequest.o: HttpRequest.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -std=gnu++0x -lpthread -L /usr/lib/includes -lpqxx -MMD -MP -MF $@.d -o ${OBJECTDIR}/HttpRequest.o HttpRequest.cpp

${OBJECTDIR}/HttpResponse.o: HttpResponse.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -std=gnu++0x -lpthread -L /usr/lib/includes -lpqxx -MMD -MP -MF $@.d -o ${OBJECTDIR}/HttpResponse.o HttpResponse.cpp

${OBJECTDIR}/IC_MCP3008.o: IC_MCP3008.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -std=gnu++0x -lpthread -L /usr/lib/includes -lpqxx -MMD -MP -MF $@.d -o ${OBJECTDIR}/IC_MCP3008.o IC_MCP3008.cpp

${OBJECTDIR}/IClientHandler.o: IClientHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -std=gnu++0x -lpthread -L /usr/lib/includes -lpqxx -MMD -MP -MF $@.d -o ${OBJECTDIR}/IClientHandler.o IClientHandler.cpp

${OBJECTDIR}/PageFunctions.o: PageFunctions.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -std=gnu++0x -lpthread -L /usr/lib/includes -lpqxx -MMD -MP -MF $@.d -o ${OBJECTDIR}/PageFunctions.o PageFunctions.cpp

${OBJECTDIR}/Pages.o: Pages.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -std=gnu++0x -lpthread -L /usr/lib/includes -lpqxx -MMD -MP -MF $@.d -o ${OBJECTDIR}/Pages.o Pages.cpp

${OBJECTDIR}/Relay.o: Relay.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -std=gnu++0x -lpthread -L /usr/lib/includes -lpqxx -MMD -MP -MF $@.d -o ${OBJECTDIR}/Relay.o Relay.cpp

${OBJECTDIR}/RelayBoard.o: RelayBoard.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -std=gnu++0x -lpthread -L /usr/lib/includes -lpqxx -MMD -MP -MF $@.d -o ${OBJECTDIR}/RelayBoard.o RelayBoard.cpp

${OBJECTDIR}/RelayConditionFunctions.o: RelayConditionFunctions.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -std=gnu++0x -lpthread -L /usr/lib/includes -lpqxx -MMD -MP -MF $@.d -o ${OBJECTDIR}/RelayConditionFunctions.o RelayConditionFunctions.cpp

${OBJECTDIR}/Sensors.o: Sensors.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -std=gnu++0x -lpthread -L /usr/lib/includes -lpqxx -MMD -MP -MF $@.d -o ${OBJECTDIR}/Sensors.o Sensors.cpp

${OBJECTDIR}/ServiceController.o: ServiceController.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -std=gnu++0x -lpthread -L /usr/lib/includes -lpqxx -MMD -MP -MF $@.d -o ${OBJECTDIR}/ServiceController.o ServiceController.cpp

${OBJECTDIR}/Templates.o: Templates.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -std=gnu++0x -lpthread -L /usr/lib/includes -lpqxx -MMD -MP -MF $@.d -o ${OBJECTDIR}/Templates.o Templates.cpp

${OBJECTDIR}/Time.o: Time.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -std=gnu++0x -lpthread -L /usr/lib/includes -lpqxx -MMD -MP -MF $@.d -o ${OBJECTDIR}/Time.o Time.cpp

${OBJECTDIR}/Utils.o: Utils.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -std=gnu++0x -lpthread -L /usr/lib/includes -lpqxx -MMD -MP -MF $@.d -o ${OBJECTDIR}/Utils.o Utils.cpp

${OBJECTDIR}/WebHttp.o: WebHttp.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -std=gnu++0x -lpthread -L /usr/lib/includes -lpqxx -MMD -MP -MF $@.d -o ${OBJECTDIR}/WebHttp.o WebHttp.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -DLINUX -std=gnu++0x -lpthread -L /usr/lib/includes -lpqxx -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/code

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
