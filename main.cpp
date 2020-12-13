#include "VNT.cpp"

int main() {

    /*
    First Fill a VNT a approximately halfway
    using the a.add() function and using a sequence
    of neither ascending or descending numbers
    (i.e. some numbers should be greater than the number added
    to the VNT before them and some should be less than the number
    added to the VNT before them)
    Then print the VNT a
    then remove a number using a.getmin();
    then remove another number using a.getmin();
    then print out the VNT a again
    */

    VNT a(5, 6);
    int addedNumber = 0;
    for (int i = 0; i <= 15; i++) {
        addedNumber = (i * 31) % 50;
        cout << addedNumber << endl;
        a.add(addedNumber);
    }


    //use whatever method you want to print the VNT
    //it can be a member function or you can just loop
    //through or however else you want to print it
    //this is just an example of what it could look like
    //it doesn't have to look like this
    a.print();


    //remove 2 numbers (if you removed a different number that's also okay)
    a.getMin();
    a.getMin();
    //now print it again
    a.print();


    /*
    now use find on some numbers that are and are not in the VNT
    print out which one's you're using find on and whether find returns
    true or false
    */
    int firstSearched = 100;
    int secondSearched = 0;
    int thirdSearched = 29;

    //the following statements will print something like:
    //"100 is in VNT is: false"
    cout << firstSearched << " is in VNT is: " << a.find(firstSearched) << endl;
    cout << secondSearched << " is in VNT is: " << a.find(secondSearched) << endl;
    cout << thirdSearched << " is in VNT is: " << a.find(thirdSearched) << endl;



    /*
    Now create a 1d int array with 50 integers in neither ascending or
    descending order as explained earlier and print out the unsorted version
    */
    int arraySize = 50;
    int* k = new int[arraySize];
    cout << "The array initially is:" << endl;
    for (int i = 0; i < arraySize; i++) {
        addedNumber = (i * 31) % 50;
        k[i] = addedNumber;
        cout << addedNumber << " ";
    }
    cout << endl << endl;

    /*
    Now use the sort method from the VNT class to
    sort the array k. It doesn't have to look like how I wrote it
    you can create a VNT instance and use that or you can call the function
    statically from the VNT class
    */
    VNT test(1, arraySize);
    test.sort(k, arraySize);

    //now reprint the now sorted array k

    cout << "The array, now sorted by VNT, is:" << endl;
    for (int i = 0; i < arraySize; i++) {
        cout << k[i] << " ";
    }
    cout << endl;

    //You're done
    return 0;
}