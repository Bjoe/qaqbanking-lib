cmake_minimum_required(VERSION 2.8.11)

project(QAqBanking)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" CACHE PATH "cmake modul path")
include(BuildUtil)

# The version number.
set (QAqBanking_VERSION_MAJOR 0)
set (QAqBanking_VERSION_MINOR 2)
set (QAqBanking_VERSION_PATCH 0)
set (QAqBanking_VERSION ${QAqBanking_VERSION_MAJOR}.${QAqBanking_VERSION_MINOR}.${QAqBanking_VERSION_PATCH})

add_definitions(-std=c++11)

# http://www.cmake.org/Wiki/CMake/Tutorials/How_to_create_a_ProjectConfig.cmake_file
# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")
if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR CMake)
else()
  set(DEF_INSTALL_CMAKE_DIR lib/CMake/QAqBanking)
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH "Installation directory for CMake files")

# Make relative paths absolute (needed later on)
foreach(p LIB INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# cmake example from http://qt-project.org/doc/qt-5.1/qtdoc/cmake-manual.html
find_package(Qt5Core)

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
export(TARGETS qaqbanking
  FILE "${PROJECT_BINARY_DIR}/QAqBankingTargets.cmake")

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE QAqBanking)



##################################################################
# Create the xxxxxxConfig.cmake and xxxxxxConfigVersion.cmake files

#-----------------------------------------------------------------
# ... for the build tree
set(CONF_INCLUDE_DIRS
  "${PROJECT_SOURCE_DIR}/src"
  "${PROJECT_BINARY_DIR}"
)
configure_file(
  "QAqBankingConfig.cmake.in"
  "${PROJECT_BINARY_DIR}/QAqBankingConfig.cmake" @ONLY
)

#-----------------------------------------------------------------
# ... for the install tree
file(RELATIVE_PATH 
  REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"
  "${INSTALL_INCLUDE_DIR}"
)  
set(CONF_INCLUDE_DIRS 
  "\${QAQBANKING_CMAKE_DIR}/${REL_INCLUDE_DIR}"
)
configure_file(
  "QAqBankingConfig.cmake.in"
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/QAqBankingConfig.cmake" @ONLY
)

#-----------------------------------------------------------------
# ... for both
configure_file(
  "QAqBankingConfigVersion.cmake.in"
  "${PROJECT_BINARY_DIR}/QAqBankingConfigVersion.cmake" @ONLY
)


##################################################################
# Install the xxxxxxConfig.cmake and xxxxxxConfigVersion.cmake
install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/QAqBankingConfig.cmake"
  "${PROJECT_BINARY_DIR}/QAqBankingConfigVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)
 
# Install the export set for use with the install-tree
install(EXPORT QAqBankingTargets DESTINATION
  "${INSTALL_CMAKE_DIR}" COMPONENT dev)
