#include "SA.cpp"

int main()
{
    SA<int> test(25);

    cout << "-- Original -- \n";

    for (int i = 0; i < 25; i++) {
        test[i] = rand() % 99;
        cout << test[i] << " ";
    }
        
    std::sort(test.begin(), test.end());

    cout << "\n-- Sorted -- \n" << test << endl;

    cout << "\nbegin: " << *(test.begin()) << "\n" << "end: " << *(test.end()) << endl;

    string output;
    int look_for = 34;
    auto result = std::find(test.begin(), test.end(), look_for);
    
    (result != test.end()) ? output = "found" : output = "not found";
    
    cout << look_for << " was " << output << endl;

    return 0;
}