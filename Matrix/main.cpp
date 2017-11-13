#include <iostream>

#include "Matrix.hpp"



int main()
{
	std::cout << "Hello world!" << std::endl;

	Mat::MatrixIndices indices(Mat::XY(2, 3));
	indices = Mat::MN(7, 5);

	Mat::MatrixIndices ind;

	std::getchar();
	return 0;
}

