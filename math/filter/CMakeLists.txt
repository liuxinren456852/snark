SET( PROJECT "filter" )
SET( TARGET_NAME snark_${PROJECT} )

FILE( GLOB source ${SOURCE_CODE_BASE_DIR}/math/${PROJECT}/*.cpp)
FILE( GLOB includes ${SOURCE_CODE_BASE_DIR}/math/${PROJECT}/*.h)

SOURCE_GROUP( ${TARGET_NAME} FILES ${source} ${includes} ${detailIncludes} )

INSTALL( FILES ${includes} DESTINATION ${snark_INSTALL_INCLUDE_DIR}/math/${PROJECT} )

IF( BUILD_TESTS )
    ADD_SUBDIRECTORY( test )
ENDIF( BUILD_TESTS )

