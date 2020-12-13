#include "file_operator.h"

FileOperator::FileOperator()
{
	file_name_ = "";
}

FileOperator::FileOperator(string file_name)
{
	this->file_name_ = file_name;
}

string FileOperator::Write(string s) {
	fstream new_file;
	new_file.open("output.txt", ios::out | ios::app | ios::binary);

	if (new_file.is_open()) {
		new_file << s;
	}

	new_file.close();
	return file_name_;
}

string FileOperator::ReadCanonical()
{
	fstream my_file;
	string all_canonical_poly; //stores every canonical poly in file
	my_file.open(file_name_, ios::in | ios::binary);

	if (my_file.is_open()) {
		string tmp;

		while (getline(my_file, tmp)) {
			all_canonical_poly.append(CanonicalForm(tmp, 1));
			all_canonical_poly.append("//"); //delimeter
		}
	}

	my_file.close();
	return all_canonical_poly;
}

string FileOperator::ReadRegular()
{
	fstream my_file;
	string all_poly; //stores every poly in file
	my_file.open(file_name_, ios::in | ios::binary);

	if (my_file.is_open()) {
		string tmp;

		while (getline(my_file, tmp)) {
			all_poly.append(tmp);
			all_poly.append("//"); //delimeter
		}
	}

	my_file.close();
	return all_poly;
}

string FileOperator::CanonicalForm(string poly, int option)
{
	string number = "", coeff = "", exp = "";

	PolyMap map;

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
				// change 77 to 83
				if (option == 1) {
					map.Insert(stoi(coeff), stoi(exp));
				}
				else { //used for subtraction
					map.Insert(stoi(coeff) * -1, stoi(exp));
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
		// change 97 to 103
		if (option == 1) {
			map.Insert(stoi(coeff), stoi(exp));
		}
		else { //used for subtraction
			map.Insert(stoi(coeff) * -1, stoi(exp));
		}

		pair = 1;
		number = "";
	}
	return map.GetCanonicalString();
}

PolyMap FileOperator::CanonicalMap(string poly)
{
	string number = "", coeff = "", exp = "";

	PolyMap map;

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
				map.Insert(stoi(coeff), stoi(exp));
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
		map.Insert(stoi(coeff), stoi(exp));
	}

	return map;
}

string FileOperator::CanonicalFormMultiply(string poly_A, string poly_B)
{
	PolyMap map_A = CanonicalMap(poly_A);
	PolyMap map_B = CanonicalMap(poly_B);

	return map_A.Multiply(map_B);
}