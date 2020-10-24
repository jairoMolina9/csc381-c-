#include "polynomial_list.h"

/* Default PolynomialList Constructor */
PolynomialList::PolynomialList() {
    head = nullptr;
    tail = nullptr;
    length = 0;
}

/* Destructor */
PolynomialList::~PolynomialList() {
    /*To successfuly erase a PolynomialList, all the nodes must be deallocated*/
    removeAll();
    cout << "Destructor called, PolynomialList deleted" << endl; // useful for debugging
}

/* This is a copy constructor that receives an object as parameter */
PolynomialList::PolynomialList(const PolynomialList& other) {
    copyAll(other);
}

/* Overloaded assignment operator, similar to copy constructor */
PolynomialList& PolynomialList::operator=(const PolynomialList& other) {
    if (head == other.head) { // some clients may try to assign two equal objects
        cerr << "They are the same" << endl;
    }
    else {
        /*Main difference between the copy constructor and the
          operator= function is that in this case we must deallocate
          any existing Nodes before copying all
        */
        removeAll();
        copyAll(other);
    }
    return *this; // returns the left hand object
}

/* Performs deep copy between two objects */
void PolynomialList::copyAll(const PolynomialList& other) {
    if (other.get_length() == 0) {
        head = tail = nullptr;
        length = 0;
    }
    else {
        length = other.length; // no need to increment length
        head = new Node;
        head->val = other.head->val;
        head->exp = other.head->exp;
        head->back = nullptr;
        head->next = nullptr;
        /*The use of two pointers is for efficiency*/
        Node* other_walker = other.head->next;
        Node* this_walker = head;
        while (other_walker != nullptr) {
            this_walker->next = new Node;
            this_walker->next->val = other_walker->val;
            this_walker->next->exp = other_walker->exp;
            this_walker->next->next = nullptr;
            this_walker->next->back = this_walker;

            this_walker = this_walker->next;
            other_walker = other_walker->next;
        }
        tail = this_walker; // let tail pointer be the last node
    }
}

void PolynomialList::insert_ascending(int val, int exp) {
    if (isEmpty()) { // if is empty create head node
        insert_front(val, exp);
    }
    else {
        if (head->exp <= exp) {
            if (head->exp == exp) { //likely term
                head->val = head->val + val;
                if (head->val == 0) {
                    delete_item(0);
                }
            }
            else { 
                insert_front(val, exp);
            }
        }
        else if (tail->exp >= exp) {
            if (tail->exp == exp) { //likely term
                tail->val = tail->val + val;
                if (tail->val == 0) {
                    delete_item(0);
                }
            }
            else {
                insert_back(val, exp);
            }
        }
        else {
            Node* wlkr = head->next;
            for (; wlkr != nullptr; wlkr = wlkr->next) {
                if (wlkr->exp < exp)
                    break;
            }
            if (wlkr != nullptr) {
                if (wlkr->back->exp == exp) { // likely term
                    wlkr->back->val = wlkr->back->val + val;
                    if (wlkr->back->val == 0) {
                        delete_item(0);
                    }
                }
                else {
                    Node* new_Node = new Node;
                    new_Node->val = val;
                    new_Node->exp = exp;

                    new_Node->next = wlkr;
                    wlkr->back->next = new_Node;
                    new_Node->back = wlkr->back;
                    wlkr->back = new_Node;
                }
            }
        }
    }
}

/* Inserts val into the front of PolynomialList */
void PolynomialList::insert_front(int val, int exp) {
    if (isEmpty()) { // if is empty create head node
        head = new Node;
        head->back = nullptr;
        head->next = nullptr;
        head->val = val;
        head->exp = exp;
        tail = head; // only one node thus tail and head point at same place
        length++;    // increase length by one
    }
    else {
        Node* new_Node = new Node;
        new_Node->val = val;
        new_Node->exp = exp;
        new_Node->back = nullptr;
        new_Node->next = head;
        head->back = new_Node;

        head = new_Node; // sets current head to new node
        length++;        // increase length by one
    }
}

/* Inserts val into the back of PolynomialList */
void PolynomialList::insert_back(int val, int exp) {
    if (isEmpty()) {
        head = new Node;
        head->back = nullptr;
        head->next = nullptr;
        head->val = val;
        head->exp = exp;
        tail = head; // only one node thus tail and head point at same place
        length++;    // increase length by one
    }
    else {
        Node* new_Node = new Node;
        new_Node->val = val;
        new_Node->exp = exp;
        new_Node->back = tail;
        new_Node->next = nullptr;
        tail->next = new_Node;

        tail = new_Node; // sets current tail to new node
        length++;        // increase length by one
    }
}

/* Removes every node front->back in a PolynomialList */
void PolynomialList::removeAll() {
    Node* walker;

    while (head != nullptr) {
        walker = head;
        head = head->next;
        delete walker;
    }
    tail = nullptr;
    length = 0;
}

/* Prints from front->back */
void PolynomialList::printFromFront() {
    if (isEmpty()) {
        cerr << "The PolynomialList is empty" << endl;
        return;
    }
    int counter = 1; // used to enumarate each node
    for (Node* ptr = head; ptr != nullptr; ptr = ptr->next) {
        cout << "NODE [" << counter++ << "] -> ";
        cout << ptr->val << "x^" << ptr->exp << endl;
    }
}

/* Checks if PolynomialList is empty */
bool PolynomialList::isEmpty() { return (length == 0); }

/* Deletes item , returns flag */
bool PolynomialList::delete_item(int item) {
    Node* dummy;    // dummy pointer used to delete val
    bool deleted = false; // flag

    if (isEmpty()) { // if the PolynomialList is empty
        cout << "PolynomialList is empty, nothing to delete" << endl;
        deleted = true;
    }
    else if (length == 1) {
        head = tail = nullptr;
        cout << "PolynomialList is empty" << endl;
        length--;
    }
    else if (head->val == item) { // if the item is found in the first node
        dummy = head;
        head = head->next;
        head->back = nullptr;
        delete dummy;

        --length;

        deleted = true;

    }
    else if (tail->val == item) { // if the item is found in the last node
        dummy = tail;
        tail = dummy->back;
        tail->next = nullptr;
        delete dummy;

        length--;

        deleted = true;
    }
    else { // traverse the PolynomialList
        Node* walker = head;
        int counter = 1; // used to know which # node was deleted
        while (walker != nullptr) {
            if (walker->val == item) { // walker pointer finds the item
                dummy = walker;
                walker->back->next = dummy->next;
                dummy->next->back = walker->back;
                delete dummy;

                length--; // decrement length

                deleted = true;
                break;
            }
            else {
                walker = walker->next;
                counter++;
            }
        }
    }
    return deleted; // returns flag
}

/* Returns the current length of a PolynomialList */
int PolynomialList::get_length() const { return length; }

string PolynomialList::GetCanonicalString() {
    string poly;
    for (Node* ptr = head; ptr != nullptr; ptr = ptr->next) {
        poly.append(to_string(ptr->val) + ' ' + to_string(ptr->exp) + ' ');
    }
    return poly;
}

string PolynomialList::multiply(PolynomialList other)
{
    PolynomialList newList;

    for (Node* ptr = this->head; ptr != nullptr; ptr = ptr->next) { //this
        for (Node* ptr_2 = other.head; ptr_2 != nullptr; ptr_2 = ptr_2->next) { //other
            newList.insert_ascending(ptr->val * ptr_2->val, ptr->exp + ptr_2->exp);
        }
    }

    return newList.GetCanonicalString();
}
