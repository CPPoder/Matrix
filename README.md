# Matrix
Defines the class templates Matrix and Vector, that have a variety of functionalities:

- Several different Constructors

- Generalized access in XY- or MN-mode (In mathematics, the first index of a matrix A_mn specifies the row. To this I refer to as MN-mode.
 However, I prefer to specify the column first, 
 because this is analogous to coordinate systems where the first number specifies the x-direction.
 To this I refer to as XY-mode.)
 
 E.g. you can create a 4x3 matrix of Integers by the following line of code:
 
 ```cpp
 Mat::Matrix<int> exampleMatrix(Mat::MN(4,3));
 ```
 
 Which is equivalent to:
 
 ```cpp
 Mat::Matrix<int> exampleMatrix(Mat::XY(3,4));
 ```

- Modifying functionalities, like swapRows, transpose, resize, fillWith or doForEveryEntry

- Mathematical operations between matrix and matrix, matrix and vector and vector and vector

- Mathematical functions, like: trace, det

E.g. the following code calculates the matrix product of two compatible matrices:

```cpp
Mat::Matrix<int> m1(Mat::MN(4,3));
Mat::Matrix<int> m2(Mat::MN(3,2));
Mat::Matrix<int> result = m1 * m2;
Mat::MatrixSize size = result.getSize();
std::cout << size.m() << ", " << size.n() << std::endl;
```

The output is then the size of the matrix in MN-mode: 4, 2
