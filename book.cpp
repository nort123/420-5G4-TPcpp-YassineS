#include <sstream>

#include "book.h"


using namespace std;

//constructeur defaut
Book::Book() : title(""),author(""),isbn(""),isAvailable(true),borrowerId(""){}

//constructeur a parametre
Book::Book(const string& title, const string& author, const string& isbn)
    : title(title), author(author), isbn(isbn), isAvailable(true), borrowerId("") {}

//getters
string Book::getTitle() const {return title;}
string Book::getAuthor() const {return author;}
string Book::getISBN() const {return isbn;}
bool Book::getAvailability() const {return isAvailable;}
string Book::getBorrowerId() const {return borrowerId;}

//setters
void Book::setTitle(const string& title) {this->title = title; }
void Book::setAuthor(const string& author) {this->author = author;}
void Book::setISBN(const string& isbn){this->isbn = isbn;}
void Book::setBorrowerId(const string& id){this-> borrowerId = id;}
void Book::setAvailability(const bool available){this-> isAvailable = available;}


//methodes

void Book::checkOut(const string& borrowerId){
    setAvailability(false); 
    setBorrowerId(borrowerId);
}

void Book::returnBook(){
    setAvailability(true);
    setBorrowerId("");
}

string Book::toString() const{
    string result ="";


    if (isAvailable)
    {
        result = "Titre: " + title + "\nAuteur: " + author + 
                        "\nISBN : " + isbn + "\nStatut: Disponible";
    }else{
        result = "Titre: " + title + "\nAuteur: " + author + 
                        "\nISBN : " + isbn + "\nStatut: Emprunte par : " + borrowerId ;
    }
    



    return result;
}

string Book::toFileFormat() const{
    string result = "";

    if (isAvailable)
    {
        result = title + "|" + author + "|" + isbn + "|1|";
    }else{
        result = title + "|" + author + "|" + isbn + "|0|" + borrowerId;
    }
    



    return result;
}

void Book::fromFileFormat(const string& line){
    stringstream ss(line);
    string token;

    getline(ss,title,'|');
    getline(ss,author,'|');
    getline(ss,isbn,'|');

    string estDispo = "";

    getline(ss,estDispo,'|');

    if (estDispo == "1")
    {
        isAvailable = true;
    }else if (estDispo == "0")
    {
        isAvailable = false;
        getline(ss,borrowerId,'|');

    }


    
    



}

