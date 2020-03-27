#include <memory>
#include <vector>

class SymGroup;
class SymOp;

class AbstractSymOp
{
public:
    typedef std::vector<std::vector<int>> MultTable;
    AbstractSymOp(int id, std::shared_ptr<MultTable> multiplication_table_ptr) : id(id), multiplication_table_ptr(multiplication_table_ptr)
    {
    }

AbstractSymOp operator*(const AbstractSymOp& rhs)
{
    if(this->multiplication_table_ptr!=rhs.multiplication_table_ptr)
    {
        //TODO: Bad things
    }

    int lhs_id=this->id;
    int rhs_id=rhs.id;

    int product_id=(*multiplication_table_ptr)[lhs_id][rhs_id];
    return AbstractSymOp(product_id,multiplication_table_ptr);
}

private:
    /// ID of this particular symmetry operation, represented as integer
    int id;

    /// Multiplication table that describes how each of the individual SymOps transform
    /// A*B=C
    /// multiplication[A][B]=C
    std::shared_ptr<MultTable> multiplication_table_ptr;
};

struct AbstractSymOpCompare_f
{
    AbstractSymOpCompare_f(AbstractSymOp input1);
    bool operator()(const AbstractSymOp& element2) const
    {
        /* return (input1_id==element2.id() && table_ptr==...); */
    }

private:

    int input1_id;
    const AbstractSymOp::MultTable* table_ptr;
};

//TODO:
std::vector<std::vector<int>> make_multiplication_table(const std::vector<SymOp>& group);   //or take SymGroup

//TODO:
std::vector<AbstractSymOp> trasform_representation(const std::vector<SymOp>& cartesian_operations);
