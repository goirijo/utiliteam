g++ --std=c++17 lattice.cpp tests.cpp coordinate_test.cpp coordinate.cpp -o coordinate_test
g++ --std=c++17 lattice.cpp tests.cpp lattice_test.cpp -o lattice_test
g++ --std=c++17 symop.cpp lattice.cpp coordinate.cpp tests.cpp site_test.cpp site.cpp -o site_test
g++ --std=c++17 symop.cpp lattice.cpp coordinate.cpp site.cpp tests.cpp structure_test.cpp structure.cpp -o structure_test
