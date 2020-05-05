#ifndef MATH_HH
#define MATH_HH

template <typename MatrixType>
bool is_unitary(const MatrixType& input_matrix, double tol)
{
    MatrixType product = input_matrix.transpose() * input_matrix;
    return product.isIdentity(tol);
}

#endif
