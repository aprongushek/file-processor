cmake_minimum_required(VERSION 3.19)
project(file_processor LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)

qt_standard_project_setup()

qt_add_executable(file_processor
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    fileprocessor.h fileprocessor.cpp


)

target_link_libraries(file_processor
    PRIVATE
        Qt::Core
        Qt::Widgets
)
target_link_libraries(file_processor PRIVATE Qt6::Core)
target_link_libraries(file_processor PRIVATE Qt6::Core)
target_link_libraries(file_processor PRIVATE Qt6::Core)

include(GNUInstallDirs)

install(TARGETS file_processor
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET file_processor
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
