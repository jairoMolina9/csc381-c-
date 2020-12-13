#include <iostream>
#include <cstring>

using namespace std;

#ifndef BIGINTEGER_H
#define BIGINTEGER_H

class BigInteger {
private:
	char *bint;
	size_t size;
	size_t length;

	BigInteger stringToBigInt(int, string);

public:
	BigInteger();

	BigInteger(int);

	BigInteger(char*);

	BigInteger(const BigInteger&);

	BigInteger &operator=(const BigInteger&);

	~BigInteger();

	BigInteger operator+(BigInteger&);

	BigInteger operator-(BigInteger&);

	BigInteger operator*(BigInteger&);

	BigInteger operator/(BigInteger&);

	bool operator<(BigInteger& other);

	bool operator==(BigInteger& other);

	friend ostream& operator<<(ostream& , const BigInteger&);;
	friend istream& operator>>(istream& , BigInteger&);
};

#endif
