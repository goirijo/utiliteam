#include "./symmetry.hpp"
#define PREC 1e-6

SymOp operator*(const SymOp& lhs, const SymOp& rhs)
{
    Eigen::Matrix3d product = lhs.cart_matrix * rhs.cart_matrix;
    SymOp symop_product(product);
    return symop_product;
}

SymOpCompare_f::SymOpCompare_f(SymOp input1, double tol) : element1(input1), tol(tol) {}
bool SymOpCompare_f::operator()(const SymOp& element2) const { return element1.cart_matrix.isApprox(element2.cart_matrix, tol); }

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
    // if element not already there
    // return false
    SymOpCompare_f compare(new_operation, PREC);
    if(find_if(this->group.begin(), this->group.end(), compare)==this->group.end())
    {
        this->group.push_back(new_operation);
        close_group(&this->group);
        return true;
    }

    return false;
}

SymGroup::SymGroup(std::vector<SymOp> generating_elements)
{
    //what if you gave it duplicate elements?
    //
    for(SymOp element:generating_elements)
    {
        this->insert(element);
    }
}

SymGroup operator*(SymGroup lhs, const SymGroup& rhs)
{
    //combines all operations of the two groups and closes the result
    //TODO
    //combines groups in total group.
    
    for (auto op: rhs.operations())
    {
//        SymOpCompare_f compare(op, PREC);
        lhs.insert(op);
    }
    return lhs;
}


SymGroupCompare_f::SymGroupCompare_f(SymGroup input1) : group1(input1) {}

bool SymGroupCompare_f::operator()(const SymGroup& group2) const
{
    if (group1.operations().size() != group2.operations().size())
    {
        return false;
    }
    else
    {
        for (SymOp symop : group1.operations())
        {
            SymOpCompare_f compare_elements(symop, PREC);
            if (std::find_if(group2.operations().begin(), group2.operations().end(), compare_elements) == group2.operations().end())
            {
                return false;
            }
        }

        return true;
    }
}

void EXPECT_EQ_MATRICES(Eigen::Matrix3d matrix1, Eigen::Matrix3d matrix2, std::string test_name)
{
    if (matrix1 == matrix2)
    {
        std::cout << "PASSED " << test_name << std::endl;
    }

    else
    {
        std::cout << "FAILED " << test_name << std::endl;
    }

    return;
}

void EXPECT_EQ_VECTORS(std::vector<SymOp> vector1, std::vector<SymOp> vector2, std::string test_name)
{
    SymGroupCompare_f compare(vector1);
    if (compare(vector2))
    {
        std::cout << "PASSED " << test_name << std::endl;
    }

    else
    {
        std::cout << "FAILED " << test_name << std::endl;
    }

    return;
}
void EXPECT_TRUE(bool is_true, std::string test_name)
{
    if (is_true)
    {
        std::cout << "PASSED " << test_name << std::endl;
    }

    else
    {
        std::cout << "FAILED " << test_name << std::endl;
    }

    return;
}

Eigen::Matrix3d make_z_rotation_matrix(double degrees)
{
    Eigen::AngleAxisd rotation_generator(degrees * M_PI / 180.0, Eigen::Vector3d(0, 0, 1));
    return rotation_generator.matrix();
}

