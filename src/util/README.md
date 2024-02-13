# Utilities
This folder consists of all the utilies for the project. This includes the following:
* CSV Parser
* Matrix library
* Deque data structure

## CSV Parser
This is a simple CSV parser that is used to parse the stock data. It is a simple class that reads the file and stores the data in a vector of strings. 


### CSV Reader
This is a simple class that reads the file and stores the data in a vector of strings.
The constructor is initialized with the name of the csv file.
It is meant to be used as an iterator. The end of iteration is signalled by a empty vector.

### CSV Writer
This is analogous to the reader. It is used to write the data to a csv file. The constructor is initialized with the name of the csv file.
A templated `write_line` function is used to write a line to the csv file. It writes a `,` separated line to the file, with all the entries in the input vector.

## Matrix Library
This is a simple matrix library that is used to perform matrix operations. This utility is used to train the linear regression model.
4 functions are provided for basic matrix operations:
* `transpose_matrix` : Transposes the input matrix
* `multiply_matrix` : Multiplies two matrices
* `inverse_matrix` : Inverts the input matrix
* `determinant` : Returns the determinant of the input matrix

## Deque
As we've used the `std::deque` data structure in the project alot, we've implemented a simple `Deque` class that is used to perform the operations on the deque. 

The need for this custom data structure arose because in some implementations of the `std::deque`, the container is a Doubly Linked List, which is not efficient for our use case. Hence, we implemented a simple deque using a vector. The implementation is very similar to the Major Question given in COL106 course.

Although this class has been defined, It is yet to be integrated with the project. The project currently uses the `std::deque` data structure.