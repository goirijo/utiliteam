#include <stack>

class IntStack
{
    public:

        /// Returns true if there are no elements on the stack
        bool empty() const{my_stack.empty();};

        /// Returns the number of elements on the stack
        int size() const{my_stack.size();};

        /// Return the top element of the stack, i.e. the last element that was pushed
        int top() const{my_stack.top();};

        /// Adds a new element to the top of the stack
        void push(const int& new_value){my_stack.push(new_value);};

        /// Remove the topmost element of the stack
        void pop(){my_stack.pop();};

    private:
	std::stack<int> my_stack;

};
//bool IntStack::empty()
//{

//}

//int Instack::size()
//{
	
//}
