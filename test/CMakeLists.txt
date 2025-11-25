add_executable(QuadtreeTest 
    QuadtreeTest.cpp 
)

set_target_properties(QuadtreeTest PROPERTIES 
    OSX_DEPLOYMENT_TARGET "15.0"
)

target_include_directories(QuadtreeTest PRIVATE 
    ${CMAKE_SOURCE_DIR}/lib/include
)

target_link_libraries(QuadtreeTest PRIVATE 
    QuadtreeTest 
    GTest::gtest_main
)
