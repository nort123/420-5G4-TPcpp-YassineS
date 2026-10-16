#include <fstream>
#include <iostream>
#include <filesystem>
#include "filemanager.h"
#include <algorithm>

using namespace std;

// Constructor
FileManager::FileManager(const string& booksFile, const string& usersFile)
    : booksFileName(booksFile), usersFileName(usersFile) {}

// Save all library data
bool FileManager::saveLibraryData(Library& library) {
    return saveBooksToFile(library) && saveUsersToFile(library);
}

// Load all library data
bool FileManager::loadLibraryData(Library& library) {
    bool booksLoaded = loadBooksFromFile(library);
    bool usersLoaded = loadUsersFromFile(library);
    return booksLoaded || usersLoaded; // Return true if at least one file was loaded
}

// Save books to file
bool FileManager::saveBooksToFile(Library& library) {
    ofstream file(booksFileName);
    if (!file.is_open()) {
        cout << "Erreur : Impossible d'ouvrir " << booksFileName << " en écriture.\n";
        return false;
    }
    
    auto books = library.getAllBooks();
    for (Book* book : books) {
        file << book->toFileFormat() << "\n";
    }
    
    file.close();
    return true;
}

// Save users to file
bool FileManager::saveUsersToFile(Library& library) {
    ofstream file(usersFileName);
    if (!file.is_open()) {
        cout << "Erreur : Impossible d'ouvrir " << usersFileName << " en écriture.\n";
        return false;
    }
    
    auto users = library.getAllUsers();
    for (User* user : users) {
        file << user->toFileFormat() << "\n";
    }
    
    file.close();
    return true;
}

// Load books from file
bool FileManager::loadBooksFromFile(Library& library) {
    ifstream file(booksFileName);
    if (!file.is_open()) {
        cout << "Aucun fichier de livres existant trouvé. Démarrage avec une bibliothèque vide.\n";
        return false;
    }
    
    string line;
    int count = 0;
    while (getline(file, line)) {
        if (!line.empty()) {
            Book book;
            book.fromFileFormat(line);
            library.addBook(book);
            count++;
        }
    }
    
    file.close();
    cout << "Chargé " << count << " livre(s) depuis le fichier.\n";
    return true;
}

// Load users from file
bool FileManager::loadUsersFromFile(Library& library) {
    ifstream file(usersFileName);
    if (!file.is_open()) {
        cout << "Aucun fichier d'utilisateurs existant trouvé. Démarrage sans utilisateurs enregistrés.\n";
        return false;
    }
    
    string line;
    int count = 0;
    while (getline(file, line)) {
        if (!line.empty()) {
            User user;
            user.fromFileFormat(line);
            library.addUser(user);
            count++;
        }
        
    }
    library.updateUserCount(count);
    
    file.close();
    cout << "Chargé " << count << " utilisateur(s) depuis le fichier.\n";
    return true;
}

// Check if file exists
bool FileManager::fileExists(const string& filename) {
    ifstream file(filename);
    return file.good();
}

// Create backup
void FileManager::createBackup() {
    if (fileExists(booksFileName)) {
        filesystem::copy_file(booksFileName, booksFileName + ".backup");
    }
    
    if (fileExists(usersFileName)) {
        filesystem::copy_file(usersFileName, usersFileName + ".backup");
    }
    
    cout << "Fichiers de sauvegarde créés.\n";
}

bool FileManager::exportToCSV(Library& library, const string& filename){
    // je verifie si le ficheir existe deja et si il est plein
    bool writeHeader = true;
    if (filesystem::exists(filename) && filesystem::file_size(filename) > 0) {
        writeHeader = false; // le fichier existe deja et est pas vide
    }

    // ca ouvre le fichier en mode append
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        cout << "erreur : Impossible d'ouvrir " << filename << " en ecriture.\n";
        return false;
    }

    // ecrit l'entete SEULEMENT si c'est un nouveau fichier
    if (writeHeader) {
        file << "Titre,Auteur,ISBN,Disponible,Emprunteur_ID\n";
    }

    // recupere tout les livres et les met en ecriture un par un
    auto books = library.getAllBooks();
    for (Book* book : books) {
        file << "\"" << book->getTitle() << "\","
             << "\"" << book->getAuthor() << "\","
             << "\"" << book->getISBN() << "\","
             << (book->getAvailability() ? "Oui" : "Non") << ","
             << "\"" << book->getBorrowerId() << "\"\n";
    }

    file.close();
    return true;



}

bool FileManager::importFromCSV(Library& library, const string& filename){
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Erreur : impossible d'ouvrir " << filename << "\n";
        return false;
    }
    string line;

    getline(file,line);

    int count = 0;

    while (getline(file,line))
    {
        if (line.empty())
        {
            continue;
        }

        stringstream ss(line);
        string titre, auteur, isbn, dispoStr, id;

        getline(ss,titre,',');
        getline(ss,auteur,',');
        getline(ss,isbn,',');
        getline(ss,dispoStr,',');
        getline(ss,id,',');


        titre.erase(remove(titre.begin(),titre.end(),'\"'),titre.end());
        auteur.erase(remove(auteur.begin(),auteur.end(),'\"'),auteur.end());
        isbn.erase(remove(isbn.begin(),isbn.end(),'\"'),isbn.end());
        id.erase(remove(id.begin(),id.end(),'\"'),id.end());


        if (!library.findBookByISBN(isbn))
        {
            Book newBook(titre,auteur,isbn);



            if (dispoStr == "Non")
            {
                newBook.setAvailability(false);
                newBook.setBorrowerId(id);
            }

            library.addBook(newBook);
            count++;
            
        }
        

        

        
    }


    file.close();

    cout << count << " livres ont ete ajoute depuis le csv. \n";

    return true;
    
    
}

