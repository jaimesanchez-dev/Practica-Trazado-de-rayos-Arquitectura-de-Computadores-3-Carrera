set(COMMON_SRC_FILES 
  "${CMAKE_SOURCE_DIR}/common/src/vector.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/rayo.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/esfera.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/cilindro.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/configuracion.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/lector_archivo_escena.cpp"
  "${CMAKE_SOURCE_DIR}/common/src/escena.cpp"
)

set(CURRENT_DIR_SRC_FILES 
  "${CMAKE_CURRENT_SOURCE_DIR}/test_vector.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_rayo.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_esfera.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_cilindro.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_configuracion.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_lector_archivo_escena.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/test_escena.cpp"
)

add_unit_test_target(
  TARGET_NAME utcommon
  SOURCE_FILES ${COMMON_SRC_FILES} ${CURRENT_DIR_SRC_FILES}
  LIBRARY_FILTER common
  COVERAGE_DIR coverage-common
  LIBRARY_TO_LINK common
)
