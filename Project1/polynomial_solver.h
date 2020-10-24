#ifndef POLYNOMIALSOLVER_H
#define POLYNOMIALSOLVER_H

#include <string>
#include <vector>
#include <iostream> //testing purposes
#include "file_operator.h"

class PolynomialSolver {
private:
	FileOperator file_operator;
public:
	PolynomialSolver(std::string);
	void Execute();
	std::string Sum(std::string, std::string);
	std::string Subtract(std::string, std::string);
	std::string Multiply(std::string, std::string);
};

#endif /* POLYNOMIALSOLVER_H */
