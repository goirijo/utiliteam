#ifndef FASTSYMMETRY_HH
#define FASTSYMMETRY_HH

#include <memory>
#include <vector>
#include "./symop.hpp"
#include "./symgroup.hpp"


typedef std::vector<std::vector<int>> MultTable;

class AbstractSymOp
{
public:
    AbstractSymOp(int id, std::shared_ptr<MultTable> multiplication_table_ptr);
    int get_id() const;
    const std::shared_ptr<MultTable>& mult_table_ptr() const;

private:
    /// ID of this particular symmetry operation, represented as integer
    int id;

    /// Multiplication table that describes how each of the individual SymOps transform
    /// A*B=C
    /// multiplication[A][B]=C
    std::shared_ptr<MultTable> multiplication_table_ptr;
};

AbstractSymOp operator*(const AbstractSymOp& lhs, const AbstractSymOp& rhs);

struct AbstractSymOpCompare_f
{

    AbstractSymOpCompare_f(AbstractSymOp input1);
    bool operator()(const AbstractSymOp& element2) const;

private:
    int input1_id;
    const std::shared_ptr<MultTable> table_ptr;
};

std::vector<std::vector<int>> make_multiplication_table(const std::vector<SymOp>& group, double tol);


class BinaryAbstractComparator_f
{
public:
    bool operator()(const AbstractSymOp& lhs, const AbstractSymOp& rhs) const;
};

SymGroup<AbstractSymOp, BinaryAbstractComparator_f> transform_representation(const SymGroup<SymOp, CartesianBinaryComparator_f>& cartesian_group, double tol);
#endif
