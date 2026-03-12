# ncurses support
file(GLOB LOCAL_SOURCES "*.cpp")
add_executable(OptaFwUpdater ${LOCAL_SOURCES})

install(TARGETS OptaFwUpdater DESTINATION "${PROJECT_SOURCE_DIR}/bin")
