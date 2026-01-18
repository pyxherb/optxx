find_package(peff REQUIRED)

file(GLOB HEADERS *.h)
file(GLOB SRC *.cc)

add_library(optxx SHARED)
target_compile_definitions(optxx PUBLIC OPTXX_DYNAMIC_LINK=1)
target_compile_definitions(optxx PRIVATE IS_OPTXX_BASE_BUILDING=1)
set_target_properties(optxx PROPERTIES CXX_STANDARD 17)
target_sources(optxx PRIVATE ${HEADERS} ${SRC})
target_include_directories(optxx PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/> $<INSTALL_INTERFACE:include/>)
target_link_libraries(optxx PUBLIC peff_base peff_utils peff_containers)
set_target_properties(optxx PROPERTIES
    PUBLIC_HEADER "${HEADERS}"
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    ARCHIVE FALSE
)

add_library(optxx_static STATIC)
target_compile_definitions(optxx_static PRIVATE IS_OPTXX_BASE_BUILDING=1)
set_target_properties(optxx_static PROPERTIES CXX_STANDARD 17)
target_sources(optxx_static PRIVATE ${HEADERS} ${SRC})
target_link_libraries(optxx_static PUBLIC peff_base_static peff_utils_static peff_containers_static)
target_include_directories(optxx_static PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/> $<INSTALL_INTERFACE:include/>)
set_target_properties(optxx_static PROPERTIES
    PUBLIC_HEADER "${HEADERS}"
    VERSION ${PROJECT_VERSION}
)

install(TARGETS optxx optxx_static
    EXPORT optxxTargets
    LIBRARY DESTINATION lib/
    ARCHIVE DESTINATION lib/
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include/optxx/
)
