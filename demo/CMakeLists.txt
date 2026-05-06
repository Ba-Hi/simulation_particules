add_executable(demo_systeme_solaire  demo_systeme_solaire.cpp)


# Collision 1D
add_executable(demo_collision_1D demo_collision_1D.cpp)


# Collision 2D
add_executable(demo_collision_2D demo_collision_2D.cpp)
add_executable(demo_collision_2D_simple demo_collision_2D_simple.cpp)
add_executable(demo_collision_3_particules demo_collision_3_particules.cpp)

add_executable(demo_collision_3D demo_collision_3D.cpp)

add_executable(demo_comparaison_murs demo_comparaison_murs.cpp)
add_executable(demo_goutte_collision demo_goutte_collision.cpp)


foreach(d demo_systeme_solaire demo_collision_1D demo_collision_2D demo_collision_2D_simple demo_collision_3_particules demo_collision_3D demo_comparaison_murs demo_goutte_collision)
    target_link_libraries(${d} particules)
endforeach()