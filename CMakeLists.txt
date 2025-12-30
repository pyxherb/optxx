cmake_minimum_required(VERSION 3.13)
project(optxx VERSION 0.1.0)

add_subdirectory("optxx")

# Generate the version file for the config file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/optxxConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMinorVersion
)

# Create config file
configure_package_config_file(
    cmake/optxxConfig.cmake.in optxxConfig.cmake
    INSTALL_DESTINATION lib/cmake/optxx
)

# Install config files
install(
    FILES   
       ${CMAKE_CURRENT_BINARY_DIR}/optxxConfig.cmake
       ${CMAKE_CURRENT_BINARY_DIR}/optxConfigVersion.cmake
    DESTINATION
        lib/cmake/${PROJECT_NAME}
)

# Exporting Targets from the Build Tree
install(EXPORT optxxTargets
    FILE optxxTargets.cmake
    DESTINATION lib/cmake/${PROJECT_NAME}
)
