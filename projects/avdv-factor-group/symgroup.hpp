#ifndef SYMGROUP_HH
#define SYMGROUP_HH

#include "symop.hpp"
#include <algorithm>
#include <tuple>
#include <vector>
#include <iostream>

template <typename Element>
struct DefaultCompare
{
    bool operator()(const Element& lhs, const Element& rhs) const { return lhs == rhs; }
};

template <typename Element>
struct DefaultMultiply
{
    Element operator()(const Element& lhs, const Element& rhs) const { return lhs * rhs; }
};

template <typename SymOpType, typename BinaryCompareType, typename MultiplyType=DefaultMultiply<SymOpType>>
class SymGroup
{
public:
    SymGroup(std::vector<SymOpType> generating_elements, const BinaryCompareType& binary_comparator, const MultiplyType& multiply_elements); 
    template<typename ...Args>
    SymGroup(std::vector<SymOpType> generating_elements, Args... args);
    static void close_group(std::vector<SymOpType>* operations_ptr, const BinaryCompareType& binary_comparator, MultiplyType multiply_elements);
    bool insert(SymOpType& new_operation);
    const std::vector<SymOpType>& operations() const { return group; }
    const BinaryCompareType get_comparator() const { return binary_comparator; }
    const MultiplyType get_multiplier() const { return multiply_elements; }
private:
    std::vector<SymOpType> group;
    BinaryCompareType binary_comparator;
    MultiplyType multiply_elements;
};

template <typename SymOpType,typename BinaryCompareType, typename MultiplyType = DefaultMultiply<SymOpType>>
SymGroup<SymOpType, BinaryCompareType, MultiplyType> operator*(SymGroup<SymOpType, BinaryCompareType, MultiplyType> lhs, const SymGroup<SymOpType, BinaryCompareType, MultiplyType>& rhs);

template <typename SymOpType,typename BinaryCompareType, typename MultiplyType = DefaultMultiply<SymOpType>>
struct SymGroupCompare_f
{
    SymGroupCompare_f(const SymGroup<SymOpType, BinaryCompareType, MultiplyType>& input1);

    bool operator()(const SymGroup<SymOpType, BinaryCompareType, MultiplyType>& group2) const;

private:
    const SymGroup<SymOpType, BinaryCompareType, MultiplyType> group1;
    double tol;
};

template <typename SymOpType, typename BinaryCompareType, typename MultiplyType = DefaultMultiply<SymOpType>>
std::vector<SymGroup<SymOpType, BinaryCompareType, MultiplyType>> find_subgroups(SymGroup<SymOpType, BinaryCompareType, MultiplyType> input_group);

//*********************************************************************//

template <typename SymOpType, typename BinaryCompareType, typename MultiplyType>
void SymGroup<SymOpType, BinaryCompareType, MultiplyType>::close_group(std::vector<SymOpType>* operations_ptr, const BinaryCompareType& binary_comparator, MultiplyType multiply_elements)
{
    int push_limit = 200;
    int push_count = 0;

    std::vector<SymOpType>& operations = *operations_ptr;

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
                SymOpType candidate_operation =multiply_elements(operations[i], operations[j]);
                auto compare_lambda=[candidate_operation, binary_comparator](const SymOpType& other){
                    return binary_comparator(other, candidate_operation);
                    };

                if (std::find_if(operations.begin(), operations.end(), compare_lambda) == operations.end())
                {
                    std::cout<<"Added Operation:\n"<<"SymOp Matrix: \n"<<candidate_operation.get_cart_matrix()<<"translation: \n"<<candidate_operation.get_translation()<<std::endl;
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

template <typename SymOpType, typename BinaryCompareType, typename MultiplyType>
bool SymGroup<SymOpType, BinaryCompareType, MultiplyType>::insert(SymOpType& new_operation)
{
                //TODO: Think of a way of not repeating this
    auto compare_lambda=[this, new_operation](const SymOpType& other){
        return this->binary_comparator(other, new_operation);
        };

    if (std::find_if(this->group.begin(), this->group.end(), compare_lambda) == this->group.end())
    {
        std::cout<<"Added Operation:\n"<<"SymOp Matrix: \n"<<new_operation.get_cart_matrix()<<"translation: \n"<<new_operation.get_translation()<<std::endl;
        this->group.push_back(new_operation);
        close_group(&this->group, this->binary_comparator, this->multiply_elements);
        return true;
    }

    return false;
}


template <typename SymOpType, typename BinaryCompareType, typename MultiplyType>
template<typename ...Args>
SymGroup<SymOpType, BinaryCompareType, MultiplyType>::SymGroup(std::vector<SymOpType> generating_elements, Args... args): SymGroup<SymOpType,BinaryCompareType, MultiplyType>(generating_elements,BinaryCompareType(args...), MultiplyType()){}

template <typename SymOpType, typename BinaryCompareType, typename MultiplyType>
SymGroup<SymOpType, BinaryCompareType, MultiplyType>::SymGroup(std::vector<SymOpType> generating_elements, const BinaryCompareType& binary_comparator, const MultiplyType& multiply_elements): binary_comparator(binary_comparator), multiply_elements(multiply_elements)
{
    for (SymOpType element : generating_elements)
    {
        this->insert(element);
    }
}

template <typename SymOpType, typename BinaryCompareType, typename MultiplyType>
SymGroup<SymOpType, BinaryCompareType, MultiplyType> operator*(SymGroup<SymOpType, BinaryCompareType, MultiplyType> lhs, const SymGroup<SymOpType, BinaryCompareType, MultiplyType>& rhs)
{
    // combines all operations of the two groups and closes the result
    // combines groups in total group.

    for (auto op : rhs.operations())
    {
        lhs.insert(op);
    }
    return lhs;
}

template <typename SymOpType, typename BinaryCompareType, typename MultiplyType>
SymGroupCompare_f<SymOpType, BinaryCompareType, MultiplyType>::SymGroupCompare_f(const SymGroup<SymOpType, BinaryCompareType, MultiplyType>& input1) : group1(input1){}

template <typename SymOpType,typename BinaryCompareType, typename MultiplyType>
bool SymGroupCompare_f<SymOpType, BinaryCompareType, MultiplyType>::operator()(const SymGroup<SymOpType, BinaryCompareType, MultiplyType>& group2) const
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

template <typename SymOpType, typename BinaryCompareType, typename MultiplyType>
std::vector<SymGroup<SymOpType, BinaryCompareType, MultiplyType>> find_subgroups(SymGroup<SymOpType, BinaryCompareType, MultiplyType> input_group)
{
    std::vector<SymGroup<SymOpType, BinaryCompareType, MultiplyType>> list_of_subgroups;
    
    //Initializing a subgroup with each  element of the total group.
    for (const SymOpType& operation : input_group.operations()) 
    {
        BinaryCompareType compare = input_group.get_comparator();
        MultiplyType multiplier = input_group.get_multiplier();
        SymGroup<SymOpType, BinaryCompareType, MultiplyType> pot_subgroup(std::vector<SymOpType> {operation}, compare, multiplier);
        if (pot_subgroup.operations().size() == input_group.operations().size()) 
        {
            continue;
        }
//check that the group compare makes sense
        
        SymGroupCompare_f<SymOpType, BinaryCompareType, MultiplyType> compare_symgroups(pot_subgroup);
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
                SymGroup<SymOpType, BinaryCompareType, MultiplyType> pot_larger_subgroup = list_of_subgroups[a] * list_of_subgroups[j];

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
