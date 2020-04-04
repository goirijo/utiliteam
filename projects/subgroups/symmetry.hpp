#ifndef SYMMETRY_HH
#define SYMMETRY_HH

#include "../eigen-git-mirror/Eigen/Dense"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <stdexcept>
#include <string>
#include <vector>

class SymOp
{
public:
    Eigen::Matrix3d cart_matrix;

    SymOp(Eigen::Matrix3d input_matrix);
};

SymOp operator*(const SymOp& lhs, const SymOp& rhs);


struct SymOpCompare_f
{
    SymOpCompare_f(SymOp input1, double tol);
    bool operator()(const SymOp& element2) const;

private:
    const SymOp element1;
    double tol;
};

//QUESTION!! Do we need to write a default constructor??
template <typename SymOpType, typename SymOpCompareType_f>
class SymGroup
{
public:
 
    SymGroup(std::vector<SymOpType> generating_elements);
    static void close_group(std::vector<SymOpType>* operations_ptr);
    bool insert(SymOpType& new_operation);
    const std::vector<SymOpType>& operations() const { return this->group; }

private:
    std::vector<SymOpType> group;
};

template <typename SymOpType,typename SymOpCompareType_f>
SymGroup<SymOpType, SymOpCompareType_f> operator*(SymGroup<SymOpType, SymOpCompareType_f> lhs, const SymGroup<SymOpType, SymOpCompareType_f>& rhs);

template <typename SymOpType,typename SymOpCompareType_f>
struct SymGroupCompare_f
{
    SymGroupCompare_f(SymGroup<SymOpType,SymOpCompareType_f> input1, double tol);

    bool operator()(const SymGroup<SymOpType,SymOpCompareType_f>& group2) const;

private:
    const SymGroup<SymOpType,SymOpCompareType_f> group1;
    double tol;
};

template <typename SymOpType, typename SymOpCompareType_f>
std::vector<SymGroup<SymOpType, SymOpCompareType_f>> find_subgroups(SymGroup<SymOpType, SymOpCompareType_f> input_group);

#endif
