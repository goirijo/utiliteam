g++ --std=c++17 lattice.cpp tests.cpp coordinate_test.cpp coordinate.cpp -o coordinate_test
g++ --std=c++17 lattice.cpp tests.cpp lattice_test.cpp -o lattice_test
g++ --std=c++17 symop.cpp lattice.cpp coordinate.cpp tests.cpp site_test.cpp site.cpp -o site_test
g++ --std=c++17 symop.cpp lattice.cpp coordinate.cpp site.cpp tests.cpp structure_test.cpp structure.cpp -o structure_test
#g++ --std=c++17 site_test.cpp site.cxx -o site_test
g++ --std=c++17 tests.cpp symop.cpp symop_test.cpp -o test_symop
g++ --std=c++17 symop.cpp tests.cpp point_group.cxx test_pointgroup.cpp -o test_pointgroup
g++ --std=c++17 lattice.cpp site.cpp coordinate.cpp structure.cpp symop.cpp tests.cpp point_group.cxx fastsymmetry.cpp test_fastsym.cpp -o test_fastsymmetry
