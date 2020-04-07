#ifndef SYMGROUP_HH
#define SYMGROUP_HH

#include "./symmetry.hpp"
#include <algorithm>
#include <tuple>
#include <vector>
#define PREC 1e-6

template <typename SymOpType, typename SymOpCompareType_f>
class SymGroup
{
public:
 
    template<typename ...Args>
    SymGroup(std::vector<SymOpType> generating_elements, Args... compare_args);
    SymGroup(std::vector<SymOpType> generating_elements, double tol);
    static void close_group(std::vector<SymOpType>* operations_ptr);
    bool insert(SymOpType& new_operation);
    const std::vector<SymOpType>& operations() const { return this->group; }

private:
    std::vector<SymOpType> group;
    typename SymOpCompareType_f::CompareArgs compare_args;
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

//*********************************************************************//

template <typename SymOpType, typename SymOpCompareType_f>
void SymGroup<SymOpType, SymOpCompareType_f>::close_group(std::vector<SymOpType>* operations_ptr)
{
    int push_limit = 200;
    int push_count = 0;

    SymOpType& operations = *operations_ptr;
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
                SymOpCompareType_f has_candidate(candidate_operation, PREC);
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

}

template <typename SymOpType, typename SymOpCompareType_f>
bool SymGroup<SymOpType, SymOpCompareType_f>::insert(SymOpType& new_operation)
{
    //TODO: Figure this out so that it accepts parameter packs
    SymOpCompareType_f compare(new_operation);
    if (find_if(this->group.begin(), this->group.end(), compare) == this->group.end())
    {
        this->group.push_back(new_operation);
        close_group(&this->group);
        return true;
    }

    return false;
}

template <typename SymOpType, typename SymOpCompareType_f>
template<typename ...Args>
SymGroup<SymOpType, SymOpCompareType_f>::SymGroup(std::vector<SymOpType> generating_elements, Args... comparator_args):
    compare_args(std::make_tuple(comparator_args...))
{
    for (SymOpType element : generating_elements)
    {
        this->insert(element);
    }
}

template <typename SymOpType, typename SymOpCompareType_f>
SymGroup<SymOpType, SymOpCompareType_f> operator*(SymGroup<SymOpType, SymOpCompareType_f> lhs, const SymGroup<SymOpType, SymOpCompareType_f>& rhs)
{
    // combines all operations of the two groups and closes the result
    // combines groups in total group.

    for (auto op : rhs.operations())
    {
        lhs.insert(op);
    }
    return lhs;
}

template <typename SymOpType, typename SymOpCompareType_f>
SymGroupCompare_f<SymOpType, SymOpCompareType_f>::SymGroupCompare_f(SymGroup<SymOpType, SymOpCompareType_f> input1, double tol) : group1(input1), tol(tol) {}

template <typename SymOpType,typename SymOpCompareType_f>
bool SymGroupCompare_f<SymOpType, SymOpCompareType_f>::operator()(const SymGroup<SymOpType, SymOpCompareType_f>& group2) const
{
    if (group1.operations().size() != group2.operations().size())
    {
        return false;
    }
    else
    {
        for (const SymOpType& symop : group1.operations())
        {
            SymOpCompareType_f compare_elements(symop, tol);
            if (std::find_if(group2.operations().begin(), group2.operations().end(), compare_elements) == group2.operations().end())
            {
                return false;
            }
        }

        return true;
    }
}

template <typename SymOpType, typename SymOpCompareType_f>
std::vector<SymGroup<SymOpType, SymOpCompareType_f>> find_subgroups(SymGroup<SymOpType, SymOpCompareType_f> input_group)
{
    std::vector<SymGroup<SymOpType, SymOpCompareType_f>> list_of_subgroups;
    
    //Initializing a subgroup with each  element of the total group.
    for (const SymOp& operation : input_group.operations()) 
    {
        SymGroup<SymOpType, SymOpCompareType_f> pot_subgroup(std::vector<SymOp> {operation});
        if (pot_subgroup.operations().size() == input_group.operations().size()) 
        {
            continue;
        }

        SymOpCompareType_f compare_symgroups(pot_subgroup, PREC);

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
                SymGroup<SymOpType, SymOpCompareType_f> pot_larger_subgroup = list_of_subgroups[a] * list_of_subgroups[j];

                if (pot_larger_subgroup.operations().size() == input_group.operations().size()) 
                {
                    continue;
                }

                SymGroupCompare_f<SymOpType, SymOpCompareType_f> compare_larger_symgroups(pot_larger_subgroup, 1e-5);

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


#endif
