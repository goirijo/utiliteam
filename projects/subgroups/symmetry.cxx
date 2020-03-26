#include "./symmetry.hpp"
#define PREC 1e-6

template <typename MatrixType>
bool is_unitary(const MatrixType& input_matrix, double tol)
{
    MatrixType product = input_matrix.transpose() * input_matrix;
    return product.isIdentity(tol);
}

SymOp::SymOp(Eigen::Matrix3d input_matrix) : cart_matrix(input_matrix)
{
    if (!is_unitary(input_matrix, PREC))
    {
        throw std::runtime_error("Your matrix isn't unitary.");
    }

    double det = input_matrix.determinant();
    if ((1 - abs(det)) > PREC)
    {
        throw std::runtime_error("Your matrix has a non-unit determinant.");
    }
}

SymOp operator*(const SymOp& lhs, const SymOp& rhs)
{
    Eigen::Matrix3d product = lhs.cart_matrix * rhs.cart_matrix;
    SymOp symop_product(product);
    return symop_product;
}

SymOpCompare_f::SymOpCompare_f(SymOp input1, double tol) : element1(input1), tol(tol) {
}

bool SymOpCompare_f::operator()(const SymOp& element2) const { 
    
        return element1.cart_matrix.isApprox(element2.cart_matrix, tol); }

void SymGroup::close_group(std::vector<SymOp>* operations_ptr)
{
    int push_limit = 200;
    int push_count = 0;

    auto& operations = *operations_ptr;
    // take first element, mulptiply with everything
    // if the product isn't there, then add it to group
    // continue to next element
    // stop when no new elements are added

    bool is_closed = false;
    while (!is_closed)
    {
        is_closed = true;
        int last_size = operations.size();
        for (int i = 0; i < last_size; ++i)
        {
            for (int j = 0; j < last_size; ++j)
            {
                SymOp candidate_operation = operations[i] * operations[j];
                SymOpCompare_f has_candidate(candidate_operation, PREC);
                if (std::find_if(operations.begin(), operations.end(), has_candidate) == operations.end())
                {
                    operations.push_back(candidate_operation);
                    is_closed = false;
                    ++push_count;
                }
            }
        }

        if (push_count > push_limit)
        {
            throw std::runtime_error("Your group couldn't be closed with " + std::to_string(push_limit) + " operations");
        }
    }

    return;
}

bool SymGroup::insert(SymOp& new_operation)
{
    SymOpCompare_f compare(new_operation, PREC);
    if (find_if(this->group.begin(), this->group.end(), compare) == this->group.end())
    {
        this->group.push_back(new_operation);
        close_group(&this->group);
        return true;
    }

    return false;
}

SymGroup::SymGroup(std::vector<SymOp> generating_elements)
{
    for (SymOp element : generating_elements)
    {
        this->insert(element);
    }
}

SymGroup operator*(SymGroup lhs, const SymGroup& rhs)
{
    // combines all operations of the two groups and closes the result
    // TODO
    // combines groups in total group.

    for (auto op : rhs.operations())
    {
        //        SymOpCompare_f compare(op, PREC);
        lhs.insert(op);
    }
    return lhs;
}

SymGroupCompare_f::SymGroupCompare_f(SymGroup input1, double tol) : group1(input1), tol(tol) {}

bool SymGroupCompare_f::operator()(const SymGroup& group2) const
{
    if (group1.operations().size() != group2.operations().size())
    {
        return false;
    }
    else
    {
        for (const SymOp& symop : group1.operations())
        {
            SymOpCompare_f compare_elements(symop, tol);
            if (std::find_if(group2.operations().begin(), group2.operations().end(), compare_elements) == group2.operations().end())
            {
                return false;
            }
        }

        return true;
    }
}

Eigen::Matrix3d make_z_rotation_matrix(double degrees)
{
    Eigen::AngleAxisd rotation_generator(degrees * M_PI / 180.0, Eigen::Vector3d(0, 0, 1));
    return rotation_generator.matrix();
}

std::vector<SymGroup> find_subgroups(SymGroup input_group)
{
    std::vector<SymGroup> list_of_subgroups;
    
    //Initializing a subgroup with each  element of the total group.
    for (const SymOp& operation : input_group.operations()) 
    {
        SymGroup pot_subgroup(std::vector<SymOp> {operation});
        if (pot_subgroup.operations().size() == input_group.operations().size()) 
        {
            continue;
        }

        SymGroupCompare_f compare_symgroups(pot_subgroup, PREC);

        if (std::find_if(list_of_subgroups.begin(), list_of_subgroups.end(), compare_symgroups) == list_of_subgroups.end()) 
        {
            list_of_subgroups.push_back(pot_subgroup);
        }
    }

    //Combining smaller subgroups into larger subgroups
    bool is_finished = false;
    while(!is_finished) 
    {
        is_finished = true;
        int last_size = list_of_subgroups.size();
        for (int a = 0; a < last_size; a++) 
        {
            for (int j = 0; j < last_size; j++) 
            {
                SymGroup pot_larger_subgroup = list_of_subgroups[a] * list_of_subgroups[j];

                if (pot_larger_subgroup.operations().size() == input_group.operations().size()) 
                {
                    continue;
                }

                SymGroupCompare_f compare_larger_symgroups(pot_larger_subgroup, 1e-5);

                if (find_if(list_of_subgroups.begin(), list_of_subgroups.end(), compare_larger_symgroups) == list_of_subgroups.end()) 
                {
                    list_of_subgroups.push_back(pot_larger_subgroup);
                    is_finished = false;
                }
            
            }   
        }
    }
    return list_of_subgroups;

}

