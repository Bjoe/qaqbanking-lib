
set(MOC_HDRS
    swift/transaction.h
)

set(HEADERS
  accountinfobuilder.h
  accountinfo.h
  imexportercontext.h
  dtaus/exporter.h
  dtaus/exportbuilder.h
  dtaus/transactionbuilder.h
  dtaus/transaction.h
  swift/importer.h
  swift/transaction.h
)

set(QT_SRCS
    accountinfo.cpp
    imexportercontext.cpp
    dtaus/transaction.cpp
    dtaus/exporter.cpp
    swift/importer.cpp
    swift/transaction.cpp
)

qt4_wrap_cpp(MOC_OUTFILES ${MOC_HDRS})

include(${QT_USE_FILE})

add_library(qiabanking STATIC ${QT_SRCS} ${MOC_OUTFILES})

set_target_properties(qiabanking PROPERTIES
  PUBLIC_HEADER "${HEADERS}")
 
install(TARGETS qiabanking
  # IMPORTANT: Add the qiabanking library to the "export-set"
  EXPORT QIABankingTargets
  ARCHIVE DESTINATION "${INSTALL_LIB_DIR}" COMPONENT dev
  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}" COMPONENT dev)
