## Yassine Satrallah

# Construire le projet



Vous pouvez utiliser un dev container de base C++ de VScode.
Le projet utilise cmake, pensez à l'inclure dans votre dev container.

Voici les lignes de commandes pour compiler le projet:
```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

# Répertoire data

Il contient 2 fichiers `books.txt`et `users.txt` que vous pouvez utilisez pour tester votre code.
Pour ca il suffit de donner chemin vers le repertoire data avec l'application `bibliotheque -d <chemin verrs data>`

# Fonctionnalités implémentées


## Interface et expérience utilisateur :

### Numérotation automatique des ID utilisateurs :

Je génère un premier le nouvel id et regardant quel est l'id le plus gros dans mon vecteur et en lui donnant la bonne mise en forme. Suite a cela, je crée le nouvel utilisateur et je le rajoute dans ma librairie.

### Afficher le nom d’utilisateur plutôt que l’id dans l’affichage des livres :
Je surcharge ma méthode `ToString*()` afin d'avoir une sans paramètre et une avec un objet `Library`. Ensuite, je vérifie pour chaque livre lors de leur affichage si ils sont disponible grace leur attribut `isAvailable`, si il l'est, on leur donne le `toString()` sans paramètres. Dans le cas contraire, on lui donne le `ToString()` avec le paramètre qui va, par la suite, trouver le nom de l'utilisateur a partir des méthodes `findUserById()` et `getName()`.
>Je sais maintenant que j'ai fait cette fonctionnalité pour rien car c'est une par section donc j'en ai fait une supplémentaire un peu plus bas :)

## Gestion des données

### Import Export CSV :
#### Import :
Je verifie si le fichier existe avant tout, si oui, je prends chaque ligne, j'extrais chaque donnée séparés par une virgule en utilisant `getline()` et les rentre dans leur variable respective. Je retire ensuite les `""` qui entoure les données de type `string` en utilisant les méthodes `erase` et `remove`. Ensuite, je cherche dans la librairie si chaque livre existe, si oui, on l'ignore, si non on crée un nouveau livre de type `Book` et on lui donne les nouveaux attributs pour finalement le rentrer dans la librarie afin d'y rentrer uniquement les nouveaux livre et éviter les doublons.

#### Export : 
En premier lieu, je vérifie si le fichier existe et si il est plein. Si oui, je sais qu'il ne faut pas faire de header. Suite a cela, on ouvre le fichier en mode append pour ajouter des données. Bien sur, dépendamment du cas, on écrit le header ou non. Finalement, on va chercher tout les livres et pour chacun, sauvegarde les données ligne par ligne, séparé par une `,` 

## Corriger un bug : 

### Si on lance sans –data-dir, saveLibraryData essaie d’écrire dans "" et échoue silencieusement : 

Ma solution à ce problème fut de vérifier la variable `dataDir` afin de ne laisser le programme continuer uniquement si elle n'était pas vide.


# Question 1 :

```cpp

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

```

### `filesystem` : 
filesystem est un peu comme l'exporateur de fichier car il permet de manipuler les fichier du système sans ouvrir les fichiers directement contrairement a ifstream ou ofstream



### `file_size` :
ici cette méthode donne directement la taille en octet du fichier choisi, dans mon cas je veux juste savoir si il est vide donc je demande si il est supérieur a 0 ou pas.



### ``ofstream file(filename, ios::app)`` : 
ofstream permet d'écrire directement dans fichier, en l'occurance ici ce serait `filename` et ``ios:app`` dit simplement que l'on veut ``append`` soit, rajouter les données en plus de celles qui sont déjà présente sans écraser.



# Question 2 : Options de développement possible : 
Éventuellement, cette bibliothèque contiendra beaucoup plus de livre et il est impossible de conserver ces données dans un simple fichier CSV ou txt car charger 10 millions d'objet dans la ram va forcément faire planter l'ordinateur. Ainsi, je suggèrerais de créer une base de donnée pour ces livres. Cela permetterait, d'avoir des données plus sécure avec la bonne architecture en plus de pouvoir faire de la manipulation de données plus facilement avec un système comme PostgreSQL. Pour l'implémenter il suffirait d'intégrer la bibliothèque officiel (API pour C++) qui se nomme `libpqxx` pour pouvoir se connecter au serveur a l'aide du réseau. Comme on a bien séparé les différents aspects du programme, il serait assez simple de faire en sorte que la sauvegarde se fasse sur la BDD ou bien que la recherche d'un ID soit une requête qui retourne le ID de l'utilisateur par exemple.