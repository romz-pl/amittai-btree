
set( LIB_NAME amittai-btree )

add_library( ${LIB_NAME} STATIC
    BPlusTree.cpp 
    InternalNode.cpp 
    KeyType.cpp
    io.cpp
    LeafNode.cpp 
    Node.cpp 
    Printer.cpp 
    Record.cpp 
#    main.cpp
)

target_include_directories( ${LIB_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src )

target_compile_options( ${LIB_NAME} PRIVATE ${ROMZ_CXX_FLAGS} )


