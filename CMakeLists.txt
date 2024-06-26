cmake_minimum_required(VERSION 3.16)
project(
  nomadbsd-adduser-gui
  HOMEPAGE_URL https://github.com/nomadbsd/nomadbsd-adduser-gui
  LANGUAGES CXX)
set(CMAKE_INSTALL_PREFIX "/usr")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROGRAM ${CMAKE_PROJECT_NAME})
set(BACKEND "nomadbsd-adduser")

set(BACKEND_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/libexec)
set(PATH_BACKEND ${BACKEND_INSTALL_DIR}/${BACKEND})

set(SOURCE_FILES
    src/backend.h
    src/main.cpp
    src/password.cpp
    src/password.h
    src/wizard.cpp
    src/wizard.h)

find_package(Qt6 REQUIRED COMPONENTS Widgets LinguistTools)
qt_standard_project_setup()

set(TS_FILES locale/${PROGRAM}_de.ts
             locale/${PROGRAM}_fr.ts
             locale/${PROGRAM}_ru.ts
             locale/${PROGRAM}_id.ts
             locale/${PROGRAM}_es.ts)

add_executable(nomadbsd-adduser-gui ${SOURCE_FILES})

qt_add_translations(nomadbsd-adduser-gui TS_FILES ${TS_FILES})

target_include_directories(
  nomadbsd-adduser-gui
  PUBLIC "${CMAKE_CURRENT_BINARY_DIR}"
         "${CMAKE_CURRENT_SOURCE_DIR}"
         "${CMAKE_CURRENT_SOURCE_DIR}/src")

configure_file(defs.h.in defs.h @ONLY)

set(LIBS Qt6::Widgets)
target_link_libraries(nomadbsd-adduser-gui PRIVATE ${LIBS})

install(TARGETS nomadbsd-adduser-gui DESTINATION bin)
install(PROGRAMS ${BACKEND} DESTINATION libexec)
install(FILES "${CMAKE_PROJECT_NAME}.desktop" DESTINATION share/applications)
