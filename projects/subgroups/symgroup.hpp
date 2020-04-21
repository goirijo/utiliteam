#ifndef SYMGROUP_HH
#define SYMGROUP_HH

#include "./symmetry.hpp"
#include <algorithm>
#include <tuple>
#include <vector>
//#define PREC 1e-6
//

//What we want:
//User specify the type of operations, and how to compare the operations
//The user gives the vector of operations at construction, as well
//as whatever is needed to compare two operations.
//
//For example:
//std::vector<SymOp> my_ops;
//SymOpBinaryCompare_f how_to_compare(1e-5);
//SymGroup<SymOp, SymOpBinaryCompare_f> group(my_ops, how_to_compare);

template <typename SymOpType, typename BinaryCompareType>
class SymGroup
{
public:
 
    SymGroup(std::vector<SymOpType> generating_elements, const BinaryCompareType& binary_comparator);
    static void close_group(std::vector<SymOpType>* operations_ptr, const BinaryCompareType& binary_comparator);
    bool insert(SymOpType& new_operation);
    const std::vector<SymOpType>& operations() const { return group; }
    const BinaryCompareType get_comparator() const {return binary_comparator;}
private:
    std::vector<SymOpType> group;
    BinaryCompareType binary_comparator;
};

template <typename SymOpType,typename BinaryCompareType>
SymGroup<SymOpType, BinaryCompareType> operator*(SymGroup<SymOpType, BinaryCompareType> lhs, const SymGroup<SymOpType, BinaryCompareType>& rhs);

template <typename SymOpType,typename BinaryCompareType>
struct SymGroupCompare_f
{
    SymGroupCompare_f(const SymGroup<SymOpType,BinaryCompareType>& input1);

    bool operator()(const SymGroup<SymOpType,BinaryCompareType>& group2) const;

private:
    const SymGroup<SymOpType,BinaryCompareType> group1;
    double tol;
};

template <typename SymOpType, typename BinaryCompareType>
std::vector<SymGroup<SymOpType, BinaryCompareType>> find_subgroups(SymGroup<SymOpType, BinaryCompareType> input_group);

//*********************************************************************//

template <typename SymOpType, typename BinaryCompareType>
void SymGroup<SymOpType, BinaryCompareType>::close_group(std::vector<SymOpType>* operations_ptr, const BinaryCompareType& binary_comparator)
{
    int push_limit = 200;
    int push_count = 0;

    std::vector<SymOpType>& operations = *operations_ptr;
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
                //TODO: Think of a way of not repeating this
                SymOpType candidate_operation = operations[i] * operations[j];
                auto compare_lambda=[candidate_operation, binary_comparator](const SymOpType& other){
                    return binary_comparator(other, candidate_operation);
                    };

                if (std::find_if(operations.begin(), operations.end(), compare_lambda) == operations.end())
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

}

template <typename SymOpType, typename BinaryCompareType>
bool SymGroup<SymOpType, BinaryCompareType>::insert(SymOpType& new_operation)
{
                //TODO: Think of a way of not repeating this
    auto compare_lambda=[this, new_operation](const SymOpType& other){
        return this->binary_comparator(other, new_operation);
        };

    if (std::find_if(this->group.begin(), this->group.end(), compare_lambda) == this->group.end())
    {
        this->group.push_back(new_operation);
        close_group(&this->group, this->binary_comparator);
        return true;
    }

    return false;
}

template <typename SymOpType, typename BinaryCompareType>
SymGroup<SymOpType, BinaryCompareType>::SymGroup(std::vector<SymOpType> generating_elements, const BinaryCompareType& binary_comparator): binary_comparator(binary_comparator)
{
    for (SymOpType element : generating_elements)
    {
        this->insert(element);
    }
}

template <typename SymOpType, typename BinaryCompareType>
SymGroup<SymOpType, BinaryCompareType> operator*(SymGroup<SymOpType, BinaryCompareType> lhs, const SymGroup<SymOpType, BinaryCompareType>& rhs)
{
    // combines all operations of the two groups and closes the result
    // combines groups in total group.

    for (auto op : rhs.operations())
    {
        lhs.insert(op);
    }
    return lhs;
}

template <typename SymOpType, typename BinaryCompareType>
SymGroupCompare_f<SymOpType, BinaryCompareType>::SymGroupCompare_f(const SymGroup<SymOpType, BinaryCompareType>& input1) : group1(input1){}

template <typename SymOpType,typename BinaryCompareType>
bool SymGroupCompare_f<SymOpType, BinaryCompareType>::operator()(const SymGroup<SymOpType, BinaryCompareType>& group2) const
{
    if (group1.operations().size() != group2.operations().size())
    {
        return false;
    }
    else
    {
        BinaryCompareType b_compare= group1.get_comparator();
        for (const SymOpType& symop : group1.operations())
        {
            auto compare_elements=[symop, b_compare](const SymOpType& other){
                return b_compare(symop, other);};
            if (std::find_if(group2.operations().begin(), group2.operations().end(), compare_elements) == group2.operations().end())
            {
                return false;
            }
        }

        return true;
    }
}

template <typename SymOpType, typename BinaryCompareType>
std::vector<SymGroup<SymOpType, BinaryCompareType>> find_subgroups(SymGroup<SymOpType, BinaryCompareType> input_group)
{
    std::vector<SymGroup<SymOpType, BinaryCompareType>> list_of_subgroups;
    
    //Initializing a subgroup with each  element of the total group.
    for (const SymOpType& operation : input_group.operations()) 
    {
        BinaryCompareType compare = input_group.get_comparator();
        SymGroup<SymOpType, BinaryCompareType> pot_subgroup(std::vector<SymOpType> {operation}, compare);
        if (pot_subgroup.operations().size() == input_group.operations().size()) 
        {
            continue;
        }
//check that the group compare makes sense
        
        SymGroupCompare_f<SymOpType, BinaryCompareType> compare_symgroups(pot_subgroup);
        auto compare_groups=[compare_symgroups, pot_subgroup](const SymGroup<SymOpType, BinaryCompareType>& other){
            return compare_symgroups(other);};

        if (std::find_if(list_of_subgroups.begin(), list_of_subgroups.end(), compare_groups) == list_of_subgroups.end()) 
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
                SymGroup<SymOpType, BinaryCompareType> pot_larger_subgroup = list_of_subgroups[a] * list_of_subgroups[j];

                if (pot_larger_subgroup.operations().size() == input_group.operations().size()) 
                {
                    continue;
                }

             auto compare_large_groups=[pot_larger_subgroup](const SymGroup<SymOpType, BinaryCompareType>& other){
                const SymGroupCompare_f<SymOpType, BinaryCompareType> compare_symgroups(pot_larger_subgroup);
                return compare_symgroups(other);};

                if (std::find_if(list_of_subgroups.begin(), list_of_subgroups.end(), compare_large_groups) == list_of_subgroups.end()) 
                {
                    list_of_subgroups.push_back(pot_larger_subgroup);
                    is_finished = false;
                }
            
            }   
        }
    }
    return list_of_subgroups;

}

#endif
