include(CMakeFindDependencyMacro)
find_dependency(Boost)
find_dependency(clapack)
find_dependency(msgpack)
find_dependency(ubitrack_boost_bindings)
find_dependency(ubitrack_log4cpp)
find_dependency(ubitrack_tinyxml)
include("${CMAKE_CURRENT_LIST_DIR}/utcoreTargets.cmake")