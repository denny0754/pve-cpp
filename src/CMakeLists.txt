# Add source to this project's executable.
add_executable (
	PVECPP
	"Main.cpp"

	"api/internal/InternalUtility.cpp"

	"api/session/PVESession.cpp"

	"api/access/PVETicket.cpp"
	"api/access/PVEUser.cpp"
)

set_target_properties(PVECPP PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET PVECPP PROPERTY CXX_STANDARD 20)
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
   	add_compile_definitions(TRX_DEVELOPER_BUILD)
endif()

find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(CURL CONFIG REQUIRED)

target_link_libraries(
	PVECPP
	PRIVATE
	fmt::fmt
	spdlog::spdlog
	CURL::libcurl
)