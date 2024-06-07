include(CMakeFindDependencyMacro)
find_dependency(Protobuf)
find_dependency(gRPC)

include("${CMAKE_CURRENT_LIST_DIR}/equeueTargets.cmake")
