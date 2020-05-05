#include "structure.hpp"
// Structure Definitions
Structure::Structure(const Lattice& lattice, const std::vector<Site>& sites) : m_lattice(lattice), m_sites(sites) {}
Lattice Structure::get_lattice() const { return this->m_lattice; }
std::vector<Site> Structure::get_sites() const { return this->m_sites; }

