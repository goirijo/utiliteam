#include "./fastsymmetry.hpp"

AbstractSymOp::AbstractSymOp(int id, std::shared_ptr<MultTable> multiplication_table_ptr) : id(id), multiplication_table_ptr(multiplication_table_ptr) {}
int AbstractSymOp::get_id() { return this->id; }
const std::shared_ptr<MultTable> AbstractSymOp::mult_table_ptr() const { return this->multiplication_table_ptr; }


AbstractSymOp AbstractSymOp::operator*(const AbstractSymOp& rhs)
{
    if(this->multiplication_table_ptr!=rhs.mult_table_ptr())
    {
        //TODO: Bad things
        throw std::runtime_error("Your multiplication tables are different!!");
    }

    int lhs_id=this->id;
    int rhs_id=rhs.get_id();

    int product_id=(*multiplication_table_ptr)[lhs_id][rhs_id];
    return AbstractSymOp(product_id,multiplication_table_ptr);
}


AbstractSymOpCompare_f::AbstractSymOpCompare_f(AbstractSymOp input1) : input1_id(input1.get_id()), table_ptr(input1.mult_table_ptr()){};
bool AbstractSymOpCompare_f::operator()(const AbstractSymOp& element2) const
{
    return (input1_id == element2.get_id() && table_ptr == element2.mult_table_ptr());
}


//TODO:
std::vector<std::vector<int>> make_multiplication_table(const SymGroup& group) 
{
    MultTable multiplication_table;
    for (operation1 : group.operations()) 
    {
        std::vector<int> temp_index_vector;
        for (operation2 : group.operations()) 
        {
            SymOp temp_product = operation1 * operation2;
            SymOpCompare_f compare(temp_product);
            int temp_product_id = std::find_if(group.operations().begin(), group.operations().end(), compare);
            temp_index_vector.push_back(temp_product_id);
        }
        multiplication_table.push_back(temp_index_vector);
    }
    return multiplication_table;
}  

//TODO:
std::vector<AbstractSymOp> transform_representation(const SymGroup& cartesian_group) 
{
    MultTable multiplication_table = make_multiplication_table(cartesian_group);
    std::vector<AbstractSymOp> pot_abstract_group;
    std::shared_ptr<MultTable> table_ptr = std::make_shared<MultTable>(multiplication_table);
    for (int i = 0; i < cartesian_group.operations().size(); i++) 
    {
        AbstractSymOp abstract_symop(i, table_ptr);
        pot_abstract_group.push_back(abstract_symop);
    }
    
    return pot_abstract_group;

}
