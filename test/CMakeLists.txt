
pkg_check_modules(GWEN REQUIRED gwenhywfar)

set(CMAKE_AUTOMOC OFF)

# cmake example from http://qt-project.org/doc/qt-5.1/qtdoc/cmake-manual.html
find_package(Qt5Test)

set(MT940TESTFILE "${PROJECT_SOURCE_DIR}/mt940.txt")

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/testconfig.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/testconfig.h"
  )

include_directories(
    ../src
    ${CMAKE_CURRENT_BINARY_DIR}/../src
    ${GWEN_INCLUDE_DIRS}
)

set(test_additional_lib
    qaqbanking
    ${AQB_LIBRARIES}
    ${GWEN_LIBRARIES}
)

qt_test(transaction transactiontest.cpp)
qt_test(exporter exportertest.cpp)
qt_test(swiftimporter swiftimportertest.cpp)
qt_test(mainswift mainswifttest.cpp)
