#include "BigInteger.h"

BigInteger::BigInteger()
{
	bint = new char[1];
	bint[0] = '0';
	size = 1;
	length = 1;
}

BigInteger::BigInteger(int number)
{
	//int data type max is 10 digits +1 for '\0'
	size =  11;
	
	bint = new char[size];

	sprintf(bint,"%d", number);

	length = strlen(bint);
}

BigInteger::BigInteger(char* number)
{
	size = strlen(number) + 1;

	bint = new char[size];

	strcpy(bint, number);

	length = size-1;
}

BigInteger::BigInteger(const BigInteger& other)
{
	size = strlen(other.bint) + 1;

	bint = new char[size];

	strcpy(bint, other.bint);

	length = size-1;
}

BigInteger& BigInteger::operator=(const BigInteger& other)
{
	delete[] bint;
	
	size = strlen(other.bint) + 1;

	bint = new char[size];
	
	strcpy(bint, other.bint);
	
	length = size-1;

	return *this;
}

BigInteger::~BigInteger()
{
	delete [] bint;
	bint = nullptr;

	size = 0;
	length = 0;
}

ostream& operator<<(ostream& output, const BigInteger& obj) {
	for (int i = 0; i < obj.length; i++)
		output << obj.bint[i];

	return output;
}

istream& operator>>(istream& input, BigInteger& obj) {
	string str_input;

	input >> str_input;
	
	size_t size = str_input.length() + 1;
	
	obj.bint = new char[size];

	strcpy(obj.bint, str_input.c_str());

	obj.size = size+1;
	obj.length = str_input.length();

	return input;
}


BigInteger BigInteger::operator+(BigInteger& other)
{
	int this_iter = length-1;
	int other_iter = other.length-1;

	size_t new_size = (this->size > other.size) ? this->size + 1 : other.size + 1;

	string s_sum = "";

	int result = 0;
	int carry = 0;

	while (this_iter != -1 || other_iter != -1) {

		/*
			case 1: ONLY check RIGHT
			case 2: ONLY check LEFT
			case 3: CHECK both
		*/

		if (other_iter == -1) {
			result = (bint[this_iter]-'0') + carry;
			this_iter--;
		}
		else if (this_iter == -1) {
			result = (other.bint[other_iter]-'0') + carry;
			other_iter--;
		}
		else {
			result = ( (bint[this_iter] - '0') + (other.bint[other_iter] - '0')) + carry;
			this_iter--;
			other_iter--;
		}

		/*
			case 1: double digit
			case 2: single digit
		*/

		if (result > 9) {
			carry = 1;

			s_sum = (char)((result-10) + '0') + s_sum;
		}
		else {
			carry = 0;
			s_sum = (char)(result + '0') + s_sum;
		}
	}

	if (carry == 1)
		s_sum = '1' + s_sum;

	BigInteger sum = stringToBigInt(new_size, s_sum);

	return sum;
}

BigInteger BigInteger::operator-(BigInteger& other)
{
	/* Handles negative output by switching RHS<->LHS */
	if (*this < other) {
		BigInteger temp_result = other - *this;

		char* char_result = new char[temp_result.length + 2];
		char_result[0] = '-';
		
		for (int i = 1; i < temp_result.length + 2; i++)
			char_result[i] = temp_result.bint[i-1];

		BigInteger final_result(char_result);

		delete[] char_result;

		return final_result;
	}

	int this_idx = length - 1;
	int other_idx = other.length - 1;

	size_t new_size = (this->size > other.size) ? this->size + 1 : other.size + 1;

	string s_subtract = "";

	int result = 0;
	int carry = 0;
	int exp = 0;

	while (this_idx != -1 || other_idx != -1) {

		int this_numb = (bint[this_idx] - '0');
		int other_numb = (other.bint[other_idx] - '0');
		
		/*
			Case 1: LHS is finished
			Case 2: RHS is finished
		*/

		if (this_idx == -1)
			this_numb = 0;

		if (other_idx == -1)
			other_numb = 0;

		/*
			Case 1: Carry is set and both digit are equal
			Case 2: RIGHT digit >= LEFT digit
			Case 3: RIGHT digit < LEFT digit
		*/

		if (carry == 1 && (this_numb == other_numb)) {
			result = 9;
			carry = 1;
		}
		else if (this_numb >= other_numb) {
			exp = 0;
			result = ((this_numb - carry) + exp) - other_numb;
			carry = 0;
		}
		else if (this_numb < other_numb) {
			exp = 10;
			result = ((this_numb - carry) + exp) - other_numb;
			carry = 1;
		}

		s_subtract = (char)(result + '0') + s_subtract;

		/*
			Case 1: ONLY move RHS
			Case 2: ONLY move LHS
			Case 3: move both
		*/

		if (this_idx == -1) {
			other_idx--;
		}
		else if (other_idx == -1) {
			this_idx--;
		}
		else {
			this_idx--;
			other_idx--;
		}
	}

	//removes leading zeros
	s_subtract.erase(0, min(s_subtract.find_first_not_of('0'), s_subtract.size() - 1));

	BigInteger subtract = stringToBigInt(new_size, s_subtract);

	return subtract;
}

BigInteger BigInteger::operator*(BigInteger& other)
{
	BigInteger result(0);

	/*
		Case 1: Multiply by 1
		Case 2: Multiply by 0
		Case 3: Normal Multiply
	*/

	if (other.length == 1 && (other.bint[0] == '1')) {
		return *this;
	}
	else if (other.length == 1 && (other.bint[0] == '0')) {
		//n*0 = 0, return 0
	}
	else {

		string zeroes = "";
		string sum = "";

		for (int other_idx = other.length-1; other_idx >= 0; other_idx--) {
			int carry = 0;

			for (int this_idx = length-1; this_idx >= 0; this_idx--) {

				int x = (other.bint[other_idx] - '0') * (bint[this_idx] - '0') + carry;

				if (x > 9 && this_idx-1 != -1) {
					carry = (x / 10) % 10;
					x %= 10;
				}
				else {
					carry = 0;
				}

				sum.insert(0, to_string(x));
			}
			
			sum.insert(sum.length(), zeroes);

			BigInteger curr = stringToBigInt(sum.length()+1,sum);

			result = result + curr;

			zeroes.insert(0, "0");
			sum = "";
		}
	}

	return result;
}

BigInteger BigInteger::operator/(BigInteger& divisor)
{
	BigInteger result(0);

	/*
		Case 1: Divide by 0
		Case 2: Divide by 1
		Case 3: Normal Division
	*/

	if (divisor.length == 1 && (divisor.bint[0] == '0')) {
		cout << " cannot divide by ";
	}
	else if (*this == divisor) {
		result = BigInteger(1); // change to this
	}
	else if (*this < divisor) {
		//decimal, return 0
	}
	else {
		string result_str;

		long long int divisor_int = stoll(string(divisor.bint), nullptr, 10);

		string curr_str = "";
		BigInteger curr_bint;

		int idx = 0;

		for (; idx < length; idx++) {
			if (divisor < curr_bint)
				break;

			curr_str.push_back(bint[idx]);
			curr_bint = stringToBigInt(curr_str.length()+1, curr_str); //move this outside loop
		}

		long long int curr_int = stoll(curr_str, nullptr, 10);

		for (; idx <= length; idx++) {

			/*
				case 1: single digit
				case 2: not single digit
			*/

			int curr_res = curr_int / divisor_int;

			if (curr_res / 10 == 0) {
				result_str += curr_res + '0';
			}
			else {
				result_str += to_string(curr_res);
			}

			
			curr_int = ((curr_int % divisor_int) * 10) + bint[idx] - '0';
		}

		result = stringToBigInt(result_str.length() + 1, result_str);
	}

	return result;
}

BigInteger BigInteger::stringToBigInt(int size, string str) {
	char* char_str = new char[size];

	strcpy(char_str, str.c_str());

	BigInteger newObj(char_str);

	delete[] char_str;

	return newObj;
}

bool BigInteger::operator<(BigInteger& other) {
	bool result = true;
	
	if (length > other.length || *this == other) {
		result = false;
	}
	else if (length == other.length) {

		for (int i = 0; i < length; i++) {
			int this_digit = bint[i] - '0';
			int other_digit = other.bint[i] - '0';
			
			if (this_digit < other_digit) {
				break;
			} else if (this_digit > other_digit) {
				result = false;
				break;
			}
		}
	}
	return result;
}

bool BigInteger::operator==(BigInteger& other) {
	bool result = true;

	if (length != other.length) {
		result = false;
	}
	else {
		for (int i = 0; i < length; i++) {
			int this_digit = bint[i] - '0';
			int other_digit = other.bint[i] - '0';

			if (this_digit != other_digit) {
				result = false;
				break;
			}
		}
	}

	return result;
}