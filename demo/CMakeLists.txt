add_executable(demo_systeme_solaire  demo_systeme_solaire.cpp)
add_executable(demo_collision        demo_collision.cpp)
add_executable(demo_collision_simple demo_collision_simple.cpp)
add_executable(demo_collision_8000 demo_collision_8000.cpp)

foreach(d demo_systeme_solaire demo_collision demo_collision_simple demo_collision_8000)
    target_link_libraries(${d} particules)
endforeach()