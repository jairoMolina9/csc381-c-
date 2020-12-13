#include "SafeArray.cpp"

template <class T>
class SM {
private:
	int low_, high_, sa_low_, sa_high_;
	SA<T>* matrix_;

public:

	// default constructor
	// allows for writing  Matrix a;

	SM() {
		low_ = sa_low_ = 0;
		high_ = sa_high_ = -1;
		matrix_ = NULL;
	}

	// 2 parameter constructor let us write
	// Matrix x(0,2);

	SM(unsigned int row, int col) {
		if (row <= 0 || col <= 0)
		{
			cout << "constructor error - invalid bounds" << endl;
			exit(1);
		}

		this->low_ = this->sa_low_ = 0;
		this->high_ = row - 1;
		this->sa_high_ = col - 1;

		matrix_ = new SA<T>[row];
		for (size_t r = 0; r < row; r++) {
			matrix_[r] = SA<T>(0, col - 1);
		}
	}

	// 4 parameter constructor let us write
	// Matrix x(0,2,0,2);

	SM(int low, int high, int sa_low, int sa_high) {
		if ((high - low + 1) <= 0 || (sa_high - sa_low) + 1 <= 0)
		{
			cout << "constructor error - invalid bounds";
			exit(1);
		}

		this->low_ = low;
		this->high_ = high;
		this->sa_low_ = sa_low;
		this->sa_high_ = sa_high;

		matrix_ = new SA<T>[high - low + 1];

		for (size_t row = 0; row < (high - low + 1); row++) {
			matrix_[row] = SA<T>(sa_low, sa_high);
		}
	}

	// copy constructor for pass by value
	// and intialization

	SM(const SM& m) {
		size_t size = m.high_ - m.low_ + 1;

		matrix_ = new SA<T>[size];
		for (size_t i = 0; i < size; i++)
			matrix_[i] = m.matrix_[i];

		high_ = m.high_;
		low_ = m.low_;
		sa_high_ = m.sa_high_;
		sa_low_ = m.sa_low_;
	}

	// destructor

	~SM() {
		delete[] matrix_;
		matrix_ = NULL; // avoid dangling pointer
	}

	// overloaded [] lets us write
	// Matrix x(0,2,0,2); x[0][0] = 1;

	SA<T>& operator[](int i) {
		if (i < low_ || i > high_) {
			cout << "index " << i << " out of range" << endl;
			exit(1);
		}
		return matrix_[i - low_];
	}

	// overloaded = let us assign
	// one Matrix to another

	SM& operator=(const SM& m) {
		if (this == &m) return *this;

		delete[] matrix_;
		size_t size = m.high_ - m.low_ + 1;
		matrix_ = new SA<T>[size];
		for (size_t i = 0; i < size; i++)
			matrix_[i] = m.matrix_[i];

		high_ = m.high_;
		low_ = m.low_;
		sa_high_ = m.sa_high_;
		sa_low_ = m.sa_high_;

		return *this;
	}

	// getter functions

	int getHigh() { return high_; }
	int getLow() { return low_; }
	int getSAhigh() { return sa_high_; }
	int getSAlow() { return sa_low_; }

	// overloads << so we can directly print Matrix

	friend ostream& operator<<(ostream& os, SM<T> m) {
		int size = m.high_ - m.low_ + 1;
		for (size_t i = 0; i < size; i++)
			cout << m.matrix_[i] << endl;

		return os;
	}
};