set(SOURCES
        main.cpp
        console/Console.cpp
        storage/Storage.cpp
        database/Database.cpp
        database/Query.cpp
        database/Validator.cpp
        storage/Disk.cpp
)

set(HEADERS include)

add_executable(LittleDB ${SOURCES} ${HEADERS})

target_include_directories(LittleDB PUBLIC include)

