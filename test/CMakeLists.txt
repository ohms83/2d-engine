include(CMakeLists.txt)
include(Classes/game/CMakeLists.txt)

list(APPEND GAME_SOURCE
    ${ENGINE_HEADER}
    ${GAME_HEADER}
)

list(APPEND GAME_HEADER
    ${ENGINE_SOURCE}
    ${GAME_SOURCE}
)