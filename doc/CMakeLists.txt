# add a target to generate API documentation with Doxygen

if(DOXYGEN_FOUND)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/pages.txt.in ${CMAKE_CURRENT_BINARY_DIR}/pages.txt @ONLY)
  add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM
  )
else(DOXYGEN_FOUND)
  message(STATUS "NOTE: Doxygen not found. API docs will not be generated. SDK target will not be available")
endif(DOXYGEN_FOUND)
