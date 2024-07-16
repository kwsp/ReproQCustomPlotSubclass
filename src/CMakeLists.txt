set(EXE_NAME main)

find_package(Qt6 CONFIG REQUIRED COMPONENTS Widgets Gui PrintSupport)
qt_standard_project_setup()
set(CMAKE_AUTOMOC ON)


# Manually include QCustomPlot
find_path(QCUSTOMPLOT_INCLUDE_DIR qcustomplot.h PATH_SUFFIXES qcustomplot)
if (WIN32)
    find_library(QCUSTOMPLOT_LIBRARY NAMES qcustomplot2)
    find_library(QCUSTOMPLOT_LIBRARY_DEBUG NAMES qcustomplotd2)

    set(QCUSTOMPLOT_LIBRARY "$<IF:$<CONFIG:Debug>,${QCUSTOMPLOT_LIBRARY_DEBUG},${QCUSTOMPLOT_LIBRARY}>")
else ()
    find_library(QCUSTOMPLOT_LIBRARY NAMES qcustomplot)
endif()

if (QCUSTOMPLOT_INCLUDE_DIR AND QCUSTOMPLOT_LIBRARY)
    message(STATUS "Found QCustomPlot: ${QCUSTOMPLOT_LIBRARY}")
else()
    message(FATAL_ERROR "Could not find QCustomPlot")
endif()

find_package(fmt CONFIG REQUIRED)


qt_add_executable(${EXE_NAME}
  main.cpp
  CustomPlot.hpp
  CustomPlot.cpp
)

target_include_directories(${EXE_NAME} PRIVATE 
    src
    ${QCUSTOMPLOT_INCLUDE_DIR}
)

set_target_properties(${EXE_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_EXTENSIONS OFF
)

target_link_libraries(${EXE_NAME} PRIVATE
    fmt::fmt
    Qt::Widgets
    Qt::Gui
    Qt::PrintSupport
    ${QCUSTOMPLOT_LIBRARY}
)

if (WIN32)
    add_compile_definitions(_USE_MATH_DEFINES)

    # On Windows, call windeployqt create a deployable folder
    # https://doc.qt.io/qt-6/windows-deployment.html
    set_target_properties(${EXE_NAME} PROPERTIES
        WIN32_EXECUTABLE ON
    )

    # Determine the path to windeployqt
    set(WINDEPLOYQT_PATH "${Qt6_DIR}/../../tools/Qt6/bin")
    set(WINDEPLOYQT_EXE "${WINDEPLOYQT_PATH}/windeployqt.exe")
    set(WINDEPLOYQT_DEBUG_EXE "${WINDEPLOYQT_PATH}/windeployqt.debug.bat")

    # Add post build command to call windeployqt
    add_custom_command(TARGET ${EXE_NAME} POST_BUILD
        COMMAND "$<IF:$<CONFIG:Debug>,${WINDEPLOYQT_DEBUG_EXE},${WINDEPLOYQT_EXE}>"
        --verbose 1
        "$<TARGET_FILE:${EXE_NAME}>"
    )

elseif (APPLE)


    # Post-build steps for macOS
    # https://doc.qt.io/qt-6/macos-deployment.html

    set_target_properties(${EXE_NAME} PROPERTIES
        MACOSX_BUNDLE ON
        MACOSX_BUNDLE_INFO_PLIST cmake/MacOSXBundleInfo.plist.in
    )

    # Find macdeployqt
    get_target_property(QT_BIN_DIR Qt::qmake IMPORTED_LOCATION)
    find_program(MACDEPLOYQT NAMES macdeployqt HINTS "${QT_BIN_DIR}")

    # Use macdeployqt to bundle Qt libraries
    add_custom_command(TARGET ${EXE_NAME} POST_BUILD
        COMMAND ${MACDEPLOYQT}
        "$<TARGET_BUNDLE_DIR:${EXE_NAME}>"
        -verbose=1
        -always-overwrite
        -codesign=-
        -no-strip
        "$<IF:$<CONFIG:Release>,-dmg,-use-debug-libs>"
        COMMENT "Bundling Qt libraries"
    )

endif()
