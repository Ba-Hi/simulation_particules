
add_executable(test_vector       test_vector.cpp)
add_executable(test_particule    test_particule.cpp)
add_executable(test_univers      test_univers.cpp)
add_executable(test_cellule      test_cellule.cpp)
add_executable(test_univers_tp4  test_univers_tp4.cpp)
add_executable(test_performance  test_performance_tp2.cpp)

foreach(t test_vector test_particule test_univers test_cellule test_univers_tp4)
    target_link_libraries(${t} particules gtest_main)
endforeach()

target_link_libraries(test_performance particules)

include(GoogleTest)
gtest_discover_tests(test_vector)
gtest_discover_tests(test_particule)
gtest_discover_tests(test_univers)
gtest_discover_tests(test_cellule)
gtest_discover_tests(test_univers_tp4)