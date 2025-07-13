cmake_minimum_required(VERSION 3.14)

project(deutschlernen LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Function to search for Qt in common locations
function(find_qt_paths)
    set(QT_POSSIBLE_PATHS "")

    if(APPLE)
        # macOS common Qt paths
        list(APPEND QT_POSSIBLE_PATHS
            "$ENV{HOME}/Qt/6.9.1/macos"
            "$ENV{HOME}/Qt/6.8.0/macos"
            "/opt/homebrew/opt/qt"
            "/opt/homebrew/opt/qt6"
            "/usr/local/opt/qt"
            "/Applications/Qt/6.9.1/macos"
        )
    elseif(WIN32)
        # Windows common Qt paths
        list(APPEND QT_POSSIBLE_PATHS
            "C:/Qt/6.9.1/msvc2019_64"
            "C:/Qt/6.8.0/msvc2019_64"
            "C:/Qt/6.7.2/msvc2019_64"
            "C:/vcpkg/installed/x64-windows"
            "C:/Qt/6.9.1/mingw_64"
        )
    else() # Linux/Unix
        # Linux common Qt paths
        list(APPEND QT_POSSIBLE_PATHS
            "$ENV{HOME}/Qt/6.9.1/gcc_64"
            "$ENV{HOME}/Qt/6.8.0/gcc_64"
            "/usr/lib/x86_64-linux-gnu/cmake/Qt6"
            "/usr/local/lib/cmake/Qt6"
            "/opt/Qt/6.9.1/gcc_64"
            "/usr/lib/cmake/Qt6"
        )
    endif()

    # Add user-specified Qt path from environment variable
    if(DEFINED ENV{QT_DIR})
        list(APPEND QT_POSSIBLE_PATHS "$ENV{QT_DIR}")
    endif()

    # Remove duplicates and non-existent paths
    list(REMOVE_DUPLICATES QT_POSSIBLE_PATHS)
    foreach(path ${QT_POSSIBLE_PATHS})
        if(EXISTS "${path}")
            list(APPEND CMAKE_PREFIX_PATH "${path}")
        endif()
    endforeach()

    # Make CMAKE_PREFIX_PATH unique and available to parent scope
    if(CMAKE_PREFIX_PATH)
        list(REMOVE_DUPLICATES CMAKE_PREFIX_PATH)
        set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}" PARENT_SCOPE)
    endif()
endfunction()

# Call the function to populate CMAKE_PREFIX_PATH
find_qt_paths()

# Try to find Qt6, fall back to Qt5 if Qt6 is not found
find_package(Qt6 QUIET COMPONENTS Widgets Core)
if(NOT Qt6_FOUND)
    message(WARNING "Qt6 not found, trying Qt5...")
    find_package(Qt5 REQUIRED COMPONENTS Widgets Core)
    if(Qt5_FOUND)
        message(STATUS "Using Qt5 found at ${Qt5_DIR}")
    else()
        message(FATAL_ERROR "Neither Qt6 nor Qt5 could be found. Please specify Qt path via -DQT_DIR or ensure Qt is installed.")
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
