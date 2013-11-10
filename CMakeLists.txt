cmake_minimum_required(VERSION 2.8.11)

project(QAqBanking)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" CACHE PATH "cmake modul path")
include(BuildUtil)

# The version number.
set (QIABanking_VERSION_MAJOR 0)
set (QIABanking_VERSION_MINOR 1)
set (QIABanking_VERSION_PATCH 0)
set (QIABanking_VERSION ${QIABanking_VERSION_MAJOR}.${QIABanking_VERSION_MINOR}.${QIABanking_VERSION_PATCH})

# http://www.cmake.org/Wiki/CMake/Tutorials/How_to_create_a_ProjectConfig.cmake_file
# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")
if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR CMake)
else()
  set(DEF_INSTALL_CMAKE_DIR lib/CMake/QIABanking)
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH "Installation directory for CMake files")

# Make relative paths absolute (needed later on)
foreach(p LIB INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

SET(QT5 "false" CACHE BOOL "Qt5 build")
if(${QT5})
    # Instruct CMake to run moc automatically when needed.
    set(CMAKE_AUTOMOC ON)

    # cmake example from http://qt-project.org/doc/qt-5.1/qtdoc/cmake-manual.html
    find_package(Qt5Core)

else(${QT5})
    find_package(Qt4 4.8 REQUIRED QtCore)
    include(${QT_USE_FILE})

endif(${QT5})

find_package(PkgConfig REQUIRED)
pkg_check_modules(AQB REQUIRED aqbanking)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(${AQB_INCLUDE_DIRS})

if(${DOC})
    find_doxygen()
endif(${DOC})

add_subdirectory(src)

if(${TEST})
    # The following are required to uses Dart and the Cdash dashboard
    enable_testing(true)
    include(CTest)
    add_subdirectory(test)
endif(${TEST})

# Add all targets to the build-tree export set
export(TARGETS qiabanking
  FILE "${PROJECT_BINARY_DIR}/QIABankingTargets.cmake")

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE QIABanking)

# Create the QIABankingConfig.cmake and QIABankingConfigVersion.cmake files
# ... for the build tree
set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}")
configure_file(QIABankingConfig.cmake.in
  "${PROJECT_BINARY_DIR}/QIABankingConfig.cmake" @ONLY)

# ... for the install tree
file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"
   "${INSTALL_INCLUDE_DIR}")
set(CONF_INCLUDE_DIRS "\${QPJSIP_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file(QIABankingConfig.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/QIABankingConfig.cmake" @ONLY)

# ... for both
configure_file(QIABankingConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/QIABankingConfigVersion.cmake" @ONLY)
 
# Install the QIABankingConfig.cmake and QIABankingConfigVersion.cmake
install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/QIABankingConfig.cmake"
  "${PROJECT_BINARY_DIR}/QIABankingConfigVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)
 
# Install the export set for use with the install-tree
install(EXPORT QIABankingTargets DESTINATION
  "${INSTALL_CMAKE_DIR}" COMPONENT dev)
