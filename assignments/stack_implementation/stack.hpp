#include <vector>

class IntStack
{
    public:

        /// Returns true if there are no elements on the stack
        bool empty() const
        {
            std::vector<int> myStack = m_stack_list;
            if (myStack.size() == 0) 
            {
                return true;
            }
            else 
            {
                return false;
            }
        }

        /// Returns the number of elements on the stack
        int size() const
        {
            std::vector<int> myStack = m_stack_list;
            int size = myStack.size();
            return size;
        }

        /// Return the top element of the stack, i.e. the last element that was pushed
        int top() const
        {
            std::vector<int> myStack = m_stack_list;
            int top_value = myStack.back();
            return top_value;
        }

        /// Adds a new element to the top of the stack
        void push(const int& new_value) 
        {
            std::vector<int> myStack = m_stack_list;
            myStack.push_back(new_value);
            return;
        }

        /// Remove the topmost element of the stack
        void pop()
        {
            std::vector<int> myStack = m_stack_list;
            myStack.pop_back();
            return;
        }

    private:

        std::vector<int> m_stack_list;

};
