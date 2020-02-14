#include <list>

class IntStack
{
    public:

        /// Returns true if there are no elements on the stack
        bool empty() const
        {
            return this->stack_values.size()==0;
        }

        /// Returns the number of elements on the stack
        int size() const
        {
            return this->stack_values.size();
        }

        /// Return the top element of the stack, i.e. the last element that was pushed
        int top() const
        {
            return this->stack_values.front();
        }

        /// Adds a new element to the top of the stack
        void push(const int& new_value)
        {
            return this->stack_values.push_front(new_value);
        }

        /// Remove the topmost element of the stack
        void pop()
        {
            return this->stack_values.pop_front();
        }

    private:

        std::list<int> stack_values;
};
