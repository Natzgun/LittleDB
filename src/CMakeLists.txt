set(SOURCES
        main.cpp
        console/Console.cpp
        storage/Storage.cpp
)

set(HEADERS include)

add_executable(LittleDB ${SOURCES} ${HEADERS})

target_include_directories(LittleDB PUBLIC include)

