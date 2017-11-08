#include <iostream>

#include "Matrix.hpp"



int main()
{
	std::cout << "Hello world!" << std::endl;
	Matrix<int> mat(Matrix<int>::Size(4u, 7u));
	std::getchar();
	return 0;
}

