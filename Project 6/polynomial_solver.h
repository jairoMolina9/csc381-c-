#ifndef POLYNOMIALSOLVER_H
#define POLYNOMIALSOLVER_H

#include <string>
#include <vector>
#include <iostream> //testing purposes
#include "file_operator.cpp"

class PolynomialSolver {
private:
	FileOperator file_operator;
public:
	PolynomialSolver(string);
	void Execute();

	string Sum(string, string);
	string Subtract(string, string);
	string Multiply(string, string);

	string clean(string str);
};

#endif /* POLYNOMIALSOLVER_H */
