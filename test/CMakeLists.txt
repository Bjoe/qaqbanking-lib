
if(CMAKE_CROSSCOMPILING)
    include(Coverage)
endif(CMAKE_CROSSCOMPILING)

pkg_check_modules(GWEN REQUIRED gwenhywfar)
find_package(QtTestUtil)

if(${QT5})
    # Instruct CMake to run moc automatically when needed.
    set(CMAKE_AUTOMOC OFF)

    # cmake example from http://qt-project.org/doc/qt-5.1/qtdoc/cmake-manual.html
    find_package(Qt5Test)

else(${QT5})
    find_package(Qt4 4.8 REQUIRED QtTest)
    include(${QT_USE_FILE})

endif(${QT5})

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
    ${QT_TEST_UTIL_INCLUDE_DIRS}
    ${GWEN_INCLUDE_DIRS}
)

set(test_additional_lib
    qiabanking
    ${AQB_LIBRARIES}
    ${GWEN_LIBRARIES}
    ${QT_TEST_UTIL_LIBRARIES}
)

qt_test(accountinfo accountinfotest.cpp)
qt_test(transaction transactiontest.cpp)
qt_test(imexportercontext imexportercontexttest.cpp)
qt_test(exporter exportertest.cpp)
qt_test(swiftimporter swiftimportertest.cpp)
qt_test(swifttransaction swifttransactiontest.cpp)
qt_test(mainswift mainswifttest.cpp)
