cmake_minimum_required(VERSION 3.14)

project(deutschlernen LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Option to enable/disable automatic Qt installation
option(INSTALL_QT_IF_MISSING "Automatically download and install Qt6 if not found" OFF)

# Allow user to specify Qt path via -DQT_DIR or $QT_DIR environment variable
if(DEFINED ENV{QT_DIR})
    set(QT_DIR $ENV{QT_DIR} CACHE PATH "Path to Qt installation")
endif()

# Set default Qt installation path if none provided
if(NOT DEFINED QT_INSTALL_PATH)
    set(QT_INSTALL_PATH "${CMAKE_BINARY_DIR}/qt" CACHE PATH "Path where Qt will be installed if missing")
endif()

# Set CMAKE_PREFIX_PATH if QT_DIR is provided
if(DEFINED QT_DIR)
    if(EXISTS "${QT_DIR}")
        list(APPEND CMAKE_PREFIX_PATH "${QT_DIR}")
    else()
        message(WARNING "Specified QT_DIR '${QT_DIR}' does not exist. Falling back to system search.")
    endif()
endif()

# Try to find Qt6
find_package(Qt6 QUIET COMPONENTS Widgets Core)

# Fallback to Qt5 if Qt6 is not found
if(NOT Qt6_FOUND)
    message(WARNING "Qt6 not found, trying Qt5...")
    find_package(Qt5 QUIET COMPONENTS Widgets Core)
    if(Qt5_FOUND)
        message(STATUS "Using Qt5 found at ${Qt5_DIR}")
    endif()
endif()

# If neither Qt6 nor Qt5 is found, attempt to install Qt6 if enabled
if(NOT Qt6_FOUND AND NOT Qt5_FOUND AND INSTALL_QT_IF_MISSING)
    include(ExternalProject)

    # Determine platform-specific Qt download URL and installation details
    if(APPLE)
        set(QT_URL "https://download.qt.io/official_releases/qt/6.9/6.9.1/submodules/qtbase-mac-opensource-6.9.1.tar.xz")
        set(QT_CONFIGURE_ARGS -prefix ${QT_INSTALL_PATH} -opensource -confirm-license -nomake examples -nomake tests)
    elseif(WIN32)
        set(QT_URL "https://download.qt.io/official_releases/qt/6.9/6.9.1/submodules/qtbase-windows-opensource-6.9.1.tar.xz")
        set(QT_CONFIGURE_ARGS -prefix ${QT_INSTALL_PATH} -opensource -confirm-license -nomake examples -nomake tests -platform win32-msvc)
    else() # Linux
        # For Linux, prefer package manager instructions to avoid large downloads
        message(WARNING "Qt not found on Linux. Automatic installation via source is complex and slow.\n"
                        "Consider installing Qt6 using your package manager:\n"
                        "  - Ubuntu/Debian: sudo apt-get install qt6-base-dev\n"
                        "  - Fedora: sudo dnf install qt6-qtbase-devel\n"
                        "Alternatively, set INSTALL_QT_IF_MISSING=ON and specify QT_INSTALL_PATH to download Qt.")
        set(QT_URL "https://download.qt.io/official_releases/qt/6.9/6.9.1/submodules/qtbase-linux-opensource-6.9.1.tar.xz")
        set(QT_CONFIGURE_ARGS -prefix ${QT_INSTALL_PATH} -opensource -confirm-license -nomake examples -nomake tests)
    endif()

    # Download and build Qt using ExternalProject
    ExternalProject_Add(
        qt6
        URL ${QT_URL}
        PREFIX ${QT_INSTALL_PATH}
        CONFIGURE_COMMAND <SOURCE_DIR>/configure ${QT_CONFIGURE_ARGS}
        BUILD_COMMAND ninja
        INSTALL_COMMAND ninja install
        BUILD_IN_SOURCE 1
    )

    # Add Qt installation path to CMAKE_PREFIX_PATH
    list(APPEND CMAKE_PREFIX_PATH "${QT_INSTALL_PATH}")
    set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}" CACHE INTERNAL "")

    # Re-run find_package after installing Qt
    find_package(Qt6 REQUIRED COMPONENTS Widgets Core)
    if(Qt6_FOUND)
        message(STATUS "Qt6 installed and found at ${QT_INSTALL_PATH}")
    else()
        message(FATAL_ERROR "Failed to install and find Qt6. Please check the installation process or specify QT_DIR manually.")
    endif()
endif()

# If Qt is still not found, provide detailed error message
if(NOT Qt6_FOUND AND NOT Qt5_FOUND)
    message(FATAL_ERROR
        "Neither Qt6 nor Qt5 could be found.\n"
        "Please specify the Qt installation path using one of these methods:\n"
        "  1. Set the QT_DIR environment variable, e.g., 'export QT_DIR=/path/to/qt' (Linux/macOS) or 'set QT_DIR=C:\\path\\to\\qt' (Windows).\n"
        "  2. Pass -DQT_DIR=/path/to/qt when running CMake.\n"
        "  3. Enable automatic Qt installation with -DINSTALL_QT_IF_MISSING=ON (may download large files).\n"
        "  4. Install Qt via your package manager:\n"
        "     - macOS: brew install qt\n"
        "     - Ubuntu/Debian: sudo apt-get install qt6-base-dev\n"
        "     - Fedora: sudo dnf install qt6-qtbase-devel\n"
        "     - Or download from https://www.qt.io/download\n"
        "Current CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
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

file(COPY ${CMAKE_SOURCE_DIR}/map.png DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/controlla_risposte.txt DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/correggiverbi.txt DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/domande_output.txt DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/ollamaoutput.txt DESTINATION ${CMAKE_BINARY_DIR})

# Make Python script executable
file(CHMOD ${CMAKE_BINARY_DIR}/returnarticles.py
     PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                 GROUP_READ GROUP_EXECUTE
                 WORLD_READ WORLD_EXECUTE)
