cmake_minimum_required(VERSION 3.1)

# TODO: SDK Versioning.
project(GGPO VERSION 1.0.0)

LINK_DIRECTORIES(
	${PROJECT_SOURCE_DIR}
	)


add_library(ggpo SHARED)

set(GGPO_LIB_INC_NOFILTER
	"lib/ggpo/bitvector.h"
	"lib/ggpo/game_input.h"
	"lib/ggpo/input_queue.h"
	"lib/ggpo/ggpolog.h"
	"lib/ggpo/poll.h"
	"lib/ggpo/ring_buffer.h"
	"lib/ggpo/sync.h"
	"lib/ggpo/timesync.h"
	"lib/ggpo/types.h"
)

set(GGPO_LIB_SRC_NOFILTER
	"lib/ggpo/bitvector.cpp"
	"lib/ggpo/game_input.cpp"
	"lib/ggpo/input_queue.cpp"
	"lib/ggpo/ggpolog.cpp"
	"lib/ggpo/ggpo.cpp"
	"lib/ggpo/poll.cpp"
	"lib/ggpo/sync.cpp"
	"lib/ggpo/timesync.cpp"
)

if(UNIX)
	set(GGPO_LIB_SRC_NOFILTER
		${GGPO_LIB_SRC_NOFILTER}
		"lib/ggpo/platform_linux.cpp"
	)
endif()

if(WIN32)
	set(GGPO_LIB_SRC_NOFILTER
		${GGPO_LIB_SRC_NOFILTER}
		"lib/ggpo/platform_windows.cpp"
	)
endif()

set(GGPO_LIB_INC_NETWORK
	"lib/ggpo/network/udp.h"
	"lib/ggpo/network/udp_msg.h"
	"lib/ggpo/network/udp_proto.h"
)

set(GGPO_LIB_SRC_NETWORK
	"lib/ggpo/network/udp.cpp"
	"lib/ggpo/network/udp_proto.cpp"
	"lib/ggpo/network/cs.cpp"
	"lib/ggpo/network/cs_proto.cpp"
)

set(GGPO_LIB_INC_BACKENDS
	"lib/ggpo/backends/backend.h"
	"lib/ggpo/backends/csbackend.h"
	"lib/ggpo/backends/p2p.h"
	"lib/ggpo/backends/spectator.h"
	"lib/ggpo/backends/synctest.h"
)

set(GGPO_LIB_SRC_BACKENDS
	"lib/ggpo/backends/csbackend.cpp"
	"lib/ggpo/backends/p2p.cpp"
	"lib/ggpo/backends/spectator.cpp"
	"lib/ggpo/backends/synctest.cpp"
)

set(GGPO_PUBLIC_INC
	"include/ggponet.h"
)

source_group(" " FILES ${GGPO_LIB_INC_NOFILTER} ${GGPO_LIB_SRC_NOFILTER})
source_group("Network" FILES ${GGPO_LIB_INC_NETWORK} ${GGPO_LIB_SRC_NETWORK})
source_group("Backends" FILES ${GGPO_LIB_INC_BACKENDS} ${GGPO_LIB_SRC_BACKENDS})
source_group("Public" FILES ${GGPO_PUBLIC_INC})

target_sources(ggpo PRIVATE
	${GGPO_LIB_INC_NOFILTER}
	${GGPO_LIB_SRC_NOFILTER}
	${GGPO_LIB_INC_NETWORK}
	${GGPO_LIB_SRC_NETWORK}
	${GGPO_LIB_INC_BACKENDS}
	${GGPO_LIB_SRC_BACKENDS}
	${GGPO_PUBLIC_INC}
)

if(WIN32)
	target_link_libraries(ggpo PUBLIC winmm.lib ws2_32.lib nicenet stdc++)
endif()

if(ANDROID)
	target_link_libraries(ggpo PUBLIC nicenet stdc++)
endif()

target_include_directories(ggpo PUBLIC
	"include"
	"nicenet"
	"./"
)
message(${PROJECT_BINARY_DIR})
message(${PROJECT_SOURCE_DIR})
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
install(TARGETS ggpo DESTINATION ${PROJECT_SOURCE_DIR})
install(DIRECTORY ${PROJECT_BINARY_DIR} DESTINATION ${PROJECT_SOURCE_DIR} FILES_MATCHING PATTERN "*.a" PATTERN "*.pdb")



