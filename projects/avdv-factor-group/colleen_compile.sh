#//g++ --std=c++17 site_test.cpp site.cxx -o site_test
#g++ --std=c++17 lattice_test.cpp lattice.cxx -o lattice_test
g++ --std=c++17 symop.cpp tests.cpp point_group.cxx test_pointgroup.cpp -o test_pointgroup
g++ --std=c++17 lattice.cpp site.cpp coordinate.cpp structure.cpp symop.cpp tests.cpp point_group.cxx fastsymmetry.cpp test_fastsym.cpp -o test_fastsymmetry
