cmake_minimum_required(VERSION 3.8)
project(sb_server)

set(CMAKE_C_STANDARD 99)

include_directories(../sb_tools/hea/)
include_directories(./hea/)
include_directories(./hea/connection/)
include_directories(./hea/data/)
include_directories(./hea/http/)
include_directories(./hea/server/)
include_directories(./hea/server/client/)
include_directories(./hea/server/worker/)

link_directories(../sb_tools/cmake-build-debug)
link_libraries(libsbtools)
link_libraries(pthread)

aux_source_directory(./ SOURCE_FILES)
aux_source_directory(./src/ SOURCE_FILES)
aux_source_directory(./src/connection/ SOURCE_FILES)
aux_source_directory(./src/data/ SOURCE_FILES)
aux_source_directory(./src/http/ SOURCE_FILES)
aux_source_directory(./src/server/ SOURCE_FILES)
aux_source_directory(./src/server/client/ SOURCE_FILES)
aux_source_directory(./src/server/worker/ SOURCE_FILES)



add_executable(sb_server ${SOURCE_FILES})