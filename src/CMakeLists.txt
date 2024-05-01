set(SOURCES
        main.cpp
        console/Console.cpp
        storage/Storage.cpp
        database/Database.cpp
        database/Query.cpp
)

set(HEADERS include
        include/database/Query.h)

add_executable(LittleDB ${SOURCES} ${HEADERS})

target_include_directories(LittleDB PUBLIC include)

