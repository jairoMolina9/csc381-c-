#include "poly_map.h"

PolyMap::PolyMap() { }

PolyMap::~PolyMap()
{
	map.clear();
}

void PolyMap::Insert(int coeff, int exp)
{
	/*
		Works for both - and +
	*/

	if (!map.count(exp)) {
		map[exp] = coeff;
	}
	else {
		map.at(exp) += coeff;
	}
}

string PolyMap::Multiply(PolyMap other)
{
	PolyMap new_map;

	for (auto i = this->map.begin(); i != this->map.end(); ++i) {
		for (auto j = other.map.begin(); j != other.map.end(); ++j) {
			int new_val = this->map.at(i->first) * other.map.at(j->first);
			int new_key = i->first + j->first;
			new_map.Insert(new_val, new_key);
		}
	}

	return new_map.GetCanonicalString();
}


string PolyMap::GetCanonicalString()
{
	string poly;

	for (auto itr = map.rbegin(); itr != map.rend(); ++itr) {
		if (itr->second == 0) continue;
		poly.append(to_string(itr->second) + ' ' + to_string(itr->first) + ' ');
	}

	if (poly == "")
		poly = "0";

	return poly;
}
