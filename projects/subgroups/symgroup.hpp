#ifndef SYMGROUP_HH
#define SYMGROUP_HH

#include "./symmetry.hpp"
#include <algorithm>
#include <tuple>
#include <vector>
//#define PREC 1e-6

template <typename SymOpType, typename BinaryCompareType>
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

template <typename SymOpType,typename BinaryCompareType>
SymGroup<SymOpType, BinaryCompareType> operator*(SymGroup<SymOpType, BinaryCompareType> lhs, const SymGroup<SymOpType, BinaryCompareType>& rhs);

template <typename SymOpType,typename BinaryCompareType>
struct SymGroupCompare_f
{
    SymGroupCompare_f(SymGroup<SymOpType,BinaryCompareType> input1);

    bool operator()(const SymGroup<SymOpType,BinaryCompareType>& group2) const;

private:
    const SymGroup<SymOpType,BinaryCompareType> group1;
    double tol;
};

template <typename SymOpType, typename BinaryCompareType>
std::vector<SymGroup<SymOpType, BinaryCompareType>> find_subgroups(SymGroup<SymOpType, BinaryCompareType> input_group);

//*********************************************************************//

template <typename SymOpType, typename BinaryCompareType>
void SymGroup<SymOpType, BinaryCompareType>::close_group(std::vector<SymOpType>* operations_ptr)
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
                SymOpType candidate_operation = operations[i] * operations[j];
                auto compare_lambda=[ candidate_operation](const SymOpType& other){
                    BinaryCompareType b_compare(candidate_operation, other);
                    return b_compare();};
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
//    BinaryCompareType compare;
    auto compare_lambda=[new_operation](const SymOpType& other){
        BinaryCompareType compare(other, new_operation);
        return compare();};
    if (std::find_if(this->group.begin(), this->group.end(), compare_lambda) == this->group.end())
    {
        this->group.push_back(new_operation);
        close_group(&this->group);
        return true;
    }

    return false;
}

template <typename SymOpType, typename BinaryCompareType>
SymGroup<SymOpType, BinaryCompareType>::SymGroup(std::vector<SymOpType> generating_elements)
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
SymGroupCompare_f<SymOpType, BinaryCompareType>::SymGroupCompare_f(SymGroup<SymOpType, BinaryCompareType> input1) : group1(input1){}

template <typename SymOpType,typename BinaryCompareType>
bool SymGroupCompare_f<SymOpType, BinaryCompareType>::operator()(const SymGroup<SymOpType, BinaryCompareType>& group2) const
{
    if (group1.operations().size() != group2.operations().size())
    {
        return false;
    }
    else
    {
        for (const SymOpType& symop : group1.operations())
        {
            auto compare_elements=[symop](const SymOpType& other){
                BinaryCompareType b_compare(symop, other);
                return b_compare(other, symop);};
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
        SymGroup<SymOpType, BinaryCompareType> pot_subgroup(std::vector<SymOpType> {operation});
        if (pot_subgroup.operations().size() == input_group.operations().size()) 
        {
            continue;
        }
//check that the group compare makes sense
        
        auto compare_groups=[pot_subgroup](const SymOpType& other){
            SymGroupCompare_f<SymOpType, BinaryCompareType> compare_symgroups(pot_subgroup);
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

             auto compare_large_groups=[pot_larger_subgroup](const SymOpType& other){
                SymGroupCompare_f<SymOpType, BinaryCompareType> compare_symgroups(pot_larger_subgroup, other);
                return compare_symgroups();};

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
