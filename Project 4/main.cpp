#include "BigInteger.cpp"

/*
	This is the optimal size needed to compute
	the arithmetic computations in main()
	
	You may change it, IF needed.
*/
#define POOL_SIZE 30000
#define EPSILON 1500

struct head { //size 32 bytes
  head * llink, * rlink;
  int tag;
  size_t size;
};

struct tail { // size 24 bytes
  head * uplink;
  int tag;
  size_t size;

};

void * pool; //pointer to beginning of memory pool

head * av; //pointer to free list

void InitPool() {

  /*
  	Initializes the pool with 3 free blocks

  	-> 1st block of size = 0
  	-> 2nd block of size = pool
  	-> 3rd block of size = 0

  	Note: pool size will autoatically decreased due to
  	the head/tail structs required for each block (line 74 and line 135)
  */

  //36 head struct + 24 tail struct + at least 1 character
  if (POOL_SIZE < 57) {
    cout << "Unable to initialize pool, not enough space" << endl;
    return;
  }

  pool = malloc(POOL_SIZE);
  if (pool) {

    /* first block head */
    head * first_h_block = (head * )((char * ) pool);
    first_h_block -> llink = (head * )((char * ) pool + ((POOL_SIZE - sizeof(tail)) - sizeof(head)));
    first_h_block -> rlink = (head * )((char * ) pool + (sizeof(head) + sizeof(tail)));
    first_h_block -> tag = 0;
    first_h_block -> size = 0;

    /* first block tail */
    tail * first_t_block = (tail * )((char * ) pool + sizeof(head));
    first_t_block -> tag = 0;
    first_t_block -> size = 0;
    first_t_block -> uplink = first_h_block;

    /* av free list starts at the first block size 0 */
    av = first_h_block;

    /* middle block head */
    head * mid_h_block = first_h_block -> rlink;
    mid_h_block -> llink = av;
    mid_h_block -> rlink = (head * )((char * ) pool + (POOL_SIZE - (sizeof(head) + sizeof(tail))));
    mid_h_block -> tag = 0;
    mid_h_block -> size = POOL_SIZE - ((3 * sizeof(tail)) + (3 * sizeof(head)));

    /* middle block tail */
    tail * mid_t_block = (tail * )((char * ) mid_h_block -> rlink - sizeof(tail));
    mid_t_block -> tag = 0;
    mid_t_block -> size = mid_h_block -> size;
    mid_t_block -> uplink = mid_h_block;

    /* last block head */
    head * last_h_block = (head * )((char * ) pool + ((POOL_SIZE - sizeof(tail)) - sizeof(head)));
    last_h_block -> llink = mid_h_block;
    last_h_block -> rlink = av;
    last_h_block -> tag = 0;
    last_h_block -> size = 0;

    /* last block tail */
    tail * last_t_block = (tail * )((char * ) pool + (POOL_SIZE - sizeof(tail)));
    last_t_block -> tag = 0;
    last_t_block -> size = 0;
    last_t_block -> uplink = last_h_block;

    cout << "Beg Pool" << "\tEnd Pool" << endl;
    cout << (long) pool << "\t" << (long) pool + POOL_SIZE << endl;
    cout << endl;
  }
}

void * operator new(size_t size) {
  /*
  	case 1: Provide entire block
  	case 2: Split block

  	Note: pool size will automatically decreased due to
  	the head/tail structs required for each block (line 135)

  	No comments because it is almost identical to the pseudocode. 
  */
  head * ptr = av -> rlink;

  do {
    if (ptr -> size >= size) {

      int diff = ptr -> size - size;

      if (diff < EPSILON) { // case 1

        ptr -> tag = 1;
        tail * curr_tail = (tail * )((char * ) ptr + ptr -> size + sizeof(head));
        curr_tail -> tag = 1;

        ptr -> llink -> rlink = ptr -> rlink;
        ptr -> rlink -> llink = ptr -> llink;

        av = ptr -> llink;

        ptr = (head * )((char * ) ptr + (sizeof(head) + 1));

        return (void * ) ptr;
      } else { // case 2

        diff = diff - (sizeof(head) + sizeof(tail));
        ptr -> size = diff;

        tail * new_tail = (tail * )((char * ) ptr + diff + sizeof(head));
        new_tail -> tag = 0;
        new_tail -> uplink = ptr;
        new_tail -> size = size;

        av = ptr;

        ptr = (head * )((char * ) new_tail + sizeof(tail));
        ptr -> tag = 1;
        ptr -> size = size;

        tail * old_tail = (tail * )((char * ) ptr + ptr -> size + sizeof(head));
        old_tail -> tag = 1;
        old_tail -> size = size;

        ptr = (head * )((char * ) ptr + sizeof(head) + 1); //pointer to beginning of free space in block

        return ptr;
      }
    }

    ptr = ptr -> rlink;

  } while (ptr != av -> rlink);

  cout << "could not allocate memory, update POOL_SIZE" << endl;
  return (void * ) 0;
}
void operator delete(void * p) throw () {
  head * p_head = (head * )((char * ) p - sizeof(head) - 1);
  tail * top_tail = (tail * )((char * ) p - sizeof(tail) - sizeof(head) - 1);

  int n = p_head -> size;

  head * bottom_head = (head * )((char * ) p + (n + sizeof(tail)) - 1);
  tail * p_tail = (tail * )((char * ) bottom_head - sizeof(tail));

  /*
  	case 1: [allocated][ p ][allocated]
  	case 2: [   free  ][ p ][allocated]
  	case 3: [allocated][ p ][   free  ]
  	case 4: [   free  ][ p ][   free  ]

  	Note: When deleting a block, we receive back the memory
  	taken by the head/tail struct of a block, thus increasing our
  	overall pool size (line 203, 218, 234)

  	No comments because it is almost identical to the pseudocode. 
  */

  if (top_tail -> tag == 1 && bottom_head -> tag == 1) {

    p_head -> tag = 0;
    p_tail -> tag = 0;

    p_tail -> uplink = p_head;
    p_head -> llink = av;
    p_head -> rlink = av -> rlink;
    p_head -> rlink -> llink = p_head;
    av -> rlink = p_head;
  } else if (bottom_head -> tag == 1 && top_tail -> tag == 0) {

    head * q = top_tail -> uplink;
    q -> size += n + sizeof(tail) + sizeof(head);

    p_tail -> uplink = q;
    p_tail -> tag = 0;
    p_tail -> size = q -> size;

    top_tail -> size = q -> size;
  } else if (bottom_head -> tag == 0 && top_tail -> tag == 1) {

    bottom_head -> llink -> rlink = p_head;
    bottom_head -> rlink -> llink = p_head;
    p_head -> llink = bottom_head -> llink;
    p_head -> rlink = bottom_head -> rlink;

    p_head -> size = n + bottom_head -> size + sizeof(tail) + sizeof(head);

    tail * bottom_tail = (tail * )((char * ) bottom_head + bottom_head -> size + sizeof(head));
    bottom_tail -> uplink = p_head;
    bottom_tail -> size = p_head -> size;
    p_head -> tag = 0;

    if (av == bottom_head)
      av = p_head;
  } else if (bottom_head -> tag == 0 && top_tail -> tag == 0) {

    bottom_head -> rlink -> llink = bottom_head -> llink;
    bottom_head -> llink -> rlink = bottom_head -> rlink;

    head * q = top_tail -> uplink;
    q -> size += n + bottom_head -> size + sizeof(tail) + sizeof(tail) + sizeof(head) + sizeof(head);

    tail * bottom_tail = (tail * )((char * ) bottom_head + bottom_head -> size + sizeof(head));
    bottom_tail -> uplink = q;

    if (av == bottom_head)
      av = p_head;
  }

}

int main() {

  InitPool();

  /* 
  Test 1
  constructing and outputting with default constructor
  */
  cout << endl << "-----TEST 1 : default constructor output -----" << endl;

  cout << "Default constructed BigInteger a is:" << endl;
  BigInteger a;
  cout << a << endl;

  /* 
  Test 2
  constructing and outputting with char input constructor
  */
  cout << endl << "-----TEST 2 :  c_string input constructor output -----" << endl;

  char * charb = new char[10];
  charb[0] = '1';
  charb[1] = '2';
  charb[2] = '3';
  charb[3] = '4';
  charb[4] = '5';
  charb[5] = '6';
  charb[6] = '7';
  charb[7] = '8';
  charb[8] = '9';
  charb[9] = '\0';

  cout << "The char array charb being input into the constructor is " << endl << charb << endl;
  cout << "Now constructing and outputting the BigInteger b with charb as input" << endl;
  BigInteger b(charb);
  cout << b << endl << endl;

  delete[] charb;

  /* 
  Test 3
  constructing and outputting with integer input constructor
  */
  cout << endl << "-----TEST 3 :  int input constructor output -----" << endl;

  int intc = 1234;
  cout << "The int intc being input into the constructor is " << endl << intc << endl;
  cout << "Now constructing and outputting the BigInteger c with intc as input:" << endl;
  BigInteger c(intc);
  cout << c << endl << endl;

  /* 
  Test 4
  constructing and outputting with copy constructor
  NOTE: if the int constructor does not work in your project
  you may change the constrictor for BigInteger d to have char* 
  or string input. in other words, you can change int d to a
  char* chard with the characters contained being the digits of int d
  */
  cout << endl << "-----TEST 4 :  copy constructor output -----" << endl;

  int intd = 1234;
  BigInteger d(intd);
  cout << "The BigInteger d being input into the constructor is " << endl << d << endl;
  cout << "Now constructing and outputting the BigInteger e with BigInteger d as input:" << endl;
  BigInteger e(d);
  cout << e << endl << endl;

  /*
  Test 5
  Testing char* and int constructor with 0 input
  */
  cout << endl << "-----TEST 5 :  constructing 0 -----" << endl;

  cout << "Outputting result of constructing BigInteger with 0 int and 0 c_string input(should be 0 twice):" << endl;
  int intf = 0;
  char * charg = new char[2];
  charg[0] = '0';
  charg[1] = '\0';
  BigInteger f(intf);
  BigInteger g(charg);
  cout << f << endl << g << endl << endl;

  delete[] charg;

  /*
  Test 6
  Testing the assignment operator
  */
  cout << endl << "-----TEST 6 :  assignment operator -----" << endl;

  BigInteger h(4321);
  BigInteger k(1234);
  BigInteger l(5678);
  cout << "h is:" << endl << h << endl << "k is:" << endl << k << endl << "l is:" << endl << l << endl;
  cout << "after doing h = k; k = l; and the values of h, k and l are:" << endl;
  h = k;
  k = l;
  cout << "h is:" << endl << h << endl << "k is:" << endl << k << endl << "l is:" << endl << l << endl;

  /*
  Test 7
  Testing addition
  */
  cout << endl << "-----TEST 7 :  + operator -----" << endl;

  cout << "Same order with overflow:" << endl;
  int intm = 87654;
  char * charn = new char[6];
  charn[0] = '5';
  charn[1] = '4';
  charn[2] = '3';
  charn[3] = '2';
  charn[4] = '1';
  charn[5] = '\0';
  BigInteger m(intm);
  BigInteger n(charn);
  cout << m << " + " << n << " = " << m + n << endl << endl;

  cout << "Large numbers" << endl;
  char * charp = new char[13];
  charp[0] = '1';
  charp[1] = '2';
  charp[2] = '3';
  charp[3] = '4';
  charp[4] = '5';
  charp[5] = '6';
  charp[6] = '7';
  charp[7] = '8';
  charp[8] = '9';
  charp[9] = '0';
  charp[10] = '1';
  charp[11] = '2';
  charp[12] = '\0';
  char * charq = new char[14];
  charq[0] = '1';
  charq[1] = '2';
  charq[2] = '3';
  charq[3] = '4';
  charq[4] = '5';
  charq[5] = '6';
  charq[6] = '7';
  charq[7] = '8';
  charq[8] = '9';
  charq[9] = '0';
  charq[10] = '1';
  charq[11] = '2';
  charq[12] = '3';
  charq[13] = '\0';
  BigInteger p(charp);
  BigInteger q(charq);
  cout << p << " + " << q << " = " << p + q << endl << endl;

  delete[] charn;
  delete[] charp;
  delete[] charq;

  /*
  Test 8
  Testing subtraction
  */
  cout << endl << "-----TEST 8 :  - operator -----" << endl;

  cout << "No change in order of magnitude:" << endl;
  int intr = 876543;
  char * chars = new char[6];
  chars[0] = '5';
  chars[1] = '4';
  chars[2] = '3';
  chars[3] = '2';
  chars[4] = '1';
  chars[5] = '\0';
  BigInteger r(intr);
  BigInteger s(chars);
  cout << r << " - " << s << " = " << r - s << endl << endl;

  cout << "Large numbers:" << endl;
  char * chart = new char[15];
  chart[0] = '1';
  chart[1] = '2';
  chart[2] = '3';
  chart[3] = '4';
  chart[4] = '5';
  chart[5] = '6';
  chart[6] = '7';
  chart[7] = '8';
  chart[8] = '9';
  chart[9] = '0';
  chart[10] = '1';
  chart[11] = '2';
  chart[12] = '3';
  chart[13] = '4';
  chart[14] = '\0';
  char * charu = new char[14];
  charu[0] = '1';
  charu[1] = '2';
  charu[2] = '3';
  charu[3] = '4';
  charu[4] = '5';
  charu[5] = '6';
  charu[6] = '7';
  charu[7] = '8';
  charu[8] = '9';
  charu[9] = '0';
  charu[10] = '1';
  charu[11] = '2';
  charu[12] = '3';
  charu[13] = '\0';
  BigInteger t(chart);
  BigInteger u(charu);
  cout << t << " - " << q << " = " << t - q << endl << endl;

  cout << "Cancellation of Order:" << endl;
  int intv = 1000010;
  int intw = 1000000;
  BigInteger v(intv);
  BigInteger w(intw);
  cout << v << " - " << w << " = " << v - w << endl << endl;

  cout << "Different order operands cancellation of Order" << endl;
  int intx = 100000;
  int inty = 99;
  BigInteger x(intx);
  BigInteger y(inty);
  cout << x << " - " << y << " = " << x - y << endl << endl;

  delete[] chars;
  delete[] chart;
  delete[] charu;

  /*
  Test 9
  Testing multiplication
  */
  cout << endl << "-----TEST 9 :  * operator -----" << endl;

  cout << "Normal ints:" << endl;
  int intaa = 87654;
  char * charz = new char[6];
  charz[0] = '5';
  charz[1] = '4';
  charz[2] = '3';
  charz[3] = '2';
  charz[4] = '1';
  charz[5] = '\0';
  BigInteger aa(intaa);
  BigInteger z(charz);
  cout << aa << " * " << z << " = " << aa * z << endl << endl;

  cout << "Large numbers" << endl;
  char * charbb = new char[13];
  charbb[0] = '1';
  charbb[1] = '2';
  charbb[2] = '3';
  charbb[3] = '4';
  charbb[4] = '5';
  charbb[5] = '6';
  charbb[6] = '7';
  charbb[7] = '8';
  charbb[8] = '9';
  charbb[9] = '0';
  charbb[10] = '1';
  charbb[11] = '2';
  charbb[12] = '\0';
  char * charcc = new char[14];
  charcc[0] = '1';
  charcc[1] = '2';
  charcc[2] = '3';
  charcc[3] = '4';
  charcc[4] = '5';
  charcc[5] = '6';
  charcc[6] = '7';
  charcc[7] = '8';
  charcc[8] = '9';
  charcc[9] = '0';
  charcc[10] = '1';
  charcc[11] = '2';
  charcc[12] = '3';
  charcc[13] = '\0';
  BigInteger bb(charbb);
  BigInteger cc(charcc);
  cout << bb << " * " << cc << " = " << bb * cc << endl << endl;

  delete[] charz;
  delete[] charbb;
  delete[] charcc;

  /*
  Test 10
  Testing Zero Cases
  */
  cout << endl << "-----TEST 10 : 0 cases -----" << endl;

  BigInteger dd(0);
  BigInteger ee(12345);
  cout << ee << " + " << dd << " = " << ee + dd << endl << endl;
  cout << ee << " - " << dd << " = " << ee - dd << endl << endl;
  cout << ee << " * " << dd << " = " << ee * dd << endl << endl;
  cout << dd << " + " << dd << " = " << dd + dd << endl << endl;
  cout << ee << " - " << ee << " = " << ee - ee << endl << endl;
  cout << dd << " * " << dd << " = " << dd * dd << endl << endl;

  /*
  Test 11
  HUGE NUMBERS
  */
  cout << endl << "-----TEST 11 : HUGE NUMBERS -----" << endl;

  cout << "HUGE numbers" << endl;
  char * charff = new char[24];
  charff[0] = '1';
  charff[1] = '2';
  charff[2] = '3';
  charff[3] = '4';
  charff[4] = '5';
  charff[5] = '6';
  charff[6] = '7';
  charff[7] = '8';
  charff[8] = '9';
  charff[9] = '0';
  charff[10] = '1';
  charff[11] = '2';
  charff[12] = '3';
  charff[13] = '4';
  charff[14] = '5';
  charff[15] = '6';
  charff[16] = '7';
  charff[17] = '8';
  charff[18] = '9';
  charff[19] = '0';
  charff[20] = '1';
  charff[21] = '2';
  charff[22] = '3';
  charff[23] = '\0';
  char * chargg = new char[23];
  chargg[0] = '1';
  chargg[1] = '2';
  chargg[2] = '3';
  chargg[3] = '4';
  chargg[4] = '5';
  chargg[5] = '6';
  chargg[6] = '7';
  chargg[7] = '8';
  chargg[8] = '9';
  chargg[9] = '0';
  chargg[10] = '1';
  chargg[11] = '2';
  chargg[12] = '3';
  chargg[13] = '4';
  chargg[14] = '5';
  chargg[15] = '6';
  chargg[16] = '7';
  chargg[17] = '8';
  chargg[18] = '9';
  chargg[19] = '0';
  chargg[20] = '1';
  chargg[21] = '2';
  chargg[22] = '\0';

  BigInteger ff(charff);
  BigInteger gg(chargg);
  cout << ff << " + " << gg << " = " << ff + gg << endl << endl;
  cout << ff << " - " << gg << " = " << ff - gg << endl << endl;
  cout << ff << " * " << gg << " = " << ff * gg << endl << endl;

  delete[] charff;
  delete[] chargg;

  /*
  FROM THIS POINT ON ALL TESTS ARE PARTICULAR TO BigInteger PART 2
  */

  /*
  Test 12
  Division
  */

  cout << endl << "-----TEST 12 :  / operator -----" << endl;

  cout << "Normal ints:" << endl;
  int inthh = 8765432;
  char * charkk = new char[6];
  charkk[0] = '5';
  charkk[1] = '4';
  charkk[2] = '3';
  charkk[3] = '2';
  charkk[4] = '1';
  charkk[5] = '\0';
  BigInteger hh(inthh);
  BigInteger kk(charkk);
  cout << hh << " / " << kk << " = " << hh / kk << endl << endl;

  cout << "Large ints" << endl;
  char * charll = new char[12];
  charll[0] = '1';
  charll[1] = '2';
  charll[2] = '3';
  charll[3] = '4';
  charll[4] = '5';
  charll[5] = '6';
  charll[6] = '7';
  charll[7] = '8';
  charll[8] = '9';
  charll[9] = '0';
  charll[10] = '1';
  charll[11] = '\0';
  char * charmm = new char[14];
  charmm[0] = '1';
  charmm[1] = '2';
  charmm[2] = '3';
  charmm[3] = '4';
  charmm[4] = '5';
  charmm[5] = '6';
  charmm[6] = '7';
  charmm[7] = '8';
  charmm[8] = '9';
  charmm[9] = '0';
  charmm[10] = '1';
  charmm[11] = '2';
  charmm[12] = '3';
  charmm[13] = '\0';
  BigInteger ll(charll);
  BigInteger mm(charmm);
  cout << mm << " / " << ll << " = " << mm / ll << endl << endl;
  cout << "Smaller numerator than Denominator(should be zero)" << endl;
  cout << ll << " / " << mm << " = " << ll / mm << endl << endl;

  cout << "Small ints" << endl;
  BigInteger nn(9);
  BigInteger oo(3);
  BigInteger qq(1);
  cout << nn << " / " << oo << " = " << nn / oo << endl << endl;
  cout << nn << " / " << qq << " = " << nn / qq << endl << endl;

  delete[] charkk;
  delete[] charll;
  delete[] charmm;

  /*
  Test 13
  CHECKING FOR SEGMENTATION by repeatedly creating and 
  deleting BigIntegers using their automatic deletion
  when they go out of scope. This just makes sure every
  time you create and delete a BigInteger you aren't permanently
  segmenting the pool of memory you created. If you aren't returning
  blocks properly using the segmentation method
  */

  cout << endl << "----TEST 13 : CHECKING FOR SEGMENTATION and Destructor ----" << endl;

  cout << "Now Creating and destroying BigIntegers on loop" << endl;
  for (int i = 100; i < 1000; i++) {
    BigInteger ww;
    BigInteger xx(i);
    BigInteger yy(i * 10);
    BigInteger zz(i * 100);
    xx = yy;
    xx = xx + yy;
    zz = zz * yy;
  }
  cout << "If you got here it probably worked." << endl;

  /*
  Test 14
  Division by Zero
  This shouldn't work,
  it should print an error message
  */

  cout << endl << "----TEST 14 : Dividing by zero ----" << endl;

  BigInteger rr(99);
  BigInteger tt(0);
  cout << "The next statement may not print because it's division by zero:" << endl;
  cout << rr << " / " << tt << " = " << rr / tt << endl << endl;

  free(pool);
  return 0;
}