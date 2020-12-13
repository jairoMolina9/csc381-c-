#ifndef FILEOPERATOR_H
#define FILEOPERATOR_H

#include <fstream>
#include "poly_map.cpp"

class FileOperator {
private:
	string file_name_;
public:
	FileOperator();
	FileOperator(string);

	string Write(string);
	string ReadRegular();
	string ReadCanonical();
	string CanonicalForm(string, int);
	string CanonicalFormMultiply(string, string);

	PolyMap CanonicalMap(string poly);
};

#endif /* FILEOPERATOR_H */