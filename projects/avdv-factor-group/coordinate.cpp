#include "coordinate.hpp"
// Coordinate defintions
Coordinate::Coordinate(const Eigen::Vector3d& coord) : m_coord(coord) {}
Eigen::Vector3d Coordinate::get_cart_coordinate() const { return this->m_coord; }
double Coordinate::get_x() const { return this->m_coord(0); }
double Coordinate::get_y() const { return this->m_coord(1); }
double Coordinate::get_z() const { return this->m_coord(2); }
Eigen::Vector3d Coordinate::get_frac_coordinate(const Lattice& lattice) const{   
   Eigen::Vector3d frac_coord= lattice.col_vector_matrix().inverse()*m_coord;
   return frac_coord;
} 

void Coordinate::bring_within(const Lattice& lattice, double prec)
{
    Eigen::Vector3d frac_coords;
    frac_coords = this->get_frac_coordinate(lattice);
    //frac_coords = lattice.col_vector_matrix().inverse() * get_cart_coordinate(lattice);
    for (int i = 0; i < 3; ++i)
    {
        if (frac_coords(i) < -prec || frac_coords(i) >= 1-prec)		
        {
            frac_coords(i) = frac_coords(i) - floor(frac_coords(i)+prec);
        }
  
  //      if (frac_coords(i) < 0 || frac_coords(i) >= 1)		
  //      {
  //          frac_coords(i) = frac_coords(i) - floor(frac_coords(i));
  //      }
    }
    this->m_coord = lattice.col_vector_matrix() * frac_coords;
}
