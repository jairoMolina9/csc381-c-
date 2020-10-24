#include "file_operator.h"

FileOperator::FileOperator() {
	file_name_ = "";
}

FileOperator::FileOperator(std::string file_name) {
	this->file_name_ = file_name;
}

std::string FileOperator::Write(std::string s) {
	std::fstream new_file;
	new_file.open("output.txt", std::ios::out | std::ios::app | std::ios::binary);

	if (new_file.is_open()) {
		new_file << s;
	}

	new_file.close();
	return file_name_;
}

std::string FileOperator::ReadCanonical() {
	std::fstream my_file;
	std::string all_canonical_poly; //stores every canonical poly in file
	my_file.open(file_name_, std::ios::in | std::ios::binary);

	if(my_file.is_open()) {
		std::string tmp;

		while (getline(my_file, tmp)) {
			all_canonical_poly.append(CanonicalForm(tmp, 1));
			all_canonical_poly.append("//"); //delimeter
		}
	}
	
	my_file.close();
	return all_canonical_poly;
}

std::string FileOperator::ReadRegular() {
	std::fstream my_file;
	std::string all_poly; //stores every poly in file
	my_file.open(file_name_, std::ios::in | std::ios::binary);

	if (my_file.is_open()) {
		std::string tmp;

		while (getline(my_file, tmp)) {
			all_poly.append(tmp);
			all_poly.append("//"); //delimeter
		}
	}

	my_file.close();
	return all_poly;
}

std::string FileOperator::CanonicalForm(std::string poly, int option) {
	string number = "", coeff = "", exp = "";

	PolynomialList list;

	int pair = 1;

	for (const char &c : poly)
	{
		if (c == ' ')
		{
			if (pair == 1) { //gets coeff
				coeff = number;
				number = "";
				pair++;
			} else if (pair == 2) { //gets exp
				exp = number;
				if (option == 1) {
					list.insert_ascending(stoi(coeff), stoi(exp));
				}
				else { //used for subtraction
					list.insert_ascending(stoi(coeff)*-1, stoi(exp));
				}
				pair = 1;
				number = "";
			}
		}
		else
		{
			number += c;
		}
	}

	if (pair == 2) { //get last exp
		exp = number;
		if (option == 1) {
			list.insert_ascending(stoi(coeff), stoi(exp));
		}
		else { //used for subtraction
			list.insert_ascending(stoi(coeff) * -1, stoi(exp));
		}
		pair = 1;
		number = "";
	}
	return list.GetCanonicalString();
}

PolynomialList FileOperator::CanonicalList(std::string poly) {
	string number = "", coeff = "", exp = "";

	PolynomialList list;

	int pair = 1;

	for (const char& c : poly)
	{
		if (c == ' ')
		{
			if (pair == 1) { //gets coeff
				coeff = number;
				number = "";
				pair++;
			}
			else if (pair == 2) { //gets exp
				exp = number;
				pair = 1;
				number = "";
				list.insert_ascending(stoi(coeff), stoi(exp));
			}
		}
		else
		{
			number += c;
		}
	}

	if (pair == 2) { //get last exp
		exp = number;
		pair = 1;
		number = "";
		list.insert_ascending(stoi(coeff), stoi(exp));
	}

	return list;
}

std::string FileOperator::CanonicalFormMultiply(std::string poly_A, std::string poly_B) {
	PolynomialList list_A = CanonicalList(poly_A);
	PolynomialList list_B = CanonicalList(poly_B);

	return list_A.multiply(list_B);
}