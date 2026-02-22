set(SOURCES
    src/main.cpp
    src/server.cpp
    src/server.h
    src/mediacontroller.cpp
    src/mediacontroller.h
    src/inputcontroller.cpp
    src/inputcontroller.h
    src/filetransfer.cpp
    src/filetransfer.h
    src/systemcontroller.cpp
    src/systemcontroller.h
    src/screenshare.cpp
    src/screenshare.h
)

add_executable(pc-remote-server ${SOURCES})

target_link_libraries(pc-remote-server
    Qt6::Core
    Qt6::Network
    Qt6::WebSockets
    Qt6::Widgets
    Qt6::Multimedia
)

if(WIN32)
    target_link_libraries(pc-remote-server user32)
endif()

install(TARGETS pc-remote-server
    RUNTIME DESTINATION bin
)
