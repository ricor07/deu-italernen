cmake_minimum_required(VERSION 3.14)

project(deutschlernen LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "/Users/riccardoorsi/Qt/6.9.1/macos")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core)

add_executable(deutschlernen
    main.cpp
    flashcards/flashcards.cpp
    flashcards/flashcards.h
    flashcards/gioca.cpp
    flashcards/gioca.h
    flashcards/settings.cpp
    flashcards/settings.h
    flashcards/account.cpp
    flashcards/account.h
    flashcards/search.cpp
    flashcards/search.h
    flashcards/statistiche.cpp
    flashcards/statistiche.h
    flashcards/scheda.cpp
    flashcards/scheda.h
    giornale/mappa.cpp
    giornale/mappa.h
    giornale/giornale.cpp
    giornale/giornale.h
    giornale/array.cpp
    giornale/array.h
    giornale/listagiornali.cpp
    giornale/listagiornali.h
    giornale/impostazioni.cpp
    giornale/impostazioni.h
    giornale/invio.cpp
    giornale/invio.h
    giornale/articoli.cpp
    giornale/articoli.h
    verbi/verbiinterface.cpp
    verbi/verbiinterface.h
    verbi/correction.cpp
    verbi/correction.h
    )   

target_link_libraries(deutschlernen PRIVATE Qt6::Widgets Qt6::Core)

add_custom_command(TARGET deutschlernen POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/csvfiles $<TARGET_FILE_DIR:deutschlernen>/csvfiles)

# Copy Python script to build directory
configure_file(
    ${CMAKE_SOURCE_DIR}/returnarticles.py
    ${CMAKE_BINARY_DIR}/returnarticles.py
    COPYONLY
)

# Make script executable
file(CHMOD ${CMAKE_BINARY_DIR}/returnarticles.py
     PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                 GROUP_READ GROUP_EXECUTE
                 WORLD_READ WORLD_EXECUTE)