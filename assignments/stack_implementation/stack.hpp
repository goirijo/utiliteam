class IntStack
{
    public:

        /// Returns true if there are no elements on the stack
        bool empty() const;

        /// Returns the number of elements on the stack
        int size() const;

        /// Return the top element of the stack, i.e. the last element that was pushed
        int top() const;

        /// Adds a new element to the top of the stack
        void push(const int& new_value);

        /// Remove the topmost element of the stack
        void pop();

    private:
};
