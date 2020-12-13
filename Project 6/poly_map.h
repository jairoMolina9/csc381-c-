#ifndef POLYMAP_H
#define POLYMAP_H

#include <iostream>
#include <iterator>
#include <string>
#include <map>

using namespace std;

class PolyMap {
private:
	map<int, int> map;
public:
	PolyMap();
	~PolyMap();

	void Insert(int, int);
	string Multiply(PolyMap other);
	string GetCanonicalString();
};

#endif /* POLYMAP_H */