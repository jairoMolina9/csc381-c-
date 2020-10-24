#ifndef POLYNOMIALLIST_H
#define POLYNOMIALLIST_H

#include <iostream>
#include <string>

using namespace std;

/*Generic node structure*/
struct Node {

	int exp;
	int val;
	Node* next;
	Node* back;
};

class PolynomialList {
private:
	Node* head;
	Node* tail;
	int length;

	void copyAll(const PolynomialList&);
	void removeAll();

public:
	PolynomialList();
	~PolynomialList();
	PolynomialList(const PolynomialList&);
	PolynomialList& operator=(const PolynomialList&);

	void insert_ascending(int, int);
	void insert_front(int, int);
	void insert_back(int, int);
	void printFromFront();
	bool isEmpty();
	bool delete_item(int);
	int get_length() const;
	string GetCanonicalString();
	string multiply(PolynomialList);
};

#endif /* POLYNOMIALLIST_H */
