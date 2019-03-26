
set (SOURCE_GROUP_HEADER_FILES
${CMAKE_CURRENT_LIST_DIR}/RicDeletableItemCollection.h
${CMAKE_CURRENT_LIST_DIR}/RicDeleteItemsInCollectionFeature.h
${CMAKE_CURRENT_LIST_DIR}/RicDeleteAllItemsInCollectionFeature.h
)

set (SOURCE_GROUP_SOURCE_FILES
${CMAKE_CURRENT_LIST_DIR}/RicDeleteItemsInCollectionFeature.cpp
${CMAKE_CURRENT_LIST_DIR}/RicDeleteAllItemsInCollectionFeature.cpp
)

list(APPEND CODE_HEADER_FILES
${SOURCE_GROUP_HEADER_FILES}
)

list(APPEND CODE_SOURCE_FILES
${SOURCE_GROUP_SOURCE_FILES}
)

source_group( "CommandFeature\\DeletableItems" FILES ${SOURCE_GROUP_HEADER_FILES} ${SOURCE_GROUP_SOURCE_FILES} ${CMAKE_CURRENT_LIST_DIR}/CMakeLists_files.cmake )
