#include <vector>

class IntStack
{
    public:

        /// Returns true if there are no elements on the stack
        bool empty() const{
		my_vec.empty();};

        /// Returns the number of elements on the stack
        int size() const{
		my_vec.size();};

        /// Return the top element of the stack, i.e. the last element that was pushed
        int top() const{
		my_vec.front();};

        /// Adds a new element to the top of the stack
        void push(const int& new_value){
		my_vec.emplace(my_vec.begin(),new_value);};

        /// Remove the topmost element of the stack
        void pop(){
		my_vec.pop_back();};

    private:
	std::vector<int> my_vec;

};
//bool IntStack::empty()
//{

//}

//int Instack::size()
//{
	
//}
