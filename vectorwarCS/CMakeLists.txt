cmake_minimum_required(VERSION 3.1)
project(GGPOAPP)
include(CMakeSources.cmake)

LINK_DIRECTORIES(
	${PROJECT_SOURCE_DIR}/..
	)
	
add_executable(VectorWarCS WIN32 
	${GGPO_EXAMPLES_VECTORWAR_SRC}
)

if(WIN32 AND BUILD_SHARED_LIBS)
	add_definitions(-DGGPO_SHARED_LIB)
endif()

# Change the character set to unicode.
add_definitions(-D_UNICODE -DUNICODE)

INCLUDE_DIRECTORIES(
	${PROJECT_SOURCE_DIR}/../include
	${PROJECT_SOURCE_DIR}/..
)
message(${PROJECT_SOURCE_DIR})
# Link against GGPO, winmm (Windows Multimedia API), and ws2_32 (Winsock).
target_link_libraries(VectorWarCS ggpo winmm ws2_32 gdi32 nicenet )

install(TARGETS VectorWarCS DESTINATION ${PROJECT_SOURCE_DIR}/../)
