//Jairo Molina
#include "Matrix.cpp"

template<class T>
Matrix<T> Multiply(Matrix<T> a, Matrix<T> b) {
	size_t a_row = a.getHigh() - a.getLow() + 1;
	size_t a_col = a.getSAhigh() - a.getSAlow() + 1;
	size_t b_row = b.getHigh() - b.getLow() + 1;
	size_t b_col = b.getSAhigh() - b.getSAlow() + 1;
	
	if (a_col != b_row) {
		cout << "Matrix A column != Matrix B column" << endl;
		exit(1);
	}

	Matrix<T> r(a_row, b_col);

	for (int i = 0; i < a_row; i++) {
		for (int j = 0; j < b_col; j++) {
			int tmp = 0;
			for (int z = 0; z < b_row; z++) {
				if (a.getSAlow() == 0 && b.getSAlow() == 0) {                                   // m used Matrix(x,y) and m_b used Matrix(x,y)
					tmp += a[i][z] * b[z][j];
				}
				else if (a.getSAlow() > 0 && b.getSAlow() == 0) {                               // m used Matrix(x1,y1,x2,y2) and m_b used Matrix(x,y)
					tmp += a[i][z + a.getSAlow()] * b[z][j];
				}
				else if (a.getSAlow() == 0 && b.getSAlow() > 0) {                               // m used Matrix(x,y) and m_b used Matrix(x1,y1,x2,y2)
					tmp += a[i][z] * b[z][j + b.getSAlow()];
				}
				else {                                                                    // m and m_b used Matrix(x1,y1,x2,y2)
					tmp += a[i + a.getLow()][z + a.getSAlow()] * b[z + b.getLow()][j + b.getSAlow()];
				}
			}
			r[i][j] = tmp;
		}
	}
	return r;
}

int main() {
	/******************************
	             2 9 0    
	3 2 1 5   x  1 3 5 =  50 42 42
	9 1 3 0      2 4 7    25 96 26
				 8 1 5
	*********************************/
	Matrix<int> m(4, 5, 4, 7);
	m[4][4] = 3; m[4][5] = 2; m[4][6] = 1; m[4][7] = 5;
	m[5][4] = 9; m[5][5] = 1; m[5][6] = 3; m[5][7] = 0;

	Matrix<int> m2(4, 7, 3, 5);
	m2[4][3] = 2; m2[4][4] = 9; m2[4][5] = 0;
	m2[5][3] = 1; m2[5][4] = 3; m2[5][5] = 5;
	m2[6][3] = 2; m2[6][4] = 4; m2[6][5] = 7;
	m2[7][3] = 8; m2[7][4] = 1; m2[7][5] = 5;

	Matrix<int> m3 = Multiply(m, m2);
	//Matrix<int> m4 = m * m2;
	cout << m3;
	return 0;
}