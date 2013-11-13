
set(HEADERS
  dtaus/exporter.h
  dtaus/exportbuilder.h
  dtaus/transactionbuilder.h
  dtaus/transaction.h
  swift/importer.h
  swift/transaction.h
)

set(QT_SRCS
    dtaus/transaction.cpp
    dtaus/exporter.cpp
    swift/importer.cpp
    swift/transaction.cpp
)


add_library(qaqbanking STATIC ${QT_SRCS} ${MOC_OUTFILES})
target_link_libraries(qaqbanking Qt5::Core)

set_target_properties(qaqbanking PROPERTIES
  PUBLIC_HEADER "${HEADERS}")
 
install(TARGETS qaqbanking
  # IMPORTANT: Add the qaqbanking library to the "export-set"
  EXPORT QAqBankingTargets
  ARCHIVE DESTINATION "${INSTALL_LIB_DIR}" COMPONENT dev
  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}" COMPONENT dev)
