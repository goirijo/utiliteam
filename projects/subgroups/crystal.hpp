#ifndef CRYSTAL_HH
#define CRYSTAL_HH

#include "../eigen-git-mirror/Eigen/Core"
#include "../eigen-git-mirror/Eigen/src/Core/Matrix.h"
#include <iostream>
#include <vector>

class CrystalStructure
{
    public:
        Eigen::Matrix3d lattice;
        std::vector<Eigen::Vector3d> basis;

        CrystalStructure(Eigen::Matrix3d input_matrix, std::vector<Eigen::Vector3d> input_basis)
        { 
            Eigen::Matrix3d lattice = input_matrix;
            std::vector<Eigen::Vector3d> basis = input_basis;
        }
};

#endif
