#include "SM.cpp"

template<class T>
SM<T> Multiply(SM<T> a, SM<T> b) {
	size_t a_row = a.getHigh() - a.getLow() + 1;
	size_t a_col = a.getSAhigh() - a.getSAlow() + 1;
	size_t b_row = b.getHigh() - b.getLow() + 1;
	size_t b_col = b.getSAhigh() - b.getSAlow() + 1;
	
	if (a_col != b_row) {
		cout << "Matrix A column != Matrix B column" << endl;
		exit(1);
	}

	SM<T> r(a_row, b_col);

	for (int i = 0; i < a_row; i++) {
		for (int j = 0; j < b_col; j++) {
			int tmp = 0;
			for (int z = 0; z < b_row; z++) {
				if (a.getSAlow() == 0 && b.getSAlow() == 0) {                                   // a used Matrix(x,y) and b used Matrix(x,y)
					tmp += a[i][z] * b[z][j];
				}
				else if (a.getSAlow() > 0 && b.getSAlow() == 0) {                               // a used Matrix(x1,y1,x2,y2) and b used Matrix(x,y)
					tmp += a[i + a.getLow()][z + a.getSAlow()] * b[z][j];
				}
				else if (a.getSAlow() == 0 && b.getSAlow() > 0) {                               // a used Matrix(x,y) and b used Matrix(x1,y1,x2,y2)
					tmp += a[i][z] * b[z + b.getLow()][j + b.getSAlow()];
				}
				else {																			// a and b used Matrix(x1,y1,x2,y2)
					tmp += a[i + a.getLow()][z + a.getSAlow()] * b[z + b.getLow()][j + b.getSAlow()];
				}
			}
			r[i][j] = tmp;
		}
	}
	return r;
}

int main() {


    //NOTE:
    //m is an arbitrary letter used to signify a safe matrix SM
    //mrlow = lowest indexed row of matrix m
    //mrhigh = highest indexed row of matrix m
    //mclow = lowest indexed column of matrix m
    //mchigh = highest indexed column of matrix m
    //so mrhigh - mrlow + 1 is the NUMBER OF ROWS in matrix m
    //while the LENGTH of a row would actually be mchigh - mclow + 1
    //These are generally used when the matrix is constructed with 4 inputs

    //mnumr is the number of rows in a matrix main
    //mnumc is the number of columns in a matric m
    //These are generally used when the matrix is constructed with 2 inputs



    /*
    Test 1
    working 4-input constructor and multiplication
    */
    cout << endl << "------TEST 1 : 4 input constructor------" << endl;
    //5 x 4
    int arlow = 1;
    int arhigh = 5;
    int aclow = 6;
    int achigh = 9;
    SM<int> a(arlow, arhigh, aclow, achigh);
    cout << "Matrix a:" << endl;
    for (int ra = arlow; ra <= arhigh; ra++) {
        for (int ca = aclow; ca <= achigh; ca++) {
            a[ra][ca] = ra * ca;
            cout << a[ra][ca] << " ";
        }
        cout << endl;
    }
    cout << endl;

    //4 x 2
    int brlow = 3;
    int brhigh = 6;
    int bclow = 7;
    int bchigh = 8;
    SM<int> b(brlow, brhigh, bclow, bchigh);
    cout << "Matrix b:" << endl;
    for (int rb = brlow; rb <= brhigh; rb++) {
        for (int cb = bclow; cb <= bchigh; cb++) {
            b[rb][cb] = rb * cb + 1;
            cout << b[rb][cb] << " ";
        }
        cout << endl;
    }
    cout << endl;


    //product of 2 matrices created by 4 input constructor
    SM<int> c(arlow, arhigh, bclow, bchigh);
    int termCompounderz = 0;
    cout << "Matrix c, the product of Matrix a and Matrix y:" << endl;
    for (int ra = arlow; ra <= arhigh; ra++) {
        for (int cb = bclow; cb <= bchigh; cb++) {
            termCompounderz = 0;
            for (int ca = aclow; ca <= achigh; ca++) {
                termCompounderz += a[ra][ca] * b[brlow - aclow + ca][cb];
            }
            c[ra][cb] = termCompounderz;
            cout << c[ra][cb] << " ";
        }
        cout << endl;
    }


    /*
    Test 2
    working 2-input constructor and multiplication
    */

    cout << endl << "-----TEST 2 : Multiply 2 input constructor-----" << endl;

    //setting and outputting a matrix created by the 2 input constructor
    int dnumr = 2;
    int dnumc = 3;
    SM<int> d(dnumr, dnumc);
    cout << "Matrix d" << endl;
    for (int rd = 0; rd < dnumr; rd++) {
        for (int cd = 0; cd < dnumc; cd++) {
            d[rd][cd] = rd * cd + 1;
            cout << d[rd][cd] << " ";
        }
        cout << endl;
    }
    cout << endl;


    //setting and outputting a matrix created by the 2 input constructor
    int enumr = 3;
    int enumc = 5;
    SM<int> e(enumr, enumc);
    cout << "Matrix e" << endl;
    for (int re = 0; re < enumr; re++) {
        for (int ce = 0; ce < enumc; ce++) {
            e[re][ce] = re * ce;
            cout << e[re][ce] << " ";
        }
        cout << endl;
    }
    cout << endl;


    //product of 2 matrices created by 2 input constructor
    SM<int> f(dnumr, enumc);
    int termCompounderf = 0;
    cout << "Matrix f, the product of Matrix d and Matrix e:" << endl;
    for (int rd = 0; rd < dnumr; rd++) {
        for (int ce = 0; ce < enumc; ce++) {
            termCompounderf = 0;
            for (int cd = 0; cd < dnumc; cd++) {
                termCompounderf += d[rd][cd] * e[cd][ce];
            }
            f[rd][ce] = termCompounderf;
            cout << f[rd][ce] << " ";
        }
        cout << endl;
    }





    /*
    Test 3
    Assignment operator test
    tests that operator copies and does not assign by reference
    */

    cout << endl << "-----TEST 3 : Assignment operator-----" << endl;


    cout << endl << "Matrix g originally: " << endl;
    int grlow = 2;
    int grhigh = 5;
    int gclow = 6;
    int gchigh = 10;
    SM<int> g(grlow, grhigh, gclow, gchigh);
    for (int rg = grlow; rg <= grhigh; rg++) {
        for (int cg = gclow; cg <= gchigh; cg++) {
            g[rg][cg] = rg - cg + 20;
            cout << g[rg][cg] << " ";
        }
        cout << endl;
    }
    cout << endl;


    cout << endl << "Matrix h originally: " << endl;
    int hrlow = 1;
    int hrhigh = 2;
    int hclow = 3;
    int hchigh = 4;
    SM<int> h(hrlow, hrhigh, hclow, hchigh);
    for (int rh = hrlow; rh <= hrhigh; rh++) {
        for (int ch = hclow; ch <= hchigh; ch++) {
            h[rh][ch] = rh + ch;
            cout << h[rh][ch] << " ";
        }
        cout << endl;
    }
    cout << endl;
    //assign
    h = g;
    //change one number to check if distinct
    h[grhigh][gchigh] = 99;


    //now reprint matrices
    cout << endl << "Matrix g now(should be unchanged): " << endl;

    for (int rg = grlow; rg <= grhigh; rg++) {
        for (int cg = gclow; cg <= gchigh; cg++) {
            cout << g[rg][cg] << " ";
        }
        cout << endl;
    }
    cout << endl;


    cout << endl << "Matrix h now(g with bottom right element changed): " << endl;
    for (int rh = grlow; rh <= grhigh; rh++) {
        for (int ch = gclow; ch <= gchigh; ch++) {
            cout << h[rh][ch] << " ";
        }
        cout << endl;
    }
    cout << endl << "Bottom right element should be only difference.";








    /*
     Test 4
     Copy constructor test
     */

    cout << endl << "-----TEST 4 : Copy Constructor-----" << endl;

    cout << endl << "Matrix k: " << endl;
    int krlow = 3;
    int krhigh = 5;
    int kclow = 9;
    int kchigh = 10;
    SM<int> k(krlow, krhigh, kclow, kchigh);
    for (int rk = krlow; rk <= krhigh; rk++) {
        for (int ck = kclow; ck <= kchigh; ck++) {
            k[rk][ck] = rk + ck + 2;
            cout << k[rk][ck] << " ";
        }
        cout << endl;
    }
    cout << endl;


    SM<int> l(k);
    cout << endl << "Matrix l(should be same as k): " << endl;
    for (int rk = krlow; rk <= krhigh; rk++) {
        for (int ck = kclow; ck <= kchigh; ck++) {
            cout << l[rk][ck] << " ";
        }
        cout << endl;
    }
    cout << endl;



    /*
    Test 5
    Testing << operator
    */

    cout << endl << "-----TEST 5 : << operator-----" << endl;
    cout << endl << "Matrix m: " << endl;
    int mrlow = 0;
    int mrhigh = 4;
    int mclow = 8;
    int mchigh = 10;
    SM<int> m(mrlow, mrhigh, mclow, mchigh);
    for (int rm = mrlow; rm <= mrhigh; rm++) {
        for (int cm = mclow; cm <= mchigh; cm++) {
            m[rm][cm] = rm * cm + 10;
            cout << m[rm][cm] << " ";
        }
        cout << endl;
    }
    cout << endl;

    cout << "Now using cout << m to print matrix m we get:" << endl;
    cout << m << endl;


    /*
    Test 6
    Testing 4 input constructor with bad (invalid bounds) input
    */
    cout << endl << "-----TEST 6 : bad 4-input constructor input-----" << endl;
    int nrlow = 4;
    int nrhigh = 0;
    int nclow = 8;
    int nchigh = 10;
    cout << "Now to construct SM n with invalid bounds(should not work):" << endl;
    SM<int> n(nrlow, nrhigh, nclow, nchigh);


    /*
    Test 7
    Testing 2 input constructor with bad (invalid bounds) input
    */
    cout << endl << "-----TEST 7 : bad 2-input constructor input-----" << endl;
    int pnumr = -2;
    int pnumc = 4;
    cout << "Now to construct SM p with invalid bounds(should not work):" << endl;
    SM<int> p(pnumr, pnumc);


    /*
    Test 8
    Testing row bounds checking for matrix construted with 4 input constructor
    */
    cout << endl << "-----TEST 8 : Tessting row bounds  checking for 4 input matrix q-----" << endl;
    int qrlow = 2;
    int qrhigh = 4;
    int qclow = 5;
    int qchigh = 6;
    SM<int> q(qrlow, qrhigh, qclow, qchigh);
    cout << "Now we will assign a number to a place out of the row bounds of the matrix:" << endl;
    q[qrhigh + 1][qclow] = 10;


    /*
    Test 9
    Testing column bounds checking for matrix construted with 4 input constructor
    */
    cout << endl << "-----TEST 9 : Tessting column bounds  checking for 4 input matrix s-----" << endl;
    int srlow = 2;
    int srhigh = 4;
    int sclow = 5;
    int schigh = 6;
    SM<int> s(srlow, srhigh, sclow, schigh);
    cout << "Now we will assign a number to a place out of the row bounds of the matrix:" << endl;
    s[srhigh][sclow - 1] = 10;


    /*
    Test 10
    Testing row bounds checking for matrix construted with 2 input constructor
    */
    cout << endl << "-----TEST 10 : Tessting row bounds  checking for 2 input matrix t-----" << endl;
    int tnumr = 3;
    int tnumc = 4;
    SM<int> t(tnumr, tnumc);
    cout << "Now we will assign a number to a place out of the row bounds of the matrix:" << endl;
    t[tnumr][0];

    return 0;
}
