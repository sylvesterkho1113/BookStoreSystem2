/*
---------------------------------------------------------
|                 *Section 1617 Group12*                |
|                Title: Book Store System               |
|-------------------------------------------------------|
| By ID             | Name                              |
| 1211207735        | See Chwan Kai                     |
| 1211211485        | Kho Wei Cong                      |
| 1211208688        | Tee Kian Hao                      | 
| 1211208756        | Tee Chin Yean                     |
---------------------------------------------------------
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <cstring> // strcmp
#include <cctype> // tolower
#include <stdlib.h> // Clear screen function
#include <limits> // numeric_limits to get limit value of data type


using namespace std;

/*
Files used:
1. users.txt to store customer and admin information (Role 1 = Customer | Role 2 = Admin)
2. books.txt to store books information
3. orders.txt to store orders total and status
4. orderdetails.txt to store placed items and quantity for each order
5. cart.txt to store the books that selected by users
*/

struct user {
    string username;
    string password;
    string phone;
    string address;
    int role;
    user *next; // For linked lists
};

struct book {
    int id;
    string title;
    string author;
    float cost;
    float sellprice;
    string description;
    int availablestock;
    int soldstock;
    book *next; // For linked lists
};

struct orderdetails {
    int orderdetails_id;
    int orderdetails_productid;
    int orderdetails_quantity;
    orderdetails *next; // For linked lists
};

struct order {
    int order_id;
    string order_username;
    float order_costtotal;
    float order_grandtotal;
    string order_status;
    order *next; // For linked lists
};

// Global pointers for linked lists
order *orderhead = nullptr;
orderdetails *detailshead = nullptr;
book *bookhead = nullptr;


struct cart {
    string cart_username;
    int cart_productid;
    int cart_quantity;
    cart *next; // For linked lists
};


// Class Definition
class CUST;
class ADMIN;

// Funtion Definition
void insertion_sort_by_id(book*& head);
void insertion_sort_by_title(book*& head);
void insertion_sort_by_author(book*& head);
void insertion_sort_by_orderid(order*& head);
void insertion_sort_by_status(order*& head);
void insertion_sort_by_username(order*& head);

class BOOK {
    public:
        book *head;
        // Use to auto assigned new ID when adding new book
        int bookautoid = 1000;

    public:
        // Constructor to read data from books.txt and insert into linked lists
        BOOK() : head(NULL) {
            ifstream file("books.txt");
            if (!file) {
                cerr << "Unable to open file books.txt" << endl;
                return;
            }

            string line;
            while (getline(file, line)) {
                istringstream iss(line);

                // Local variables to store current data read from txt file
                int id, availablestock, soldstock;
                string title, author, description;
                float cost, sellprice;

                // Read the fields
                if (!(iss >> id)) {
                    cerr << "Error reading id from line: " << line << endl;
                    continue;
                }

                bookautoid = max(bookautoid, id);

                // Skip whitespace and the opening quote before the title
                iss.ignore(numeric_limits<streamsize>::max(), '\"');
                // Read title until the closing quote
                if (!getline(iss, title, '\"')) {
                    cerr << "Error reading title from line: " << line << endl;
                    continue;
                }

                // Skip whitespace and the opening quote before the author
                iss.ignore(numeric_limits<streamsize>::max(), '\"');
                // Read author until the closing quote
                if (!getline(iss, author, '\"')) {
                    cerr << "Error reading author from line: " << line << endl;
                    continue;
                }

                // Read cost and sellprice directly
                if (!(iss >> cost >> sellprice)) {
                    cerr << "Error reading cost and sellprice from line: " << line << endl;
                    continue;
                }

                // Skip whitespace and the opening quote before the description
                iss.ignore(numeric_limits<streamsize>::max(), '\"');
                // Read description until the closing quote
                if (!getline(iss, description, '\"')) {
                    cerr << "Error reading description from line: " << line << endl;
                    continue;
                }

                // Read availablestock and soldstock directly
                if (!(iss >> availablestock >> soldstock)) {
                    cerr << "Error reading availablestock and soldstock from line: " << line << endl;
                    continue;
                }

                // Create a new object and link it to the previous lists
                book* new_book = new book{id, title, author, cost, sellprice, description, availablestock, soldstock, NULL};
                new_book->next = head;
                head = new_book;
            }
            file.close();
        } // End of BOOK constructor

        // Destructor to free memory
        ~BOOK() {
            book* current = head;
            while (current != NULL) {
                book* next = current->next;
                delete current;
                current = next;
            }
        }

        // Pure virtual function for display_books
        virtual void display_books() = 0;

        // Pure virtual function for search_books
        virtual void search_books() = 0;

        // Pure virtual function for sort_books
        virtual void sort_books() = 0;
}; // End of BOOK class

class CBOOK : public BOOK {
    private:

    public:
        // Overriding Function for customer to display all linked list book data
        void display_books() override {
            // Display title
            system("CLS");
            cout<<"----------------------------------------------------------------------------------------------------------"<<endl;
            cout<<"  BROWSE ALL BOOKS"<<endl;
            cout<<"----------------------------------------------------------------------------------------------------------"<<endl;
            cout << setw(10) << "Book ID#" << setw(20) << "Book Name" << setw(20) << "Author" 
                << setw(15) << "Sell Price" << setw(30) << "Description" << setw(10) << "Stock" << endl;

            // Display data
            book* current = head;
            while (current != NULL) {
                cout << setw(10) << current->id << setw(20) << current->title << setw(20) << current->author
                    << setw(15) << fixed << setprecision(2) << current->sellprice << setw(30) << current->description 
                    << setw(10) << current->availablestock << endl;
                current = current->next;
            }
        } // End of display_books function

        // Function to get the middle node of the linked list
        book* get_middle(book* start, book* end) {
            if (start == NULL) return NULL;

            book* slow = start;
            book* fast = start->next;

            while (fast != end) {
                fast = fast->next;
                if (fast != end) {
                    slow = slow->next;
                    fast = fast->next;
                }
            }

            return slow;
        }

        // Function to perform binary search by book ID
        book* binary_search_by_id(book* head, int searchid) {
            book* start = head;
            book* end = NULL;

            do {
                book* mid = get_middle(start, end);

                if (mid == NULL) return NULL;

                if (mid->id == searchid) {
                    return mid;
                } else if (mid->id < searchid) {
                    start = mid->next;
                } else {
                    end = mid;
                }
            } while (end == NULL || end != start);

            return NULL;
        }

        // Function to perform binary search by book title
        book* binary_search_by_title(book* head, const string& searchtitle) {
            book* start = head;
            book* end = NULL;

            do {
                book* mid = get_middle(start, end);

                if (mid == NULL) return NULL;

                if (mid->title == searchtitle) {
                    return mid;
                } else if (mid->title < searchtitle) {
                    start = mid->next;
                } else {
                    end = mid;
                }
            } while (end == NULL || end != start);

            return NULL;
        }

        // Function to perform binary search by author
        book* binary_search_by_author(book* head, const string& searchauthor) {
            book* start = head;
            book* end = NULL;

            do {
                book* mid = get_middle(start, end);

                if (mid == NULL) return NULL;

                if (mid->author == searchauthor) {
                    return mid;
                } else if (mid->author < searchauthor) {
                    start = mid->next;
                } else {
                    end = mid;
                }
            } while (end == NULL || end != start);

            return NULL;
        }

        // Overriding Function for customer to search book data
        void search_books() override {
            system("CLS");
            // Call display_books function
            display_books();

            int choice;

            do {
                cout<<"----------------------------------------------------------------------------------------------------------"<<endl;
                cout << "  Book Searching" << endl;
                cout<<"----------------------------------------------------------------------------------------------------------"<<endl;
                // Ask use which parameter to search
                cout << "[1] Book ID" << endl;
                cout << "[2] Book Title" << endl;
                cout << "[3] Book Author" << endl;
                cout << "[4] Exit to main menu" << endl;
                cout << "Enter your choice: ";
                cin >> choice;

                while (choice < 1 || choice > 4) {
                    cout << "Invalid input! Please enter again [1-4]: ";
                    cin >> choice;
                }

                if (choice == 1) {
                    // Search by Book ID
                    int searchid;
                    // Call sort_books function
                    insertion_sort_by_id(head);

                    // Ask to enter key
                    cout << "Enter the search key: ";
                    cin >> searchid;

                    // Perform binary search
                    book* found_book = binary_search_by_id(head, searchid);

                    if (found_book != NULL) {
                        // Book found, display details
                        system("CLS");
                        cout<<"----------------------------------------------------------------------------------------------------------"<<endl;
                        cout << "  Searching Result" << endl;
                        cout<<"----------------------------------------------------------------------------------------------------------"<<endl;
                        cout << setw(10) << "Book ID#" << setw(20) << "Book Name" << setw(20) << "Author"
                            << setw(15) << "Sell Price" << setw(30) << "Description" << setw(10) << "Stock" << endl;
                        cout << setw(10) << found_book->id << setw(20) << found_book->title << setw(20) << found_book->author
                            << setw(15) << found_book->sellprice << setw(30) << found_book->description
                            << setw(10) << found_book->availablestock << endl;
                    }
                    else {
                        system("CLS");
                        cout << "[ERROR] Book with ID " << searchid << " not found." << endl;
                    }
                } else if (choice == 2) {
                    // Search by Book Title
                    string searchtitle;
                    // Call sort_books function
                    insertion_sort_by_title(head);

                    // Ask to enter key
                    cout << "Enter the search key: ";
                    cin.ignore();
                    getline(cin, searchtitle);

                    // Perform binary search
                    book* found_book = binary_search_by_title(head, searchtitle);

                    if (found_book != NULL) {
                        // Book found, display details
                        system("CLS");
                        cout<<"----------------------------------------------------------------------------------------------------------"<<endl;
                        cout << "  Searching Result" << endl;
                        cout<<"----------------------------------------------------------------------------------------------------------"<<endl;
                        cout << setw(10) << "Book ID#" << setw(20) << "Book Name" << setw(20) << "Author"
                            << setw(15) << "Sell Price" << setw(30) << "Description" << setw(10) << "Stock" << endl;
                        cout << setw(10) << found_book->id << setw(20) << found_book->title << setw(20) << found_book->author
                            << setw(15) << found_book->sellprice << setw(30) << found_book->description
                            << setw(10) << found_book->availablestock << endl;
                    }
                    else {
                        system("CLS");
                        cout << "[ERROR] Book with ID " << searchtitle << " not found." << endl;
                    }
                } else if (choice == 3) {
                    // Search by Book Author
                    string searchauthor;
                    // Call sort_books function
                    insertion_sort_by_author(head);

                    // Ask to enter key
                    cout << "Enter the search key: ";
                    cin.ignore();
                    getline(cin, searchauthor);

                    // Perform binary search
                    book* found_book = binary_search_by_author(head, searchauthor);

                    if (found_book != NULL) {
                        // Book found, display details
                        system("CLS");
                        cout<<"----------------------------------------------------------------------------------------------------------"<<endl;
                        cout << "  Searching Result" << endl;
                        cout<<"----------------------------------------------------------------------------------------------------------"<<endl;
                        cout << setw(10) << "Book ID#" << setw(20) << "Book Name" << setw(20) << "Author"
                            << setw(15) << "Sell Price" << setw(30) << "Description" << setw(10) << "Stock" << endl;
                        cout << setw(10) << found_book->id << setw(20) << found_book->title << setw(20) << found_book->author
                            << setw(15) << found_book->sellprice << setw(30) << found_book->description
                            << setw(10) << found_book->availablestock << endl;
                    }
                    else {
                        system("CLS");
                        cout << "[ERROR] Book with ID " << searchauthor << " not found." << endl;
                    }
                } else if (choice == 4) {
                    return;
                }
            } while (choice != 4);
        } // End of search_books function

        friend void insertion_sort_by_id(book*& head);
        friend void insertion_sort_by_title(book*& head);
        friend void insertion_sort_by_author(book*& head);

        // Overriding Function for customer to sort books
        void sort_books() override {
            system("CLS");
            cout<<"---------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
            cout<<"  UNSORTED DATA"<<endl;
            // Call display books function
            display_books();

            int choice;

            do{
                // Ask use which parameter to sort
                cout<<"---------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
                cout<<"  Book Sorting"<<endl;
                cout<<"---------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
                cout<<"[1] Book ID"<<endl;
                cout<<"[2] Book Title"<<endl;
                cout<<"[3] Author"<<endl;
                cout<<"[4] Exit to main menu"<<endl;
                cout<<"Enter your searching criteria: ";
                cin>>choice;

                while(choice < 1 || choice > 4){
                    cout<<"Invalid input! Please enter again [1-4]: ";
                    cin>>choice;
                }

                if(choice == 1){
                    insertion_sort_by_id(head);
                    system("CLS");
                    cout<<"---------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
                    cout<<"  Sorted Result Based On Book ID"<<endl;
                    // Display books
                    display_books();
                }
                else if(choice == 2){
                    insertion_sort_by_title(head);
                    // Display books
                    system("CLS");
                    cout<<"---------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
                    cout<<"  Sorted Result Based On Book Title"<<endl;
                    display_books();
                }
                else if(choice == 3){
                    insertion_sort_by_author(head);
                    system("CLS");
                    cout<<"---------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
                    // Display books
                    cout<<"  Sorted Result Based On Book Author"<<endl;
                    display_books();
                }
                else if(choice == 4){
                    return;
                }
                
            }while(choice != 4);

        } // End of sort_books function
}; // End of CBOOK class

class ABOOK : public BOOK {
    private:

    public:
        // Overriding Function for admin to display all linked list book data
        void display_books() override {
            // Display title
            cout<<"---------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
            cout<<"  BROWSE ALL BOOKS"<<endl;
            cout<<"---------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
            cout << setw(10) << "Book ID#" << setw(20) << "Book Name" << setw(20) << "Author" << setw(15) << "Cost Price"
                << setw(15) << "Sell Price" << setw(30) << "Description" << setw(15) << "Current Stock" << setw(15) << "Sold Stock" << endl;

            // Display data
            book* current = head;
            while (current != NULL) {
                cout << setw(10) << current->id << setw(20) << current->title << setw(20) << current->author << setw(15) << fixed << setprecision(2) << current->cost
                    << setw(15) << fixed << setprecision(2) << current->sellprice << setw(30) << current->description 
                    << setw(15) << current->availablestock << setw(15) << current->soldstock << endl;
                current = current->next;
            }
        } // End of display_books function

        // Function to get the middle node of the linked list
        book* get_middle(book* start, book* end) {
            if (start == NULL) return NULL;

            book* slow = start;
            book* fast = start->next;

            while (fast != end) {
                fast = fast->next;
                if (fast != end) {
                    slow = slow->next;
                    fast = fast->next;
                }
            }

            return slow;
        }

        // Function to perform binary search by book ID
        book* binary_search_by_id(book* head, int searchid) {
            book* start = head;
            book* end = NULL;

            do {
                book* mid = get_middle(start, end);

                if (mid == NULL) return NULL;

                if (mid->id == searchid) {
                    return mid;
                } else if (mid->id < searchid) {
                    start = mid->next;
                } else {
                    end = mid;
                }
            } while (end == NULL || end != start);

            return NULL;
        }

        // Function to perform binary search by book title
        book* binary_search_by_title(book* head, const string& searchtitle) {
            book* start = head;
            book* end = NULL;

            do {
                book* mid = get_middle(start, end);

                if (mid == NULL) return NULL;

                if (mid->title == searchtitle) {
                    return mid;
                } else if (mid->title < searchtitle) {
                    start = mid->next;
                } else {
                    end = mid;
                }
            } while (end == NULL || end != start);

            return NULL;
        }

        // Function to perform binary search by author
        book* binary_search_by_author(book* head, const string& searchauthor) {
            book* start = head;
            book* end = NULL;

            do {
                book* mid = get_middle(start, end);

                if (mid == NULL) return NULL;

                if (mid->author == searchauthor) {
                    return mid;
                } else if (mid->author < searchauthor) {
                    start = mid->next;
                } else {
                    end = mid;
                }
            } while (end == NULL || end != start);

            return NULL;
        }

        // Overriding Function for admin to search book data
        void search_books() override {
            system("CLS");
            // Call display_books function
            display_books();

            int choice;

            do {
                cout << "---------------------------------------------------------------------------------------------------------------------------------------------" << endl;
                cout << "  Book Searching" << endl;
                cout << "---------------------------------------------------------------------------------------------------------------------------------------------" << endl;
                // Ask use which parameter to search
                cout << "[1] Book ID" << endl;
                cout << "[2] Book Title" << endl;
                cout << "[3] Book Author" << endl;
                cout << "[4] Exit to main menu" << endl;
                cout << "Enter your choice: ";
                cin >> choice;

                while (choice < 1 || choice > 4) {
                    cout << "Invalid input! Please enter again [1-4]: ";
                    cin >> choice;
                }

                if (choice == 1) {
                    // Search Key
                    int searchid;
                    // Call sort_books function
                    insertion_sort_by_id(head);

                    // Ask to enter key
                    cout << "Enter the search key: ";
                    cin >> searchid;

                    // Perform binary search
                    book* found_book = binary_search_by_id(head, searchid);

                    if (found_book != NULL) {
                        // Book found, display details
                        system("CLS");
                        cout << "---------------------------------------------------------------------------------------------------------------------------------------------" << endl;
                        cout << "  Searching Result" << endl;
                        cout << "---------------------------------------------------------------------------------------------------------------------------------------------" << endl;
                        cout << setw(10) << "Book ID#" << setw(20) << "Book Name" << setw(20) << "Author" << setw(15) << "Cost Price"
                            << setw(15) << "Sell Price" << setw(30) << "Description" << setw(15) << "Current Stock" << setw(15) << "Sold Stock" << endl;
                        cout << setw(10) << found_book->id << setw(20) << found_book->title << setw(20) << found_book->author << setw(15) << found_book->cost
                            << setw(15) << found_book->sellprice << setw(30) << found_book->description 
                            << setw(15) << found_book->availablestock << setw(15) << found_book->soldstock << endl;
                    }
                    else {
                        system("CLS");
                        cout << "[ERROR] Book with ID " << searchid << " not found." << endl;
                    }
                }
                else if (choice == 2) {
                    // Search Key
                    string searchtitle;
                    // Call sort_books function
                    insertion_sort_by_title(head);

                    // Ask to enter key
                    cout << "Enter the search key: ";
                    cin.ignore();
                    getline(cin, searchtitle);

                    // Perform binary search
                    book* found_book = binary_search_by_title(head, searchtitle);

                    if (found_book != NULL) {
                        // Book found, display details
                        system("CLS");
                        cout << "---------------------------------------------------------------------------------------------------------------------------------------------" << endl;
                        cout << "  Searching Result" << endl;
                        cout << "---------------------------------------------------------------------------------------------------------------------------------------------" << endl;
                        cout << setw(10) << "Book ID#" << setw(20) << "Book Name" << setw(20) << "Author" << setw(15) << "Cost Price"
                            << setw(15) << "Sell Price" << setw(30) << "Description" << setw(15) << "Current Stock" << setw(15) << "Sold Stock" << endl;
                        cout << setw(10) << found_book->id << setw(20) << found_book->title << setw(20) << found_book->author << setw(15) << found_book->cost
                            << setw(15) << found_book->sellprice << setw(30) << found_book->description 
                            << setw(15) << found_book->availablestock << setw(15) << found_book->soldstock << endl;
                    }
                    else {
                        system("CLS");
                        cout << "[ERROR] Book with title " << searchtitle << " not found." << endl;
                    }
                }
                else if (choice == 3) {
                    // Search Key
                    string searchauthor;
                    // Call sort_books function
                    insertion_sort_by_author(head);

                    // Ask to enter key
                    cout << "Enter the search key: ";
                    cin.ignore();
                    getline(cin, searchauthor);

                    // Perform binary search
                    book* found_book = binary_search_by_author(head, searchauthor);

                    if (found_book != NULL) {
                        // Book found, display details
                        system("CLS");
                        cout << "---------------------------------------------------------------------------------------------------------------------------------------------" << endl;
                        cout << "  Searching Result" << endl;
                        cout << "---------------------------------------------------------------------------------------------------------------------------------------------" << endl;
                        cout << setw(10) << "Book ID#" << setw(20) << "Book Name" << setw(20) << "Author" << setw(15) << "Cost Price"
                            << setw(15) << "Sell Price" << setw(30) << "Description" << setw(15) << "Current Stock" << setw(15) << "Sold Stock" << endl;
                        cout << setw(10) << found_book->id << setw(20) << found_book->title << setw(20) << found_book->author << setw(15) << found_book->cost
                            << setw(15) << found_book->sellprice << setw(30) << found_book->description 
                            << setw(15) << found_book->availablestock << setw(15) << found_book->soldstock << endl;
                    }
                    else {
                        system("CLS");
                        cout << "[ERROR] Book with author " << searchauthor << " not found." << endl;
                    }
                }
                else if (choice == 4) {
                    return;
                }
            } while (choice != 4);
        } // End of search_books function

        friend void insertion_sort_by_id(book*& head);
        friend void insertion_sort_by_title(book*& head);
        friend void insertion_sort_by_author(book*& head);

        // Overriding Function for admin to sort books
        void sort_books() override {
            system("CLS");
            cout<<"---------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
            cout<<"  UNSORTED DATA"<<endl;
            // Call display_books function
            display_books();

            int choice;

            do{
                // Ask use which parameter to sort
                cout<<"---------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
                cout<<"  Book Sorting"<<endl;
                cout<<"---------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
                cout<<"[1] Book ID"<<endl;
                cout<<"[2] Book Title"<<endl;
                cout<<"[3] Author"<<endl;
                cout<<"[4] Exit to main menu"<<endl;
                cout<<"Enter your searching criteria: ";
                cin>>choice;

                while(choice < 1 || choice > 4){
                    cout<<"Invalid input! Please enter again [1-4]: ";
                    cin>>choice;
                }

                if(choice == 1){
                    insertion_sort_by_id(head);
                    system("CLS");
                    cout<<"---------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
                    cout<<"  Sorted Result Based On Book ID"<<endl;
                    // Display books
                    display_books();
                }
                else if(choice == 2){
                    insertion_sort_by_title(head);
                    system("CLS");
                    cout<<"---------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
                    cout<<"  Sorted Result Based On Book Title"<<endl;
                    // Display books
                    display_books();
                }
                else if(choice == 3){
                    insertion_sort_by_author(head);
                    system("CLS");
                    cout<<"---------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
                    // Display books
                    cout<<"  Sorted Result Based On Book Author"<<endl;
                    // Display books
                    display_books();
                }
                else if(choice == 4){
                    return;
                }
                
            }while(choice != 4);

        } // End of sort_books function

        // Function for admin to add new books
        void add_books(){
            // Local variable to store data entered by admin
            string ttl, athr, desc; //Title, Author, Description
            int stk, sstk = 0; //Stock, Sold Stock
            float cp, sp; //Cost Price, Selling Price
            char choice;

            do{
                bookautoid += 1;

                // Open file to write into file
                ofstream addbook("books.txt", ios::app);

                // Display title
                system("CLS");
                cout<<"---------------------------------------------"<<endl;
                cout<<" ADD NEW BOOK"<<endl;
                cout<<"---------------------------------------------"<<endl;
                // Ask for book's information
                cout<<"Enter book's title       : ";
                fflush(stdin);
                getline(cin,ttl);
                cout<<"Enter book's author      : ";
                fflush(stdin);
                getline(cin,athr);
                cout<<"Enter book's cost price  : RM ";
                cin>>cp;
                cout<<"Enter book's sell price  : RM ";
                cin>>sp;
                cout<<"Enter book's description : ";
                fflush(stdin);
                getline(cin,desc);
                cout<<"Enter book's stock       : ";
                cin>>stk;

                // Write into file
                if(addbook.is_open()){
                    addbook<<(bookautoid)<<" \""<<ttl<<"\" \""<<athr<<"\" "<<cp<<" "<<sp<<" \""<<desc<<"\" "<<stk<<" "<<sstk<<endl;
                }

                // Insert into linked lists
                book* new_book = new book{bookautoid, ttl, athr, cp, sp, desc, stk, sstk, NULL};
                new_book->next = head;
                head = new_book;

                // Display successful message
                system("CLS");
                cout<<endl<<"--------------------------------"<<endl;
                cout<<"  Book added successfully!"<<endl;
                cout<<"--------------------------------"<<endl;
                cout<<"Book ID              : #"<<bookautoid<<endl;
                cout<<"Book Title           : "<<ttl<<endl;
                cout<<"Book Author          : "<<athr<<endl;
                cout<<"Book Cost Price      : RM "<<cp<<endl;
                cout<<"Book Sell Price      : RM "<<sp<<endl;
                cout<<"Book Description     : "<<desc<<endl;
                cout<<"Book Available Stock : "<<stk<<endl;

                //close file
                addbook.close();

                //ask user if they want to add another book
                cout<<endl<<"Do you want to add another book? [Y/N] : ";
                cin>>choice;

            }while(choice=='Y' || choice == 'y');
        } // End of add_books function

        // Function for admin to delete book using ID
        void delete_books() {
            char loop;

            do {
                // Display books
                display_books();

                // Display title
                cout<<"---------------------------------------------"<<endl;
                cout<<" DELETE BOOK"<<endl;
                cout<<"---------------------------------------------"<<endl;

                bool bookfound = false;

                do{
                    int id;
                    book* temp = head;
                    book* prev = NULL;

                    // Ask admin which Book ID to be deleted
                    cout << "Enter the ID you want to delete : ";
                    cin >> id;

                    while (temp != NULL) {
                        if (temp->id == id) {
                            bookfound = true;
                            // If the deleting node is head (Special Handling Case)
                            if (temp == head) {
                                head = head->next; // Move head to the next node
                                delete temp;
                                temp = head; // Reassign temp to the new head
                            } else {
                                prev->next = temp->next;
                                delete temp;
                                temp = prev->next; // Move temp to the next node after deletion
                            }
                            break;
                        }
                        prev = temp;
                        temp = temp->next;
                    }

                    if (!bookfound) {
                        cout << "[ERROR] Book ID #" << id << " not found! Press enter to try again." << endl;
                        fflush(stdin);
                        getchar();
                    }
                    else {
                        // Create a new file books.txt and insert linked list into file
                        ofstream file("books.txt");
                        if (!file) {
                            cerr << "Unable to open file books.txt for writing." << endl;
                            return;
                        }

                        temp = head;
                        while (temp != NULL) {
                            file << temp->id << " \"" << temp->title << "\" \"" << temp->author << "\" " << temp->cost << " " << temp->sellprice << " \"" << temp->description << "\" " << temp->availablestock << " " << temp->soldstock << endl;
                            temp = temp->next;
                        }

                        file.close();

                        // Successful message
                        cout << "Book ID #" << id << " deleted successfully!" << endl;
                    }

                }while(bookfound == false);

                cout << "Do you want to delete another book [Y/N]: ";
                cin >> loop;

            } while (loop == 'Y' || loop == 'y');
        } // End of admin delete_books function

        void update_books(){
            
            bool bookfound;
            char loop;
            int id;

            do{

                // Display books
                display_books();

                // Display title
                cout<<"---------------------------------------------"<<endl;
                cout<<" UPDATE BOOK'S INFORMATION"<<endl;
                cout<<"---------------------------------------------"<<endl;

                do{
                    bookfound = false;
                    loop = 'n';

                    // Ask admin which Book ID to be updated
                    cout<<"Enter the Book ID you want to update: ";
                    cin>>id;

                    book* temp = head;
                    while((temp != NULL) && (temp->id != id))
                    {
                        temp = temp->next;
                    }

                    if(temp != NULL){
                        int selection;
                        bookfound = true;

                        // Display title
                        system("CLS");
                        cout<<"-------------------------------------------------------------"<<endl;
                        cout<<" Information of BOOK ID #"<<id<<endl;
                        cout<<"-------------------------------------------------------------"<<endl;
                        cout<<"[1] Title           : "<<temp->title<<endl;
                        cout<<"[2] Author          : "<<temp->author<<endl;
                        cout<<"[3] Cost Price      : RM "<<fixed<<setprecision(2)<<temp->cost<<endl;
                        cout<<"[4] Sell Price      : RM "<<fixed<<setprecision(2)<<temp->sellprice<<endl;
                        cout<<"[5] Desciption      : "<<temp->description<<endl;
                        cout<<"[6] Available Stock : "<<temp->availablestock<<endl;
                        cout<<"** P/S: Book ID and Sold Stock cannot be changed. **"<<endl;
                        cout<<"Select the field you want to change [1-6]: ";
                        cin>>selection;

                        while(selection < 1 || selection > 6){
                            cout<<"Invalid input! Please try again [1-6]: ";
                            cin>>selection;
                        }

                        if(selection == 1){
                            cout<<"-------------------------------------------------------------"<<endl;
                            cout<<"Enter new title: ";
                            fflush(stdin);
                            getline(cin,temp->title);
                        }
                        else if(selection == 2){
                            cout<<"-------------------------------------------------------------"<<endl;
                            cout<<"Enter new author: ";
                            fflush(stdin);
                            getline(cin,temp->author);
                        }
                        else if(selection == 3){
                            cout<<"-------------------------------------------------------------"<<endl;
                            cout<<"Enter new cost price: RM ";
                            cin>>temp->cost;
                        }
                        else if(selection == 4){
                            cout<<"-------------------------------------------------------------"<<endl;
                            cout<<"Enter new selling price: RM ";
                            cin>>temp->sellprice;
                        }
                        else if(selection == 5){
                            cout<<"-------------------------------------------------------------"<<endl;
                            cout<<"Enter new description: ";
                            fflush(stdin);
                            getline(cin,temp->description);
                        }
                        else if(selection == 6){
                            cout<<"-------------------------------------------------------------"<<endl;
                            cout<<"Enter new available stock: ";
                            cin>>temp->availablestock;
                        }

                    }

                    if(bookfound == false){
                            cout<<"Book ID #"<<id<<" not found! Press enter to try again.";
                            fflush(stdin);
                            getchar();
                    }
                
                }while(bookfound == false);

                // create a new file books.txt and insert linked list into file
                ofstream file("books.txt");
                if (!file) {
                    cerr << "Unable to open file books.txt for writing." << endl;
                    return;
                }

                book* temp2 = head;
                while (temp2 != NULL) {
                    file<<temp2->id<<" \""<<temp2->title<<"\" \""<<temp2->author<<"\" "<<temp2->cost<<" "<<temp2->sellprice<<" \""<<temp2->description<<"\" "<<temp2->availablestock<<" "<<temp2->soldstock<<endl;
                    temp2 = temp2->next;
                }
            
                file.close();

                // Successful message
                system("CLS");
                cout<<"Information ofBook ID #"<<id<<" updated successfully!"<<endl;
                cout<<"Do you want to update anothor book [Y/N]: ";
                cin>>loop;
                
            }while(loop == 'Y' || loop == 'y');

        } // End of update_books function

}; // End of ABOOK class

class USERS {
    protected:
        user* head; // Head of the user linked list
        order* orderhead;
        orderdetails* orderdetailshead;

        int orderautoid = 2000;
        string pass_un, pass_phone, pass_address; // Data for current login customer, need to share with CUST class

    public:
        // Constructor to read data from users.txt, orders.txt, sales.txt
        USERS() : head(NULL), orderhead(NULL), orderdetailshead(NULL) {
            // Read users.txt
            ifstream file("users.txt");
            if (!file) {
                cerr << "Unable to open file users.txt" << endl;
                return;
            }

            string line;
            while (getline(file, line)) {
                istringstream iss(line);

                // Local variable to store current data read from txt file
                string username, password, phone, address;
                int role;

                // Read and strip quotes from each field
                if (!(getline(iss, username, '\"') && getline(iss, username, '\"') &&
                    getline(iss, password, '\"') && getline(iss, password, '\"') &&
                    getline(iss, phone, '\"') && getline(iss, phone, '\"') &&
                    getline(iss, address, '\"') && getline(iss, address, '\"') &&
                    iss >> role)) {
                    cerr << "Error reading line: " << line << endl;
                    continue;
                }

                // Create a new object and linked to previous lists
                user* new_user = new user{username, password, phone, address, role, NULL};
                new_user->next = head;
                head = new_user;
            }
            file.close();

            // Read orders.txt
            ifstream file2("orders.txt");
            if (!file2) {
                cerr << "Unable to open file orders.txt" << endl;
                return;
            }

            string line2;
            while (getline(file2, line2)) {
                istringstream iss2(line2);

                // Local variable to store current data read from txt file
                string oun, ostatus; // Order username, order status
                int oid; // Order ID
                float ocost, ototal; // Order Cost Price, Order Total

                // Read the id
                if (!(iss2 >> oid)) {
                    cerr << "Error reading id from line: " << line2 << endl;
                    continue;
                }

                orderautoid = max(orderautoid, oid);

                // Skip whitespace and the opening quote before the username
                iss2.ignore(numeric_limits<streamsize>::max(), '\"');
                // Read username until the closing quote
                if (!getline(iss2, oun, '\"')) {
                    cerr << "Error reading username from line: " << line2 << endl;
                    continue;
                }

                // Read the cost
                if (!(iss2 >> ocost)) {
                    cerr << "Error reading cost from line: " << line2 << endl;
                    continue;
                }

                // Read the total
                if (!(iss2 >> ototal)) {
                    cerr << "Error reading total from line: " << line2 << endl;
                    continue;
                }

                // Skip whitespace and the opening quote before the status
                iss2.ignore(numeric_limits<streamsize>::max(), '\"');
                // Read status until the closing quote
                if (!getline(iss2, ostatus, '\"')) {
                    cerr << "Error reading author from line: " << line2 << endl;
                    continue;
                }

                // Create a new object and linked to previous lists
                order* new_order = new order{oid, oun, ocost, ototal, ostatus, NULL};
                new_order->next = orderhead;
                orderhead = new_order;
            }
            file2.close();

            // Read orderdetails.txt
            ifstream file3("orderdetails.txt");
            if (!file3) {
                cerr << "Unable to open file orderdetails.txt" << endl;
                return;
            }

            string line3;
            while (getline(file3, line3)) {
                istringstream iss3(line3);

                // Local variable to store current data read from txt file
                int odid, odpid, odq; // Orderdetails ID, Orderdetails Product ID, Orderdetails Quantity

                // Read the odid
                if (!(iss3 >> odid)) {
                    cerr << "Error reading ID from line: " << line3 << endl;
                    continue;
                }

                // Read the product id
                if (!(iss3 >> odpid)) {
                    cerr << "Error reading Product ID from line: " << line3 << endl;
                    continue;
                }

                // Read the quantity
                if (!(iss3 >> odq)) {
                    cerr << "Error reading quantity from line: " << line3 << endl;
                    continue;
                }

                // Create a new object and linked to previous lists
                orderdetails* new_orderdetails = new orderdetails{odid, odpid, odq, NULL};
                new_orderdetails->next = orderdetailshead;
                orderdetailshead = new_orderdetails;
            }
            file3.close();
        } // End of USERS Constructor

        // Destructor to free memory
        ~USERS() {
            user* current = head;
            while (current != NULL) {
                user* next = current->next;
                delete current;
                current = next;
            }
            order* ordercurrent = orderhead;
            while (ordercurrent != NULL){
                order* next = ordercurrent->next;
                delete ordercurrent;
                ordercurrent = next;
            }
            orderdetails* orderdetailscurrent = orderdetailshead;
            while (orderdetailscurrent != NULL){
                orderdetails* next = orderdetailscurrent->next;
                delete orderdetailscurrent;
                orderdetailscurrent = next;
            }
        } // End of ~USERS Destructor

        // Register function for both admin and customer
        void register_user(int role) {
            // Local variable to store data inserted by users
            string un, pw, cpw, tel, adr;
            string rolename;

            // Boolean to check if username existed in the system
            bool unexistkey;
            // Boolean to check if both passwords matched
            bool pwcomparekey;

            if (role == 1){
                rolename = "CUSTOMER";
            }
            else if (role == 2){
                rolename = "ADMIN";
            }

            do {
                // Display title
                system("CLS");
                cout << "--------------------------------" << endl;
                cout << " REGISTER NEW " << rolename << endl;
                cout << "--------------------------------" << endl;

                // Ask for username
                cout << "Enter your username: ";
                fflush(stdin);
                getline(cin, un);

                unexistkey = false;

                // Check if this username is used before
                user *temp = head;
                while (temp != NULL && temp->username != un) {
                    temp = temp->next;
                }
                if (temp != NULL) {
                    cout << "[ERROR] Username existed! Press any key to try again.";
                    fflush(stdin);
                    getchar();
                    unexistkey = true;
                }
            } while (unexistkey == true);

            do {
                // Ask for password and confirm password
                cout << "Enter your password: ";
                fflush(stdin);
                getline(cin, pw);
                cout << "Re-enter your password: ";
                fflush(stdin);
                getline(cin, cpw);

                pwcomparekey = false;

                // Check if both passwords matched
                if (pw != cpw) {
                    cout << "[ERROR] Password unmatched! Please try again." << endl;
                    pwcomparekey = true;
                }
            } while (pwcomparekey == true);

            // Ask for phone number and address
            cout << "Enter your phone number: ";
            fflush(stdin);
            getline(cin, tel);
            cout << "Enter your address: ";
            fflush(stdin);
            getline(cin, adr);

            ofstream outfile("users.txt", ios::app);
            if (outfile.is_open()) {
                outfile << "\"" << un << "\" \"" << pw << "\" \"" << tel << "\" \"" << adr << "\" " << role << endl;
                outfile.close();
            } else {
                cerr << "Unable to open users.txt file for writing." << endl;
                return;
            }

            // Save into the linked list
            user* new_user = new user{un, pw, tel, adr, role, head};
            head = new_user;

            // Display successful message 
            cout << "Registration successfully!"<<endl;
            fflush(stdin);
            getchar();
        }

        // Login function for both admin and customer
        void login_user(int role) {
            // Local variable to store data inserted by users
            string un, pw;
            string rolename;
            // Boolean to check if credentials found in system
            bool accfound;

            if (role == 1){
                rolename = "CUSTOMER";
            }
            else if (role == 2){
                rolename = "ADMIN";
            }

            do {
                // Display title
                system("CLS");
                cout << "--------------------------------" << endl;
                cout << "  " << rolename << " LOGIN" << endl;
                cout << "--------------------------------" << endl;

                // Ask for username and password
                cout << "Enter your username: ";
                fflush(stdin);
                getline(cin, un);
                cout << "Enter your password: ";
                fflush(stdin);
                getline(cin, pw);

                accfound = false;

                // Check from the linked list if same account existed
                user *temp = head;
                while (temp != NULL && temp->username != un) {
                    temp = temp->next;
                }
                if (temp != NULL) {
                    if (un == temp->username && pw == temp->password && role == temp->role) {
                        // set accfound to true if found matched record
                        pass_un = temp->username;
                        pass_phone = temp->phone;
                        pass_address = temp->address;
                        accfound = true;
                    }
                }

                if(accfound == false){
                    cout << "[ERROR] Wrong username or password! Press any key to try again.";
                    fflush(stdin);
                    getchar();
                }

            } while (accfound == false);

            // Go back to main_menu function
        }

        // Pure virtual mainmenu function
        virtual void mainmenu() = 0;

        // Pure virtual display_orders function
        virtual void display_orders() = 0;

        // Pure virtual check_orderdetails function
        virtual void check_orderdetails() = 0;

        // Pure virtual search_orders function
        virtual void search_orders() = 0;

        // Pure virtual sort_orders function
        virtual void sort_orders() = 0;
};

class CUST : public USERS {
    private:
        cart* carthead;
        CBOOK CB;

    public:
        // Constructor to load cart.txt into linked list
        CUST() : carthead (NULL){
            ifstream file("cart.txt");
            if (!file) {
                cerr << "Unable to open file cart.txt" << endl;
                return;
            }

            string line;
            while (getline(file, line)) {
                istringstream iss(line);

                // Local variables to store current data read from txt file
                int cpid, cpq; // Cart Product ID, Cart Product Quantity
                string cun; // Cart Username

                // Skip whitespace and the opening quote before the username
                iss.ignore(numeric_limits<streamsize>::max(), '\"');
                // Read title until the closing quote
                if (!getline(iss, cun, '\"')) {
                    cerr << "Error reading Username from line: " << line << endl;
                    continue;
                }

                // Read the fields
                if (!(iss >> cpid)) {
                    cerr << "Error reading Product ID from line: " << line << endl;
                    continue;
                }

                // Read quantity directly
                if (!(iss >> cpq)) {
                    cerr << "Error reading Quantity from line: " << line << endl;
                    continue;
                }

                // Create a new object and link it to the previous lists
                cart* new_cart = new cart{cun, cpid, cpq, NULL};
                new_cart->next = carthead;
                carthead = new_cart;
            }

            file.close();
        } // End of CUST constructor

        // Destructor to free memory
        ~CUST() {
            cart* current = carthead;
            while (current != NULL) {
                cart* next = current->next;
                delete current;
                current = next;
            }
        } // End of ~CUST Destructor

        // Overriding function for Customer Main Menu Page
        void mainmenu() override {
            int option=1;

            // Keep looping until users logout
            while(option != 9){
                
                // Display title and main menu
                system("CLS");
                cout<<"Welcome back, "<<pass_un<<"!"<<endl;
                cout<<"---------------------------------------------"<<endl;
                cout<<" CUSTOMER MAIN MENU"<<endl;
                cout<<"---------------------------------------------"<<endl;
                cout<<"[1] Browse All Books"<<endl;
                cout<<"[2] Search Books"<<endl;
                cout<<"[3] Sort Books"<<endl;
                cout<<"[4] Checkout Shopping Cart"<<endl<<endl;

                cout<<"[5] Display All Orders"<<endl;
                cout<<"[6] Search Orders"<<endl;
                cout<<"[7] Sort Orders"<<endl<<endl;

                cout<<"[8] Change Password"<<endl;
                cout<<"[9] Logout"<<endl;

                cout<<"Select your action: ";
                cin>>option;

                while(option < 1 || option > 9){
                    cout<<"Invalid input, please try again [1-9]: ";
                    cin>>option;
                }

                switch(option){
                    case 1:
                        // Browse All Books
                        CB.display_books();
                        // Function to ask which book and quantity to insert into cart
                        cart_menu();
                        cout<<"Press any key to continue...";
                        fflush(stdin);
                        getchar();
                        break;
                    case 2:
                        // Search Books
                        CB.search_books();
                        break;
                    case 3:
                        // Sort Books
                        CB.sort_books();
                        break;
                    case 4:
                        // Checkout Shopping Cart
                        // Can also update cart by deleting
                        checkout();
                        cout<<"Press any key to continue...";
                        fflush(stdin);
                        getchar();
                        break;
                    case 5:
                        // Display all orders
                        system("CLS");
                        display_orders();
                        int option;
                        do{
                            // Display option menu
                            cout << "--------------------------------------------------------------------------------------------------------" << endl;
                            cout<<"[1] Check Order details"<<endl;
                            cout<<"[2] Back to Main Menu"<<endl;
                            cout<<"Enter your option: ";
                            cin>>option;

                            while(option < 1 || option > 2){
                                cout<<"Invalid input! Please try again [1-2]: ";
                                cin>>option;
                            }

                            if(option == 1){
                                check_orderdetails();
                            }
                            else if(option == 2){
                                break;
                            }
                        } while(option != 2);
                        break;
                    case 6:
                        // Search Orders
                        search_orders();
                        cout<<"Press any key to continue...";
                        fflush(stdin);
                        getchar();
                        break;
                    case 7:
                        // Sort Orders
                        sort_orders();
                        break;
                    case 8:
                        // Change Password
                        change_password();
                        cout<<"Press any key to continue...";
                        fflush(stdin);
                        getchar();
                        break;
                    case 9:
                        //Logout
                        cout<<"Logging out...Press any key to continue...";
                        fflush(stdin);
                        getchar();
                        break;
                } // End of switch case
            } // End of while loop
        } // End of mainmenu function

        // Overriding Function for customer to display orders
        void display_orders() override{
            bool found = false;

            // Display title
            cout << "--------------------------------------------------------------------------------------------------------" << endl;
            cout << "  Display Orders" << endl;
            cout << "--------------------------------------------------------------------------------------------------------" << endl;
            cout << setw(10) << "Order ID#" << setw(20) << "Order Total" << setw(20) << "Order Status" << endl;

            // Set the current to the head and check from beginning
            order* ordercurrent = orderhead;
            while (ordercurrent != NULL) {
                if (pass_un == ordercurrent->order_username) {
                    cout << setw(10) << ordercurrent->order_id << setw(20) << fixed << setprecision(2) << ordercurrent->order_grandtotal << setw(20) << ordercurrent->order_status << endl;
                    found = true;
                }
                ordercurrent = ordercurrent->next;
            }

            if (found == false){
                cout<<"No orders founds."<<endl;
            }

        } // End of customer display_orders

        // Overriding Function for customer to check order details
        void check_orderdetails() override{
            int checkid;
            bool found = false;
            // Ask for which Order ID to check details
            cout<<"Please enter Order ID to check Details: ";
            cin>>checkid;

            system("CLS");
            cout << "--------------------------------------------------------------------------------------------------------" << endl;
            cout << "  Order Details #" << checkid << endl;
            cout << "--------------------------------------------------------------------------------------------------------" << endl;

            // Set the current to the head and check from beginning
            orderdetails* orderdetailscurrent = orderdetailshead;
            while (orderdetailscurrent != NULL) {

                if (checkid == orderdetailscurrent->orderdetails_id) {
                    cout << "Book ID# " << orderdetailscurrent->orderdetails_productid << " | ";

                    // Iterate through the book list to find the matching product id
                    book *current = CB.head;
                    while (current != NULL) {
                        if (current->id == orderdetailscurrent->orderdetails_productid) {
                            cout << "Title: " << setw(20) << current->title << " | Author: " << setw(15) << current->author << " | Price: RM" << fixed << setprecision(2) << current->sellprice;
                        }
                        current = current->next;
                    }
                    cout << " | Quantity : " << orderdetailscurrent->orderdetails_quantity << endl;
                    found = true;
                }

                orderdetailscurrent = orderdetailscurrent->next;
            }
            
            // If Order ID not found
            if (found == false){
                cout<<"  Order not found."<<endl;
            }
        } // End of customer check_orderdetails function

        // Overriding and overloading Function for customer to check order details (used for search_orders function)
        void check_orderdetails(int orderID){
            int checkid;
            bool found = false;
            cout << "--------------------------------------------------------------------------------------------------------" << endl;
            cout << "  Order Details #" << orderID << endl;
            cout << "--------------------------------------------------------------------------------------------------------" << endl;

            // Set the current to the head and check from beginning
            orderdetails* orderdetailscurrent = orderdetailshead;
            while (orderdetailscurrent != NULL) {

                if (orderID == orderdetailscurrent->orderdetails_id) {
                    cout << "Book ID# " << orderdetailscurrent->orderdetails_productid << " | ";

                    // Iterate through the book list to find the matching product id
                    book *current = CB.head;
                    while (current != NULL) {
                        if (current->id == orderdetailscurrent->orderdetails_productid) {
                            cout << "Title: " << setw(20) << current->title << " | Author: " << setw(15) << current->author << " | Price: RM" << fixed << setprecision(2) << current->sellprice;
                        }
                        current = current->next;
                    }
                    cout << " | Quantity : " << orderdetailscurrent->orderdetails_quantity << endl;
                    found = true;
                }

                orderdetailscurrent = orderdetailscurrent->next;
            }
            
            // If Order ID not found
            if (found == false){
                cout<<"  Order not found."<<endl;
            }
        } // End of customer check_orderdetails function

        int getListLength(order* head) {
            int length = -1;
            while (head != NULL) {
                length++;
                head = head->next;
            }
            return length;
        }

        // Function to perform jump search based on order_id
        order* jumpSearchByID(order* head, int orderID) {
            int length = getListLength(head); int step = sqrt(length); order* prev = NULL; order* curr = head;
            if (head == NULL) {return NULL;}
            // Check if the head node is the target
            if (head->order_id == orderID) {return head;}

            // Find the block where the target order ID might be located
            while (curr != NULL && curr->order_id < orderID) {
                prev = curr;
                for (int i = 0; i < step && curr->next != NULL; i++) {
                    curr = curr->next;
                }
            }

            // Perform linear search within the block
            while (prev != NULL && prev->order_id < orderID) {
                prev = prev->next;
            }

            // Check if the order ID matches the target order ID
            if (prev != NULL && prev->order_id == orderID) {
                return prev;
            }

            return NULL; // Return NULL if the order ID is not found
        }

        // Function to display the details of the searched order
        void display_searchedorder(order* searched_order) {
            cout << "--------------------------------------------------------------------------------------------------------" << endl;
            cout << "Order ID    : " << searched_order->order_id << endl;
            cout << "Cost Total  : RM" << fixed << setprecision(2) << searched_order->order_costtotal << endl;
            cout << "Grand Total : RM" << fixed << setprecision(2) << searched_order->order_grandtotal << endl;
            cout << "Status      : " << searched_order->order_status << endl;
        }

        // Overriding Function for customer to search order
        void search_orders() override {
            int ans;
            bool orderfound = false;

            system("CLS");
            display_orders();

            cout << "--------------------------------------------------------------------------------------------------------" << endl;
            cout << "  Orders Searching" << endl;
            cout << "--------------------------------------------------------------------------------------------------------" << endl;
            cout << "[1] Order ID\n";
            cout << "[2] Main Menu\n";
            cout << "Enter your choice: ";
            cin >> ans;

            while (ans < 1 || ans > 2) {
                cout << "Invalid input, please enter again [1-2]: ";
                cin >> ans;
            }

            // Run search based on user choice
            if (ans == 1) {
                int target;

                cout << "Enter the Order ID you want to search: ";
                cin>>target;

                order* temp = orderhead;
                while(temp != NULL){
                    if (target == temp->order_id){
                        orderfound = true;
                    }
                    temp = temp->next;
                }

                if(orderfound){
                    insertion_sort_by_orderid(orderhead);
                    // Perform jump search by order ID
                    order* result = jumpSearchByID(orderhead, target);
                    if (result != NULL) {
                        // Display search result
                        system("CLS");
                        display_searchedorder(result);
                        check_orderdetails(target);
                    }
                }
                else{
                    cout << "Order not found." << endl;
                }
            }
            else if (ans == 2){
                return;
            }
        } // End of customer search_orders function

        friend void insertion_sort_by_orderid(order*& head);
        friend void insertion_sort_by_status(order*& head);

        // Overriding Function for customer to sort orders
        void sort_orders() override{
            system("CLS");
            cout<<"---------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
            cout<<"  UNSORTED DATA"<<endl;
            // Call display_books function
            display_orders();

            int choice;

            do{
                // Ask use which parameter to sort
                cout<<"---------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
                cout<<"  Orders Sorting"<<endl;
                cout<<"---------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
                cout<<"[1] Orders ID"<<endl;
                cout<<"[2] Order Status"<<endl;
                cout<<"[3] Exit to main menu"<<endl;
                cout<<"Enter your searching criteria: ";
                cin>>choice;

                while(choice < 1 || choice > 3){
                    cout<<"Invalid input! Please enter again [1-3]: ";
                    cin>>choice;
                }

                if(choice == 1){
                    insertion_sort_by_orderid(orderhead);
                    system("CLS");
                    cout<<"---------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
                    cout<<"  Sorted Result Based On Order ID"<<endl;
                    // Display orders
                    display_orders();
                }
                else if(choice == 2){
                    insertion_sort_by_status(orderhead);
                    system("CLS");
                    cout<<"---------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
                    cout<<"  Sorted Result Based On Order Status"<<endl;
                    // Display orders
                    display_orders();
                }
                else if(choice == 3){
                    return;
                }
                
            }while(choice != 3);
        }

        void change_password() {
            // Local variable to store data entered by user
            string opw, npw, ncpw; // Old password, New password, New current Password
            bool oldpasswordcorrect = false; // Loop when data entered is incorrect
            bool newpasswordmatch = false; // Loop when new passwords is unmatch

            // Display title
            system("CLS");
            cout<<"---------------------------------------------"<<endl;
            cout << "  Change Password" << endl;
            cout<<"---------------------------------------------"<<endl;

            do {
                cout << "Enter your current password: ";
                fflush(stdin);
                getline(cin, opw);

                // Check from linked list
                user *temp = head;
                while (temp != NULL && (temp->username != pass_un || temp->role != 1)) {
                    temp = temp->next;
                }

                if (temp != NULL) {
                    if (opw == temp->password) {
                        oldpasswordcorrect = true;
                    }
                }

                if (oldpasswordcorrect == false) {
                    cout << "[ERROR] Password unmatched! Please try again." << endl;
                }
            } while (oldpasswordcorrect == false);

            do{
                // Proceed with changing the password
                cout << "Enter your new password: ";
                fflush(stdin);
                getline(cin, npw);
                cout << "Confirm your new password: ";
                fflush(stdin);
                getline(cin, ncpw);

                if (npw == ncpw) {
                    user *temp = head;
                    while (temp != NULL && (temp->username != pass_un || temp->role != 1)) {
                        temp = temp->next;
                    }
                    if (temp != NULL) {
                        temp->password = npw;
                        cout << "Password changed successfully!" << endl;
                        newpasswordmatch = true;

                        // Write into file
                        ofstream file("users.txt");
                        if (!file) {
                            cerr << "Unable to open file users.txt for writing" << endl;
                            return;
                        }

                        user* temp = head;
                        while (temp != NULL) {
                            file << "\"" << temp->username << "\" \"" << temp->password << "\" \"" << temp->phone << "\" \"" << temp->address << "\" " << temp->role << endl;
                            temp = temp->next;
                        }
                        file.close();
                    } 
                }
                else {
                    cout << "[ERROR] New passwords do not match. Please try again." << endl;
                }
            }while(newpasswordmatch == false);
        } // End of customer change password function

        // Function to allow customer to add books to cart
        void cart_menu(){
            int choice;

            cout << "----------------------------------------------------------------------------------------------------------" << endl;
            cout << "[1] Add to cart" << endl;
            cout << "[2] Main Menu" << endl;
            cout << "Select your choice: ";
            cin >> choice;

            while (choice < 1 || choice > 2) {
                cout << "Invalid choice! Please try again [1-2]: ";
                cin >> choice;
            }

            if (choice == 1) {
                int bid, bq; // Book ID, Book Quantity
            char repeatcart;
            bool foundbook = false;
            bool enoughstock = false;

            do {
                do {
                    cout << "Enter Book ID you wish to add to cart: ";
                    cin >> bid;

                    book *temp = CB.head;
                    while (temp != NULL && temp->id != bid) {
                        temp = temp->next;
                    }

                    if (temp != NULL && temp->availablestock > 0) {
                        foundbook = true;

                        do {
                            // Ask user to enter quantity
                            cout << "Enter quantity you would like to purchase: ";
                            cin >> bq;

                            if (temp->availablestock < bq) {
                                cout << "Stock insufficient! Please select " << temp->availablestock << " pcs or less than " << temp->availablestock << " pcs." << endl;
                                enoughstock = false;
                            } else {
                                enoughstock = true;
                            }
                        } while (!enoughstock);

                    } else {
                        if (temp == NULL) {
                            cout << "Book ID you entered is invalid, please try again." << endl;
                        } else {
                            cout << "Book ID you entered is out of stock, please select other books." << endl;
                        }
                    }
                } while (!foundbook);

                // Check if the book is already in the cart for the given username
                cart *carttemp = carthead;
                bool inCart = false;

                while (carttemp != NULL) {
                    if (carttemp->cart_username == pass_un && carttemp->cart_productid == bid) {
                        // Find the book in the book list to get available stock
                        book *booktemp = CB.head;
                        while (booktemp != NULL && booktemp->id != bid) {
                            booktemp = booktemp->next;
                        }

                        if (booktemp != NULL) {
                            // Update quantity and check stock limit
                            if (carttemp->cart_quantity + bq <= booktemp->availablestock) {
                                carttemp->cart_quantity += bq;
                                cout << "Book ID #" << bid << " updated in cart with new quantity: " << carttemp->cart_quantity << endl;
                            } else {
                                cout << "Total quantity exceeds available stock. Current quantity in cart: " << carttemp->cart_quantity << endl;
                            }
                            inCart = true;
                        }
                        break;
                    }
                    carttemp = carttemp->next;
                }

                // If not already in the cart, add a new entry
                if (!inCart) {
                    cart* new_cart = new cart{pass_un, bid, bq, NULL};
                    new_cart->next = carthead;
                    carthead = new_cart;
                    cout << "Book ID #" << bid << " added to cart with quantity: " << bq << endl;
                }

                // Write the updated cart into the file
                ofstream file("cart.txt");
                if (!file) {
                    cerr << "Unable to open file cart.txt for writing" << endl;
                    return;
                }

                cart* tempCart = carthead;
                while (tempCart != NULL) {
                    file << "\"" << tempCart->cart_username << "\" " << tempCart->cart_productid << " " << tempCart->cart_quantity << endl;
                    tempCart = tempCart->next;
                }
                file.close();

                // Ask if user wants to add another book
                cout << "Enter [1] to add another book, any other key to finish: ";
                cin >> repeatcart;

            } while (repeatcart == '1');

            } else if (choice == 2) {
                return;
            }
        } // End of function cart_menu

        // Function for customer to checkout
        void checkout() {
            int choice;

            // Display title
            system("CLS");
            cout<<"------------------------------------------------------------------"<<endl;
            cout << "  Checkout Page" << endl;
            cout<<"------------------------------------------------------------------"<<endl;

            cout << setw(10) << "Book ID#" << setw(30) << "Book Title" << setw(10) << "Price" << setw(10) << "Quantity" << endl;

            // Display all items in cart
            cart* temp = carthead;
            while (temp != NULL) {
                if (temp->cart_username == pass_un) {
                    cout << setw(10) << temp->cart_productid;

                    book* booktemp = CB.head;
                    while (booktemp != NULL && booktemp->id != temp->cart_productid) {
                        booktemp = booktemp->next;
                    }

                    if (booktemp != NULL) {
                        cout << setw(30) << booktemp->title << setw(10) << booktemp->sellprice;
                    }

                    cout << setw(10) << temp->cart_quantity << endl;
                }
                temp = temp->next; // Move to the next item in the cart
            }

            cout << "------------------------------------------------------------------" << endl;
            cout << "1. Browse books" << endl;
            cout << "2. Edit quantity" << endl;
            cout << "3. Delete book from cart" << endl;
            cout << "4. Checkout" << endl;
            cout << "5. Return to menu" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            while (choice < 1 || choice > 5) {
                cout << "Invalid selection! Please try again [1-5]: ";
                cin >> choice;
            }

            if (choice == 1) {
                // Browse Books Function
                CB.display_books();
                cart_menu();
            }
            else if (choice == 2) {
                // Edit Book Quantity Funciton
                int tempbookid, tempquantity;
                bool bookfound = false;

                do {
                    cout << "Enter the Book ID you want to edit quantity: ";
                    cin >> tempbookid;

                    cart* temp = carthead;
                    while (temp != NULL) {
                        if (temp->cart_productid == tempbookid && temp->cart_username == pass_un) {
                            bookfound = true;
                            break;
                        }
                        temp = temp->next;
                    }

                    if (bookfound) {
                        cout << "Enter the quantity to be updated: ";
                        cin >> tempquantity;

                        book* booktemp = CB.head;
                        while (booktemp != NULL && booktemp->id != tempbookid) {
                            booktemp = booktemp->next;
                        }

                        // Check if quantity entered by user is not smaller than 0 and not more than stock available in system
                        while (tempquantity <= 0 || tempquantity > booktemp->availablestock) {
                            cout << "Input invalid! Current quantity for Book ID# " << tempbookid << " : " << booktemp->availablestock << " pcs" << endl;
                            cout << "Please enter the quantity again: ";
                            cin >> tempquantity;
                        }

                        temp->cart_quantity = tempquantity;
                    }
                    else {
                        cout << "Book ID #" << tempbookid << " not found! Please try again." << endl;
                    }
                } while (!bookfound);

                // Write into file
                ofstream file("cart.txt");
                if (!file) {
                    cerr << "Unable to open file cart.txt for writing" << endl;
                    return;
                }

                cart* temp2 = carthead;
                while (temp2 != NULL) {
                    file << "\"" << temp2->cart_username << "\" " << temp2->cart_productid << " " << temp2->cart_quantity << endl;
                    temp2 = temp2->next;
                }
                file.close();

                // Display successful message
                cout << "Quantity updated for Book ID #" << tempbookid << "! Press any key to continue...";
                fflush(stdin);
                getchar();
                checkout();
            }
            else if (choice == 3) {
                // Delete book from cart
                int tempbookid;
                bool bookfound = false;

                do {
                    cout << "Enter the Book ID you want to delete: ";
                    cin >> tempbookid;
                    
                    // Check if the head node needs to be deleted (Special Handling Case)
                    if (carthead != NULL && carthead->cart_productid == tempbookid && carthead->cart_username == pass_un) {
                        bookfound = true;
                        cart* temp = carthead;
                        carthead = carthead->next; // Move head to the next node
                        delete temp;
                        break;
                    }

                    // Normal case (deleting node not head node)
                    cart *prev = NULL;
                    cart* temp = carthead;
                    while (temp != NULL) {
                        if (temp->cart_productid == tempbookid && temp->cart_username == pass_un) {
                            bookfound = true;
                            break;
                        }
                        prev = temp;
                        temp = temp->next;
                    }

                    if (bookfound) {
                        if (prev != NULL) {
                            prev->next = temp->next;
                        }
                        delete temp;
                    }
                    else {
                        cout << "Book ID #" << tempbookid << " not found! Please try again." << endl;
                    }
                } while (!bookfound);

                // Write into file
                ofstream file("cart.txt");
                if (!file) {
                    cerr << "Unable to open file cart.txt for writing" << endl;
                    return;
                }

                cart* temp2 = carthead;
                while (temp2 != NULL) {
                    file << "\"" << temp2->cart_username << "\" " << temp2->cart_productid << " " << temp2->cart_quantity << endl;
                    temp2 = temp2->next;
                }
                file.close();

                // Display successful message
                cout << "Book ID #" << tempbookid << " deleted from the cart! Press any key to continue";
                fflush(stdin);
                getchar();
                checkout();
            }
            else if (choice == 4) {
                // Checkout Function
                float total_cost = 0;
                float total_sell_price = 0;

                // Update book soldstock and available stock for each book in cart
                temp = carthead;
                while (temp != NULL) {
                    if (temp->cart_username == pass_un) {
                        book* booktemp = CB.head;
                        while (booktemp != NULL && booktemp->id != temp->cart_productid) {
                            booktemp = booktemp->next;
                        }

                        if (booktemp != NULL) {
                            booktemp->soldstock += temp->cart_quantity;
                            booktemp->availablestock -= temp->cart_quantity;
                            total_cost += booktemp->cost * temp->cart_quantity;
                            total_sell_price += booktemp->sellprice * temp->cart_quantity;
                        }
                    }
                    temp = temp->next;
                }

                // Add order into order linked list and orders.txt
                order* new_order = new order{(++orderautoid), pass_un, total_cost, total_sell_price, "Order placed", NULL};
                new_order->next = orderhead;
                orderhead = new_order;

                ofstream orderfile("orders.txt", ios::app);
                if (!orderfile) {
                    cerr << "Unable to open file orders.txt for writing" << endl;
                    return;
                }
                orderfile << new_order->order_id << " \"" << new_order->order_username << "\" " << new_order->order_costtotal << " " << new_order->order_grandtotal << " \"" << new_order->order_status << "\"" << endl;
                orderfile.close();

                // Add order details for each ordered book into orderdetails linked list and orderdetails.txt
                temp = carthead;
                while (temp != NULL) {
                    if (temp->cart_username == pass_un) {
                        orderdetails* new_orderdetails = new orderdetails{new_order->order_id, temp->cart_productid, temp->cart_quantity, NULL};
                        new_orderdetails->next = orderdetailshead;
                        orderdetailshead = new_orderdetails;

                        ofstream orderdetailsfile("orderdetails.txt", ios::app);
                        if (!orderdetailsfile) {
                            cerr << "Unable to open file orderdetails.txt for writing" << endl;
                            return;
                        }
                        orderdetailsfile << new_orderdetails->orderdetails_id << " " << new_orderdetails->orderdetails_productid << " " << new_orderdetails->orderdetails_quantity << endl;
                        orderdetailsfile.close();
                    }
                    temp = temp->next;
                }

                // Delete the items that are already placed in cart linked list and cart.txt after checkout
                cart* prev = NULL;
                temp = carthead;
                while (temp != NULL) {
                    if (temp->cart_username == pass_un) {
                        // If the deleting node is head node (Special Handling Case)
                        if (prev == NULL) {
                            carthead = temp->next;
                            delete temp;
                            temp = carthead;
                        } else {
                            prev->next = temp->next;
                            delete temp;
                            temp = prev->next;
                        }
                    } else {
                        prev = temp;
                        temp = temp->next;
                    }
                }

                // Delete items from cart.txt and cart linked list
                ofstream cartfile("cart.txt");
                if (!cartfile) {
                    cerr << "Unable to open file cart.txt for writing" << endl;
                    return;
                }
                temp = carthead;
                while (temp != NULL) {
                    cartfile << "\"" << temp->cart_username << "\" " << temp->cart_productid << " " << temp->cart_quantity << endl;
                    temp = temp->next;
                }
                cartfile.close();

                // Update books.txt with new availablestock and soldstock values
                ofstream booksfile("books.txt");
                if (!booksfile) {
                    cerr << "Unable to open file books.txt for writing" << endl;
                    return;
                }
                book* booktemp = CB.head;
                while (booktemp != NULL) {
                    booksfile<<booktemp->id<<" \""<<booktemp->title<<"\" \""<<booktemp->author<<"\" "<<booktemp->cost<<" "<<booktemp->sellprice<<" \""<<booktemp->description<<"\" "<<booktemp->availablestock<<" "<<booktemp->soldstock<<endl;
                    booktemp = booktemp->next;
                }
                booksfile.close();

                system("CLS");
                // Display receipt
                cout<<"---------------------------------------------"<<endl;
                cout << "Receipt:" << endl;
                cout<<"---------------------------------------------"<<endl;
                cout << setw(20) << "Book ID#" << setw(30) << "Book Title" << setw(10) << "Price" << setw(10) << "Quantity" << setw(10) << "Total" << endl;

                orderdetails* odtemp = orderdetailshead;
                while (odtemp != NULL) {
                    if (odtemp->orderdetails_id == new_order->order_id) {
                        book* booktemp = CB.head;
                        while (booktemp != NULL && booktemp->id != odtemp->orderdetails_productid) {
                            booktemp = booktemp->next;
                        }

                        if (booktemp != NULL) {
                            double price = booktemp->sellprice * odtemp->orderdetails_quantity;
                            cout << setw(20) << odtemp->orderdetails_productid << setw(30) << booktemp->title << setw(10) << booktemp->sellprice << setw(10) << odtemp->orderdetails_quantity << setw(10) << fixed << setprecision(2) << price << endl;
                        }
                    }
                    odtemp = odtemp->next;
                }

                cout<<"---------------------------------------------"<<endl;
                cout << "Order ID #" << new_order->order_id<<endl;
                cout << "Grandtotal: RM" << total_sell_price << endl;

                cout << "Thank you for purchasing with us!" << endl;
            }
            else if (choice == 5) {
                return;
            }
        } // End of customer checkout function
}; // End of CUST class

class ADMIN : public USERS {
    private:
        ABOOK AB;

    public:
        // Overriding function for Admin Main Menu Page
        void mainmenu() override {
            int option = 1;

            while(option != 13){
                // Display title and main menu
                system("CLS");
                cout<<"---------------------------------------------"<<endl;
                cout<<" ADMIN MAIN MENU"<<endl;
                cout<<"---------------------------------------------"<<endl;
                cout<<"[1]  Add New Books"<<endl;
                cout<<"[2]  Delete Books"<<endl;
                cout<<"[3]  Update Books Information"<<endl;
                cout<<"[4]  Display all Books"<<endl;
                cout<<"[5]  Search Books"<<endl;
                cout<<"[6]  Sort Books"<<endl<<endl;
                
                cout<<"[7]  Display Orders"<<endl;
                cout<<"[8]  Update Orders Status"<<endl;
                cout<<"[9]  Search Orders"<<endl;
                cout<<"[10] Sort Orders"<<endl<<endl;

                cout<<"[11] Sales Report"<<endl;
                cout<<"[12] Add New Admin"<<endl;
                cout<<"[13] Logout"<<endl;

                cout<<"Select your action: ";
                cin>>option;

                while(option < 1 || option > 13){
                    cout<<"Invalid input, please try again [1-13]: ";
                    cin>>option;
                }

                switch(option){
                    case 1:
                        // Add New Books
                        AB.add_books();
                        cout<<"Press any key to continue...";
                        fflush(stdin);
                        getchar();
                        break;
                    case 2:
                        // Delete Books
                        AB.delete_books();
                        cout<<"Press any key to continue...";
                        fflush(stdin);
                        getchar();
                        break;
                    case 3:
                        // Update Books Information
                        AB.update_books();
                        cout<<"Press any key to continue...";
                        fflush(stdin);
                        getchar();
                        break;
                    case 4:
                        // Display All Books
                        system("CLS");
                        AB.display_books();
                        cout<<"Press any key to continue...";
                        fflush(stdin);
                        getchar();
                        break;
                    case 5:
                        // Search Books
                        AB.search_books();
                        break;
                    case 6:
                        // Sort Books
                        AB.sort_books();
                        break;
                    case 7:
                        // Display Orders
                        system("CLS");
                        display_orders();
                        int option;

                        do{
                            // Display option menu
                            cout << "--------------------------------------------------------------------------------------------------------" << endl;
                            cout<<"[1] Check Order details"<<endl;
                            cout<<"[2] Back to Main Menu"<<endl;
                            cout<<"Enter your option: ";
                            cin>>option;

                            while(option < 1 || option > 2){
                                cout<<"Invalid input! Please try again [1-2]: ";
                                cin>>option;
                            }

                            if(option == 1){
                                check_orderdetails();
                            }
                            else if(option == 2){
                                break;
                            }
                        } while(option != 2);
                        break;
                    case 8:
                        // Update Orders Status
                        system("CLS");
                        update_status();
                        break;
                    case 9:
                        // Search Orders
                        search_orders();
                        cout<<"Press any key to continue...";
                        fflush(stdin);
                        getchar();
                        break;
                    case 10:
                        // Sort Orders
                        sort_orders();
                        break;
                    case 11:
                        // Sales Report
                        sales_report();
                        break;
                    case 12:
                        // Add New Admin
                        register_user(2);
                        cout<<"Press any key to continue...";
                        fflush(stdin);
                        getchar();
                        break;
                    case 13:
                        // Logout
                        cout<<"Logging out...Press any key to continue...";
                        fflush(stdin);
                        getchar();
                        break;
                } // End of switch case
            } // End of while loop
        } // End of admin mainmenu

        // Overriding Function for admin to display all orders
        void display_orders() override{
            bool found = false;

            // Display title
            cout << "--------------------------------------------------------------------------------------------------------" << endl;
            cout << "  Display Orders" << endl;
            cout << "--------------------------------------------------------------------------------------------------------" << endl;
            cout << setw(10) << "Order ID#" << setw(20) << "Order By" << setw(20) << "Order Cost" << setw(20) << "Order Total" << setw(20) << "Order Status" << endl;

            // Set the current to the head and check from beginning
            order* ordercurrent = orderhead;
            while (ordercurrent != NULL) {
                cout << setw(10) << ordercurrent->order_id << setw(20) << ordercurrent->order_username << setw(20) << fixed << setprecision(2) << ordercurrent->order_costtotal << setw(20) << fixed << setprecision(2) << ordercurrent->order_grandtotal << setw(20) << ordercurrent->order_status << endl;
                found = true;
                ordercurrent = ordercurrent->next;
            }

            if (found == false){
                cout<<"No orders founds."<<endl;
            }
        } // End of admin display_orders function

        // Function for admin to check order details
        void check_orderdetails() override {
            int checkid;
            bool found = false;
            string order_username;
            
            // Ask for which Order ID to check details
            cout << "Please enter Order ID to check details: ";
            cin >> checkid;

            system("CLS");
            cout << "--------------------------------------------------------------------------------------------------------" << endl;
            cout << "  Order Details #" << checkid << endl;
            cout << "--------------------------------------------------------------------------------------------------------" << endl;

            // Set the current to the head and check from beginning
            orderdetails* orderdetailscurrent = orderdetailshead;
            while (orderdetailscurrent != NULL) {

                if (checkid == orderdetailscurrent->orderdetails_id) {
                    cout << "Book ID# " << orderdetailscurrent->orderdetails_productid << " | ";

                    // Iterate through the book list to find the matching product id
                    book* current = AB.head;
                    while (current != NULL) {
                        if (current->id == orderdetailscurrent->orderdetails_productid) {
                            cout << "Title: " << setw(20) << current->title << " | Author: " << setw(15) << current->author << " | Price: RM" << fixed << setprecision(2) << current->sellprice;
                        }
                        current = current->next;
                    }
                    cout << " | Quantity : " << orderdetailscurrent->orderdetails_quantity << endl;
                    found = true;
                }

                orderdetailscurrent = orderdetailscurrent->next;
            }

            // If Order ID not found
            if (found == false) {
                cout << "  Order not found." << endl;
                return;
            }

            // Display customer information
            // First, find the order to get the username
            order* ordercurrent = orderhead;
            while (ordercurrent != NULL) {
                if (ordercurrent->order_id == checkid) {
                    order_username = ordercurrent->order_username;
                    break;
                }
                ordercurrent = ordercurrent->next;
            }

            if (!order_username.empty()) {
                // Find customer details using the username
                user* custcurrent = head;
                while (custcurrent != NULL) {
                    if (custcurrent->username == order_username) {
                        cout << "--------------------------------------------------------------------------------------------------------" << endl;
                        cout << "Customer Name:    " << custcurrent->username << endl;
                        cout << "Customer Phone:   " << custcurrent->phone << endl;
                        cout << "Customer Address: " << custcurrent->address << endl;
                        break;
                    }
                    custcurrent = custcurrent->next;
                }
            }
        } // End of admin check orderdetails

        // Overriding and overloading Function for admin to check order details (used for search_orders function)
        void check_orderdetails(int orderID){
            int checkid;
            bool found = false;
            cout << "--------------------------------------------------------------------------------------------------------" << endl;
            cout << "  Order Details #" << orderID << endl;
            cout << "--------------------------------------------------------------------------------------------------------" << endl;

            // Set the current to the head and check from beginning
            orderdetails* orderdetailscurrent = orderdetailshead;
            while (orderdetailscurrent != NULL) {

                if (orderID == orderdetailscurrent->orderdetails_id) {
                    cout << "Book ID# " << orderdetailscurrent->orderdetails_productid << " | ";

                    // Iterate through the book list to find the matching product id
                    book *current = AB.head;
                    while (current != NULL) {
                        if (current->id == orderdetailscurrent->orderdetails_productid) {
                            cout << "Title: " << setw(20) << current->title << " | Author: " << setw(15) << current->author << " | Price: RM" << fixed << setprecision(2) << current->sellprice;
                        }
                        current = current->next;
                    }
                    cout << " | Quantity : " << orderdetailscurrent->orderdetails_quantity << endl;
                    found = true;
                }

                orderdetailscurrent = orderdetailscurrent->next;
            }
            
            // If Order ID not found
            if (found == false){
                cout<<"  Order not found."<<endl;
            }
        } // End of admin check_orderdetails function

        int getListLength(order* head) {
            int length = -1;
            while (head != NULL) {
                length++;
                head = head->next;
            }
            return length;
        }

        // Function to perform jump search based on order_id
        order* jumpSearchByID(order* head, int orderID) {
            int length = getListLength(head); int step = sqrt(length); order* prev = NULL; order* curr = head;
            if (head == NULL) {return NULL;}
            // Check if the head node is the target
            if (head->order_id == orderID) {return head;}

            // Find the block where the target order ID might be located
            while (curr != NULL && curr->order_id < orderID) {
                prev = curr;
                for (int i = 0; i < step && curr->next != NULL; i++) {
                    curr = curr->next;
                }
            }

            // Perform linear search within the block
            while (prev != NULL && prev->order_id < orderID) {
                prev = prev->next;
            }

            // Check if the order ID matches the target order ID
            if (prev != NULL && prev->order_id == orderID) {
                return prev;
            }

            return NULL; // Return NULL if the order ID is not found
        }

        // Function to display the details of the searched order
        void display_searchedorder(order* searched_order) {
            cout << "--------------------------------------------------------------------------------------------------------" << endl;
            cout << "Order ID    : " << searched_order->order_id << endl;
            cout << "Cost Total  : RM" << fixed << setprecision(2) << searched_order->order_costtotal << endl;
            cout << "Grand Total : RM" << fixed << setprecision(2) << searched_order->order_grandtotal << endl;
            cout << "Status      : " << searched_order->order_status << endl;
        }

        // Overriding Function for admin to search order
        void search_orders() override {
            int ans;
            bool orderfound = false;

            system("CLS");
            display_orders();

            cout << "--------------------------------------------------------------------------------------------------------" << endl;
            cout << "  Orders Searching" << endl;
            cout << "--------------------------------------------------------------------------------------------------------" << endl;
            cout << "[1] Order ID\n";
            cout << "[2] Main Menu\n";
            cout << "Enter your choice: ";
            cin >> ans;

            while (ans < 1 || ans > 2) {
                cout << "Invalid input, please enter again [1-2]: ";
                cin >> ans;
            }

            // Run search based on user choice
            if (ans == 1) {
                int target;

                cout << "Enter the Order ID you want to search: ";
                cin>>target;

                order* temp = orderhead;
                while(temp != NULL){
                    if (target == temp->order_id){
                        orderfound = true;
                    }
                    temp = temp->next;
                }

                if(orderfound){
                    insertion_sort_by_orderid(orderhead);
                    // Perform jump search by order ID
                    order* result = jumpSearchByID(orderhead, target);
                    if (result != NULL) {
                        // Display search result
                        system("CLS");
                        display_searchedorder(result);
                        check_orderdetails(target);
                    }
                }
                else{
                    cout << "Order not found." << endl;
                }
            }
            else if (ans == 2){
                return;
            }
        } // End of admin search_orders function

        friend void insertion_sort_by_orderid(order*& head);
        friend void insertion_sort_by_username(order*& head);
        friend void insertion_sort_by_status(order*& head);

        // Overriding Function for admin to sort orders
        void sort_orders() override{
            system("CLS");
            cout<<"---------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
            cout<<"  UNSORTED DATA"<<endl;
            // Call display_books function
            display_orders();

            int choice;

            do{
                // Ask use which parameter to sort
                cout<<"---------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
                cout<<"  Orders Sorting"<<endl;
                cout<<"---------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
                cout<<"[1] Orders ID"<<endl;
                cout<<"[2] Order Username"<<endl;
                cout<<"[3] Order Status"<<endl;
                cout<<"[4] Exit to main menu"<<endl;
                cout<<"Enter your searching criteria: ";
                cin>>choice;

                while(choice < 1 || choice > 4){
                    cout<<"Invalid input! Please enter again [1-4]: ";
                    cin>>choice;
                }

                if(choice == 1){
                    insertion_sort_by_orderid(orderhead);
                    system("CLS");
                    cout<<"---------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
                    cout<<"  Sorted Result Based On Order ID"<<endl;
                    // Display orders
                    display_orders();
                }
                else if(choice == 2){
                    insertion_sort_by_username(orderhead);
                    system("CLS");
                    cout<<"---------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
                    cout<<"  Sorted Result Based On Order Username"<<endl;
                    // Display orders
                    display_orders();
                }
                else if(choice == 3){
                    insertion_sort_by_status(orderhead);
                    system("CLS");
                    cout<<"---------------------------------------------------------------------------------------------------------------------------------------------"<<endl;
                    cout<<"  Sorted Result Based On Order Status"<<endl;
                    // Display orders
                    display_orders();
                }
                else if(choice == 4){
                    return;
                }
                
            }while(choice != 4);
        }

        // Function for admin to update order status
        void update_status(){
            char repeat;

            // Keep looping when admin wants to update another order
            do{
                // Display orders
                display_orders();

                int choice;
                bool foundorder = false;

                // Loop when order id not found
                do{
                    // Ask admin which order to update status
                    cout<<"--------------------------------------------------------------------------------------------------------"<<endl;
                    cout<<"  Update Order Status"<<endl;
                    cout<<"--------------------------------------------------------------------------------------------------------"<<endl;
                    cout<<"Select an order ID to update the status: ";
                    cin>>choice;

                    order* temp = orderhead;
                    while(temp != NULL && temp->order_id != choice){
                        temp = temp->next;
                    }

                    if(temp != NULL){
                        // Ask admin which status to update
                        int newstatus;

                        foundorder = true;
                        cout<<"--------------------------"<<endl;
                        cout<<"  Order ID #"<<choice<<endl;
                        cout<<"--------------------------"<<endl;
                        cout<<"[1] Preparing"<<endl;
                        cout<<"[2] Delivered"<<endl;
                        cout<<"[3] Cancelled"<<endl;
                        cout<<"Select a status to update: ";
                        cin>>newstatus;

                        while(newstatus < 1 || newstatus > 3){
                            cout<<"Invalid selection, please try again [1-3]: ";
                            cin>>newstatus;
                        }

                        if(newstatus == 1){
                            temp->order_status = "Preparing";
                        }
                        else if(newstatus == 2){
                            temp->order_status = "Delivered";
                        }
                        else if(newstatus == 3){
                            temp->order_status = "Cancelled";
                        }

                        // Write into file
                        ofstream file("orders.txt");
                        if (!file) {
                            cerr << "Unable to open file orders.txt for writing" << endl;
                            return;
                        }

                        order* temp = orderhead;
                        while (temp != NULL) {
                            file << temp->order_id << " \"" << temp->order_username << "\" " << temp->order_costtotal << " " << temp->order_grandtotal << " \"" << temp->order_status << "\""<< endl;
                            temp = temp->next;
                        }
                        file.close();

                        system("CLS");
                        cout<<"Order status updated successfully!"<<endl;
                    
                    }

                    if(foundorder == false){
                        system("CLS");
                        cout<<"Order not found, please try again."<<endl;
                    }
                } while(foundorder == false);

                // Ask if admin wants to udpate another status
                cout<<"Enter [1] to update another status: ";
                cin>>repeat;
            } while(repeat == '1');
        } // End of admin update status function
       
        // Function to generate sales report
        void sales_report() {
            int option;

            do{
                do{

                    //ask for user input
                    system("CLS");
                    cout<<"-------------------------------------"<<endl;
                    cout<<"  Sales Report "<<endl;
                    cout<<"-------------------------------------"<<endl;
                    cout<<"[1] Earning and Profit"<<endl;
                    cout<<"[2] Sold Book"<<endl;
                    cout<<"[3] Main Menu"<<endl;
                    cout<<"Please select Your option : ";
                    cin>>option;

                    //display error message if user enter invalid code
                    if(option < 1 || option > 3){
                        cout<<"Invalid option, Press enter to try again...";
                        fflush(stdin);
                        getchar();
                    }
                
                }while(option < 1 || option > 3);

                if(option == 1){
                    float totalcost = 0;
                    float totalearning = 0;
                    float totalprofit = 0; // totalearning - totalcost

                    order* temp = orderhead;
                    while(temp != NULL){
                        totalcost += temp->order_costtotal;
                        totalearning += temp->order_grandtotal;
                        temp = temp->next;
                    }

                    // Calculate total profit
                    totalprofit = totalearning - totalcost;

                    //display result
                    system("CLS");
                    cout<<"-------------------------------------"<<endl;
                    cout<<"  Earning and Profit"<<endl;
                    cout<<"-------------------------------------"<<endl;
                    cout<<"Total cost    : RM "<<fixed<<setprecision(2)<<totalcost<<endl;
                    cout<<"Total earning : RM "<<fixed<<setprecision(2)<<totalearning<<endl;
                    cout<<"Profit        : RM "<<fixed<<setprecision(2)<<totalprofit<<endl;

                    cout<<"Press any key to continue ";
                    fflush(stdin);
                    getchar();
                }
                else if(option == 2){
                    system("CLS");
                    //display title
                    cout<<"----------------------------------------------------------------------------------------------------"<<endl;
                    cout<<"  Sold Book"<<endl;
                    cout<<"----------------------------------------------------------------------------------------------------"<<endl;
                    cout<<setw(8)<<"Book ID"<<setw(20)<<"Title"<<setw(15)<<"Sold Stock"<<setw(15)<<"Total Cost"<<setw(15)<<"Total Earning"<<setw(15)<<"Total Profit"<<endl;

                    book* temp = AB.head; 
                    while(temp != NULL){
                        cout<<setw(8)<<temp->id<<setw(20)<<temp->title<<setw(15)<<temp->soldstock<<setw(15)<< fixed << setprecision(2) <<(temp->soldstock * temp->cost)<<setw(15)<< fixed << setprecision(2) <<(temp->soldstock * temp->sellprice)<<setw(15) << fixed << setprecision(2) <<((temp->soldstock * temp->sellprice)-(temp->soldstock * temp->cost))<<endl;
                        temp = temp->next;
                    }

                    cout<<"Press any key to continue...";
                    fflush(stdin);
                    getchar();

                }else if(option == 3){
                    return;
                }
            
            }while(option != 3);
           
        } // End of sales_report function
  
}; // End of ADMIN class

// Function to sort books based on Book ID using insertion sort
void insertion_sort_by_id(book*& head) {
    if (head == NULL || head->next == NULL) {
        return;
    }

    book* sorted = NULL;
    book* current = head;

    while (current != NULL) {
        book* next = current->next;
        if (sorted == NULL || current->id < sorted->id) {
            current->next = sorted;
            sorted = current;
        } else {
            book* temp = sorted;
            while (temp->next != NULL && temp->next->id < current->id) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        current = next;
    }
    head = sorted;
}

// Function to sort books based on Book Title using insertion sort
void insertion_sort_by_title(book*& head) {
    if (head == NULL || head->next == NULL) {
        return;
    }

    book* sorted = NULL;
    book* current = head;

    while (current != NULL) {
        book* next = current->next;
        if (sorted == NULL || current->title < sorted->title) {
            current->next = sorted;
            sorted = current;
        } else {
            book* temp = sorted;
            while (temp->next != NULL && temp->next->title < current->title) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        current = next;
    }
    head = sorted;
}

// Function to sort books based on Author using insertion sort
void insertion_sort_by_author(book*& head) {
    if (head == NULL || head->next == NULL) {
        return;
    }

    book* sorted = NULL;
    book* current = head;

    while (current != NULL) {
        book* next = current->next;
        if (sorted == NULL || current->author < sorted->author) {
            current->next = sorted;
            sorted = current;
        } else {
            book* temp = sorted;
            while (temp->next != NULL && temp->next->author < current->author) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        current = next;
    }
    head = sorted;
}

// Function to sort orders based on Order ID using insertion sort
void insertion_sort_by_orderid(order*& head) {
    if (head == NULL || head->next == NULL) {
        return;
    }

    order* sorted = NULL;
    order* current = head;

    while (current != NULL) {
        order* next = current->next;
        if (sorted == NULL || current->order_id < sorted->order_id) {
            current->next = sorted;
            sorted = current;
        } else {
            order* temp = sorted;
            while (temp->next != NULL && temp->next->order_id < current->order_id) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        current = next;
    }
    head = sorted;
}

void insertion_sort_by_username(order*& head) {
    if (head == NULL || head->next == NULL) {
        return;
    }

    order* sorted = NULL;
    order* current = head;

    while (current != NULL) {
        order* next = current->next;
        if (sorted == NULL || current->order_username < sorted->order_username) {
            current->next = sorted;
            sorted = current;
        } else {
            order* temp = sorted;
            while (temp->next != NULL && temp->next->order_username < current->order_username) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        current = next;
    }
    head = sorted;
}

// Function to sort orders based on Order ID using insertion sort
void insertion_sort_by_status(order*& head) {
    if (head == NULL || head->next == NULL) {
        return;
    }

    order* sorted = NULL;
    order* current = head;

    while (current != NULL) {
        order* next = current->next;
        if (sorted == NULL || current->order_status < sorted->order_status) {
            current->next = sorted;
            sorted = current;
        } else {
            order* temp = sorted;
            while (temp->next != NULL && temp->next->order_status < current->order_status) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        current = next;
    }
    head = sorted;
}

void main_menu() {
    int option;

    // Keep looping until user exit
    do{
        system("CLS");
        cout<<"---------------------------------------------"<<endl;
        cout<<"\tWELCOME TO TEEPLE K BOOKSTORE"<<endl;
        cout<<"---------------------------------------------"<<endl;
        cout<<"[1] Customer"<<endl;
        cout<<"[2] Admin"<<endl;
        cout<<"[3] Exit"<<endl;
        cout<<"Select your role: ";
        cin >> option;

        while (option < 1 || option > 3) {
            cout << "Invalid input, please enter again [1-3]: ";
            cin >> option;
        }

        if (option == 1) {
            // Create appropriate object
            CUST C;
            int option2;

            system("CLS");
            cout<<"---------------------------------------------"<<endl;
            cout<<" CUSTOMER MENU"<<endl;
            cout<<"---------------------------------------------"<<endl;
            cout<<"[1] Login"<<endl;
            cout<<"[2] Register"<<endl;
            cout<<"Select your option: ";
            cin >> option2;

            while (option2 < 1 || option2 > 2) {
                cout << "Invalid input, please enter again [1-2]: ";
                cin >> option2;
            }

            if (option2 == 1) {
                // Pass to login page with parameter 1 (1 = customer)
                cout << "Customer Login" << endl;
                C.login_user(1);
            }
            else if (option2 == 2) {
                // Pass to register page with parameter 1 (1 = customer)
                cout << "Customer Registration" << endl;
                C.register_user(1);
                // Display successful message and redirect to login page
                cout << "Redirecting to login page..." << endl;
                C.login_user(1);
            }

            C.mainmenu();

        }
        else if (option == 2) {
            ADMIN A;
            int option2;

            system("CLS");
            cout<<"---------------------------------------------"<<endl;
            cout<<"  ADMIN MENU"<<endl;
            cout<<"---------------------------------------------"<<endl;
            cout<<"[1] Login"<<endl;
            cout<<"[2] Register"<<endl;
            cout<<"Select your option: ";
            cin >> option2;

            while (option2 < 1 || option2 > 2) {
                cout << "Invalid input, please enter again [1-2]: ";
                cin >> option2;
            }

            if (option2 == 1) {
                // Pass to login page with parameter 2 (2 = admin)
                cout << "Admin Login" << endl;
                A.login_user(2);
            } else if (option2 == 2) {
                // Pass to register page with parameter 2 (2 = admin)
                cout << "Admin Registration" << endl;
                A.register_user(2);
                // Display successful message and redirect to login page
                cout << "Redirecting to login page..." << endl;
                A.login_user(2);
            }

            A.mainmenu();

        }
        else if (option == 3) {
            system("CLS");
            cout<<"\nThank you for using our system, have a nice day! :)\n"<<endl;
        }
    }while(option != 3);
} // End of main_menu function

int main() {
    // Call Bookstore main menu
    main_menu();
    return 0;
}