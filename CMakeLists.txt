cmake_minimum_required(VERSION 3.14)

project(deutschlernen LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Allow user to specify Qt path via -DQT_DIR or $QT_DIR environment variable
if(DEFINED ENV{QT_DIR})
    set(QT_DIR $ENV{QT_DIR} CACHE PATH "Path to Qt installation")
endif()

# Set CMAKE_PREFIX_PATH if QT_DIR is provided
if(DEFINED QT_DIR)
    if(EXISTS "${QT_DIR}")
        list(APPEND CMAKE_PREFIX_PATH "${QT_DIR}")
    else()
        message(WARNING "Specified QT_DIR '${QT_DIR}' does not exist. Falling back to system search.")
    endif()
endif()

# Minimal set of common Qt paths as fallback (only if no QT_DIR is set)
if(NOT DEFINED QT_DIR)
    if(APPLE)
        list(APPEND CMAKE_PREFIX_PATH
            "$ENV{HOME}/Qt"
            "/opt/homebrew/opt/qt"
            "/usr/local/opt/qt"
        )
    elseif(WIN32)
        list(APPEND CMAKE_PREFIX_PATH
            "C:/Qt"
            "C:/vcpkg/installed/x64-windows"
        )
    else() # Linux/Unix
        list(APPEND CMAKE_PREFIX_PATH
            "$ENV{HOME}/Qt"
            "/usr/lib/x86_64-linux-gnu/cmake/Qt6"
            "/usr/lib/cmake/Qt6"
            "/opt/Qt"
        )
    endif()
    # Remove duplicates from CMAKE_PREFIX_PATH
    if(CMAKE_PREFIX_PATH)
        list(REMOVE_DUPLICATES CMAKE_PREFIX_PATH)
    endif()
endif()

# Try to find Qt6
find_package(Qt6 QUIET COMPONENTS Widgets Core)

# Fallback to Qt5 if Qt6 is not found
if(NOT Qt6_FOUND)
    message(WARNING "Qt6 not found, trying Qt5...")
    find_package(Qt5 REQUIRED COMPONENTS Widgets Core)
    if(Qt5_FOUND)
        message(STATUS "Using Qt5 found at ${Qt5_DIR}")
    else()
        message(FATAL_ERROR
            "Neither Qt6 nor Qt5 could be found.\n"
            "Please specify the Qt installation path using one of these methods:\n"
            "  1. Set the QT_DIR environment variable, e.g., 'export QT_DIR=/path/to/qt' (Linux/macOS) or 'set QT_DIR=C:\\path\\to\\qt' (Windows).\n"
            "  2. Pass -DQT_DIR=/path/to/qt when running CMake.\n"
            "  3. Install Qt via your package manager (e.g., 'brew install qt' on macOS, 'sudo apt-get install qt6-base-dev' on Ubuntu, or download from https://www.qt.io/download).\n"
            "Current CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
    endif()
endif()

# Print Qt version for confirmation
if(Qt6_FOUND)
    message(STATUS "Using Qt6 version ${Qt6Core_VERSION}")
else()
    message(STATUS "Using Qt5 version ${Qt5Core_VERSION}")
endif()

# Define the executable with all source files
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

# Link against Qt libraries
if(Qt6_FOUND)
    target_link_libraries(deutschlernen PRIVATE Qt6::Widgets Qt6::Core)
else()
    target_link_libraries(deutschlernen PRIVATE Qt5::Widgets Qt5::Core)
endif()

# Copy CSV files after build
add_custom_command(TARGET deutschlernen POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/csvfiles
            $<TARGET_FILE_DIR:deutschlernen>/csvfiles)

# Copy Python script
configure_file(
    ${CMAKE_SOURCE_DIR}/returnarticles.py
    ${CMAKE_BINARY_DIR}/returnarticles.py
    COPYONLY
)

# Make Python script executable
file(CHMOD ${CMAKE_BINARY_DIR}/returnarticles.py
     PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                 GROUP_READ GROUP_EXECUTE
                 WORLD_READ WORLD_EXECUTE)
