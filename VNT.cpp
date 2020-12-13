#include "SM.cpp"

struct coord {
	int col;
	int row;
};

class VNT {
private:
	SM<int>* vnt;
	int row, col, capacity;
	coord avail_crd;

	void swap(int r_1, int c_1, int r_2, int c_2) {
		int tmp = (*vnt)[r_1][c_1];
		(*vnt)[r_1][c_1] = (*vnt)[r_2][c_2];
		(*vnt)[r_2][c_2] = tmp;
	}

public:
	VNT(int row, int col) {
		this->row = row;
		this->col = col;

		vnt = new SM<int>(row, col);

		for (int r = 0; r < row; r++) {
			for (int c = 0; c < col; c++) {
				(*vnt)[r][c] = INT32_MAX;
			}
		}
		capacity = 0;
		avail_crd.col = 0;
		avail_crd.row = 0;
	}

	void add(int numb) {
		if ((numb > INT32_MAX || capacity == col * row) && numb != INT32_MAX)
			return;

		/*
			Case 1: insert into VNT
			Case 2: VNT full
		*/

		if (capacity != row * col) {

			(*vnt)[avail_crd.row][avail_crd.col] = numb;

			int c_changed = 0;
			int c = 0;
			int r = 0;

			int prev_row = 0;
			bool check_col = false;

			if (avail_crd.row > 0) {
				prev_row = avail_crd.row - 1;
			}

			// checks if new number needs iteration

			if ((*vnt)[avail_crd.row][avail_crd.col] < (*vnt)[prev_row][avail_crd.col]) {
				check_col = true;
				swap(avail_crd.row, avail_crd.col, avail_crd.row - 1, avail_crd.col);
			}

			/*
				- iterates through col, only true if ^ is true
				- orders left to right
			*/
			if (prev_row == 0 || check_col == true) {
				while (c + 1 < col) {
					if ((*vnt)[prev_row][c] > (*vnt)[prev_row][c + 1]) {
						swap(prev_row, c, prev_row, c + 1);
						c_changed = c;
						c = -1;
					}
					c++;
				}
			}

			/*
				- iterates through row, only true if ^ is true
				- orders top bottom
			*/
			if (check_col == true) {
				while (r + 1 < row) {
					if ((*vnt)[r][c_changed] > (*vnt)[r + 1][c_changed]) {
						swap(r, c_changed, r + 1, c_changed);
						r = -1;
					}
					r++;
				}
				check_col = false;
			}

			//sets new available coordinate

			if (avail_crd.col + 1 < col) {
				avail_crd.col++;
			}
			else {
				avail_crd.row++;
				avail_crd.col = 0;
			}

			capacity++;
		}
		else {
			cout << "no more capacity" << endl;
		}
	}

	int getMin() {

		/*
			Sets 0,0 coord to highest
			possible number and reorders
			the VNT accordingly
		*/

		int val = (*vnt)[0][0];
		(*vnt)[0][0] = INT32_MAX;

		int r_changed = -1;
		// true if a number changed from one row to another
		int r_chg_bool = false;
		int c = 0;
		int c2 = 0;
		int r = 0;

		/*
			Case 1: single row
			Case 2: many rows
		*/

		if (row == 1) {
			while (c + 1 < col) {
				if ((*vnt)[0][c] > (*vnt)[0][c + 1]) {
					swap(0, c, 0, c + 1);
					c = -1;
				}
				c++;
			}
		}
		else {
			while (r + 1 < row || r_chg_bool) {
				int tmp_r = r;

				//checks one coord below from row ONLY if a row has not been currently swapped

				if ((*vnt)[r][0] > (*vnt)[r + 1][0] && !r_chg_bool) {
					swap(r, 0, r + 1, 0);
					r_changed = r;
					tmp_r = -1;
				}

				// true if a number swapped between rows

				if (r_changed != -1) {
					r_chg_bool = true;

					/*
						- iterates through column of BOTH rows that have been swapped
						- won't iterate col 2 if it is the last/unique row
					*/
					if (c + 1 < col || c2 + 1 < col) { // col 1
						if (c + 1 < col) {

							if ((*vnt)[r_changed][c] > (*vnt)[r_changed][c + 1]) {
								swap(r_changed, c, r_changed, c + 1);
								c = -1;
							}
						}

						if (c2 + 1 < col && (r_changed + 2 > row || (*vnt)[r_changed + 1][0] <= (*vnt)[r_changed + 2][0])) { // col 2

							if ((*vnt)[r_changed + 1][c2] > (*vnt)[r_changed + 1][c2 + 1]) {
								swap(r_changed + 1, c2, r_changed + 1, c2 + 1);
								c2 = -1;
							}
						}

						c++;
						c2++;
					}
					else {
						r_chg_bool = false;
						tmp_r = -1;
						r_changed = -1;
						c = 0;
						c2 = 0;
					}
				}

				if (!r_chg_bool)
					r = ++tmp_r;
			}
		}

		return val;
	}

	string find(int i) {

		int r = 0, c = r;
		bool iterate_c = false;

		if (i < (*vnt)[r][c]) {
			return "false";
		}
		else {

			while (r + 1 < row) {

				//checks one coord below from row and last coord from col
				if (i < (*vnt)[r + 1][c] || i < (*vnt)[r][col - 1])
					iterate_c = true;

				//checks if either one below or end is i
				if (i == (*vnt)[r + 1][c] || i == (*vnt)[r][col - 1])
					return "true";

				//iterates through column
				if (iterate_c) {
					if (i == (*vnt)[r][c]) {
						return "true";
					}
					else  if (c + 1 > col) {
						iterate_c = false;
						c = 0;
					}
					else if (i < (*vnt)[r][c + 1]) {
						return "false";
					}
					else if (i > (*vnt)[r][c]) {
						c++;
					}
				}

				if (!iterate_c)
					r++;
			}
		}

		return "false";
	}

	void sort(int k[], int size) {
		for (int i = 0; i < size; i++)
			add(k[i]);

		for (int i = 0; i < size; i++)
			k[i] = getMin();
	}

	SA<int>& operator[](int i) {
		return (*vnt)[i];
	}

	~VNT() {
		delete vnt;
	}

	void print() {
		cout << endl;

		for (int r = 0; r < row; r++) {
			for (int c = 0; c < col; c++) {
				cout << (*vnt)[r][c] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
};