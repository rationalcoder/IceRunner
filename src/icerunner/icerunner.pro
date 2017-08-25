TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../middleware/SDL2-2.0.4/include
INCLUDEPATH += ../middleware/SDL2_ttf-2.0.14-VC/include
INCLUDEPATH += ../middleware/SDL2_image-2.0.1-VC/include
INCLUDEPATH += ../middleware/SDL2_mixer-2.0.1-VC/include
INCLUDEPATH += ../middleware/lua53DLL/include
INCLUDEPATH += ../middleware/glm

LIBS += -L../middleware/SDL2-2.0.4/lib/x64 -lSDL2main -lSDL2
LIBS += -L../middleware/SDL2_ttf-2.0.14-VC/lib/x64 -lSDL2_ttf
LIBS += -L../middleware/SDL2_image-2.0.1-VC/lib/x64 -lSDL2_image
LIBS += -L../middleware/SDL2_mixer-2.0.1-VC/lib/x64 -lSDL2_mixer
LIBS += -L../middleware/lua53DLL -llua53

LIBS += -lXInput -ld3dcompiler -ld3d11 -ldxgi

SOURCES += main.cpp \
    rendering/dx11constantbuffer.cpp \
    rendering/dx11geometry.cpp \
    rendering/dx11renderer.cpp \
    rendering/dx11shader.cpp \
    rendering/dx11texture.cpp \
    rendering/displaymanager.cpp \
    events/eventmanager.cpp \
    game/gamemanager.cpp \
    game/deckloader.cpp \
    rendering/dx11resourceloader.cpp \
    game/maptestingstate.cpp \
    game/menu.cpp \
    game/resourcemanager.cpp \
    game/maptestingresourcemanager.cpp \
    physics/scene.cpp \
    events/hidmanager.cpp \
    game/background.cpp \
    game/luainterface.cpp \
    game/maprenderer.cpp \
    sound/soundmanager.cpp \
    sound/resourceloader.cpp \
    sound/effect.cpp \
    sound/music.cpp \
    game/dj.cpp \
    game/mainmenuresourcemanager.cpp \
    game/mainmenustate.cpp \
    game/racingstate.cpp \
    game/racingresourcemanager.cpp \
    game/deckchoosingsubstate.cpp \
    game/racingsubstate.cpp \
    game/resultssubstate.cpp


HEADERS += \
    core/engine.hpp \
    rendering/displaymanager.h \
    sound/soundmanager.h \
    physics/physicsmanager.h \
    rendering/dx11renderer.h \
    rendering/dx11structs.h \
    rendering/comptrhack.hpp \
    events/eventmanager.h \
    core/constants.hpp \
    game/gamemanager.h \
    game/game.h \
    game/gamestate.hpp \
    game/types.hpp \
    game/gamestates.hpp \
    events/hidmanager.h \
    game/map.hpp \
    game/maputility.hpp \
    game/deckloader.h \
    game/mapkit.hpp \
    game/mapfactory.hpp \
    game/deckettings.hpp \
    events/eventstructs.hpp \
    game/gamerenderstructs.hpp \
    rendering/dx11fwd.hpp \
    game/constants.hpp \
    game/resourcemanager.h \
    game/maptools.hpp \
    rendering/fwd.hpp \
    core/utility.hpp \
    physics/physicsstructs.hpp \
    game/luainterface.h \
    game/maprenderer.h \
    sound/soundstructs.hpp \
    sound/soundfwd.hpp \
    game/dj.h \
    game/racingsubstates.hpp \
    events/eventfwd.hpp
