#include "./fastsymmetry.hpp"
#include "./symop.hpp"

AbstractSymOp::AbstractSymOp(int id, std::shared_ptr<MultTable> multiplication_table_ptr) : id(id), multiplication_table_ptr(multiplication_table_ptr) {}
int AbstractSymOp::get_id() const { return this->id; }
const std::shared_ptr<MultTable>& AbstractSymOp::mult_table_ptr() const { return this->multiplication_table_ptr; }


AbstractSymOp operator*(const AbstractSymOp& lhs, const AbstractSymOp& rhs)
{
    if(lhs.mult_table_ptr()!=rhs.mult_table_ptr())
    {
        throw std::runtime_error("Your multiplication tables are different!!");
    }

    int lhs_id=lhs.get_id();
    int rhs_id=rhs.get_id();

    int product_id=(*lhs.mult_table_ptr())[lhs_id][rhs_id];
    return AbstractSymOp(product_id,lhs.mult_table_ptr());
}


AbstractSymOpCompare_f::AbstractSymOpCompare_f(const AbstractSymOp input1) : input1_id(input1.get_id()), table_ptr(input1.mult_table_ptr()){};
bool AbstractSymOpCompare_f::operator()(const AbstractSymOp& element2) const
{
    return (input1_id == element2.get_id() && table_ptr == element2.mult_table_ptr());
}

std::vector<std::vector<int>> make_multiplication_table(const std::vector<SymOp>& group, double tol) 
{
    MultTable multiplication_table;
    for (const SymOp& operation1 : group) 
    {
        std::vector<int> temp_index_vector;
        for (const SymOp& operation2 : group) 
        {
            SymOp temp_product = operation1 * operation2;
            SymOpCompare_f compare(temp_product,tol);
            auto temp_product_it = std::find_if(group.begin(), group.end(), compare);
            int temp_product_id=std::distance(group.begin(), temp_product_it);
            temp_index_vector.push_back(temp_product_id);
        }
        multiplication_table.push_back(temp_index_vector);
    }
    return multiplication_table;
}  



bool BinaryAbstractComparator_f::operator()(const AbstractSymOp& lhs, const AbstractSymOp& rhs) const
{
      AbstractSymOpCompare_f compare(lhs);
      return compare(rhs);
}

//TODO: This will eventually change to return a SymGroup
SymGroup<AbstractSymOp, BinaryAbstractComparator_f> transform_representation(const SymGroup<SymOp, CartesianBinaryComparator_f>& cartesian_group, double tol) 
{
    MultTable multiplication_table = make_multiplication_table(cartesian_group.operations(), tol);
    BinaryAbstractComparator_f comp;
    SymGroup<AbstractSymOp, BinaryAbstractComparator_f> pot_abstract_group({}, comp);
    std::shared_ptr<MultTable> table_ptr = std::make_shared<MultTable>(multiplication_table);
    for (int i = 0; i < cartesian_group.operations().size(); i++) 
    {
        AbstractSymOp abstract_symop(i, table_ptr);
        pot_abstract_group.insert(abstract_symop);
    }
    
    return pot_abstract_group;
}
