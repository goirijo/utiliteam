#include <vector>

class IntStack
{
    public:

        /// Returns true if there are no elements on the stack
        bool empty() const
        {
            if (m_stack_list.size() == 0) 
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
            int size = m_stack_list.size();
            return size;
        }

        /// Return the top element of the stack, i.e. the last element that was pushed
        int top() const
        {
            int top_value = m_stack_list.back();
            return top_value;
        }

        /// Adds a new element to the top of the stack
        void push(const int& new_value) 
        {
            m_stack_list.push_back(new_value);
            return;
        }

        /// Remove the topmost element of the stack
        void pop()
        {
            m_stack_list.pop_back();
            return;
        }

    private:

        std::vector<int> m_stack_list;

};
