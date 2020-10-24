#ifndef FILEOPERATOR_H
#define FILEOPERATOR_H

#include <fstream>
#include <string>
#include <iostream> //testing purposes
#include "polynomial_list.h"

class FileOperator {
private:
	std::string file_name_;
public:
	FileOperator();
	FileOperator(std::string);

	std::string Write(std::string);
	std::string ReadRegular();
	std::string ReadCanonical();
	std::string CanonicalForm(std::string, int);
	PolynomialList CanonicalList(std::string poly);
	std::string CanonicalFormMultiply(std::string, std::string);
};

#endif /* FILEOPERATOR_H */