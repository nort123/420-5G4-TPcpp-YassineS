#include "book.h"

using namespace std;

//constructeur defaut
Book::Book() : title(""),author(""),isbn(""),isAvailable(true),borrowerId(""){}

//constructeur a parametre
Book::Book(const string& title, const string& author, const string& isbn)
    : title(title), author(author), isbn(isbn) {}

//getters
string Book::getTitle() const {return title;}
string Book::getAuthor() const {return author;}
string Book::getISBN() const {return isbn;}
bool Book::getAvailability() const {return isAvailable;}
string Book::getBorrowerId() const {return borrowerId;}

//setters



