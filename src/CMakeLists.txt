set(SOURCES
        main.cpp
        console/Console.cpp
        storage/Storage.cpp
        database/Database.cpp
        database/Query.cpp
        database/Validator.cpp
        database/DatabaseMediator.cpp
        storage/Disk.cpp
        storage/Disk_manager.cpp
        storage/HeapFile.cpp
        buffer/Buffer_Pool.cpp
        buffer/BufferManager.cpp
        buffer/Clock.cpp
        buffer/Frame.cpp
        buffer/Page.cpp
)

set(HEADERS include)

add_executable(LittleDB ${SOURCES} ${HEADERS})

target_include_directories(LittleDB PUBLIC include)

