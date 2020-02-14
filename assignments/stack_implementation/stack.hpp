#include <vector>

class IntStack
{
    public:

        /// Returns the number of elements on the stack
        int size() const
        {
            return this->top_index+1;
        }

        /// Returns true if there are no elements on the stack
        bool empty() const
        {
            return this->size()==0;
        }

        /// Return the top element of the stack, i.e. the last element that was pushed
        int top() const
        {
            return this->stack_values.at(this->top_index);
        }

        /// Adds a new element to the top of the stack
        void push(const int& new_value)
        {
            ++top_index;
            if(top_index==stack_values.size())
            {
                stack_values.push_back(new_value);
            }

            else
            {
                stack_values[top_index]=new_value;
            }
            return;
        }

        /// Remove the topmost element of the stack
        void pop()
        {
            --top_index;
            return;
        }

    private:

        std::vector<int> stack_values;
        int top_index=-1;
};
