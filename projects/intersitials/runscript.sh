#g++ interstitials.cpp xtal_classes.cpp symmetry_operations.cpp tests.cpp -o run
g++ --std=c++17 ../avdv-factor-group/lattice.cpp ../avdv-factor-group/site.cpp ../avdv-factor-group/coordinate.cpp ../avdv-factor-group/symop.cpp ../avdv-factor-group/structure.cpp ../avdv-factor-group/tests.cpp interstitial_mesh.cpp interstitial_mesh_test.cpp -o interstitial_test
exit 0
