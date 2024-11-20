cmake_minimum_required(VERSION 3.29)
project(OPIDR_EXP)
set(CRYPTOPP_DIR "D:/cryptopp/")
set(GMP_DIR "D:/gmp-6.2.1")
include_directories(${PROJECT_SOURCE_DIR}/include ${CRYPTOPP_DIR} ${GMP_DIR}/include)

set(CMAKE_CXX_STANDARD 11)

set(SRC ${PROJECT_SOURCE_DIR}/src/Hash/murmurhash.cpp  ${PROJECT_SOURCE_DIR}/src/Hash/cuckoo_hash.cpp main.cpp ${PROJECT_SOURCE_DIR}/src/AES_utils.cpp
 ${PROJECT_SOURCE_DIR}/src/utils/utils.cpp ${PROJECT_SOURCE_DIR}/src/soprf.cpp opidr.cpp)
link_directories( ${GMP_DIR}/.libs ${CRYPTOPP_DIR} )


add_executable(main ${SRC})


target_link_libraries(main gmp cryptopp)

