#include <iostream>
#include <set>
#include <list>
using namespace std;

// Here are the resources I used when researching how to implement a hash table with separate chaining.
// I used these resources as a pseudocode for how to implement the hash table with a separate chaining collision solution
// Resources: https://www.geeksforgeeks.org/c-program-hashing-chaining/ , https://en.wikipedia.org/wiki/Hash_table#Separate_chaining
// https://www.geeksforgeeks.org/separate-chaining-collision-handling-technique-in-hashing/
// Used the hash PowerPoint slides, page 80, for reference and the list PowerPoint
class unordered_map{

private:

    // Declaring a size for the table. Must be constant since it is an array
    static const int tableSize = 150000;

    // This is the actual hash table. I created a pair of strings since each spot in the array should hold the books name
    // and its genres. I used the list stl in order to create a linked list when there is a collision.
    // I used the Hash Table PowerPoint slide 80 and the module 2 list PowerPoint in order to create
    // the table
    list<pair<string, string>> table[tableSize];

public:
    int hash(string key);
    void insert(string key, string value);
    set<string> get_set(string key);
    bool validInput(string key);
};

// This function gets the hash value of the book title
int unordered_map::hash(string key) {

    int index = 0;

    // This finds the total ASCII value of the book title
    for(int i = 0; i < key.size(); i++){
        index = index + int(key[i]);
    }

    // This next line will reduce the hash to get an index and the function will return that value
    return index%tableSize;
}

void unordered_map::insert(string key, string value) {

    // Finds the hash value of the book title and reduces it to an index
    int index = hash(key);
    int counter = 0;

    // Goes through the length of the table and when it reaches the specific index it will insert the title and genre
    for(int i = 0; i < tableSize; i++){
        if(counter == index){
            table[i].push_back(make_pair(key,value));
        }
        counter++;
    }

}

set<string> unordered_map::get_set(string key) {
    set<string> s1;

    // This hashes the key in order to find what index the word is at
    int index = hash(key);

    // A for loop then runs at that specific index that finds where the key is located in the linked list. Once found
    // the genres of that book will be added into a set
    for(auto it = table[index].begin(); it != table[index].end(); ++it){
        if(it->first == key){
            s1.insert(it->second);
        }
    }
    return s1;
}

bool unordered_map::validInput(string key) {

    // This will find the hash value of the key in order to get its index value
    int index = hash(key);

    // Based on the index value, the for loop will run for the length of the linked list in order to see
    // whether the key exists
    for(auto it = table[index].begin(); it != table[index].end(); ++it){
        if(it->first == key){
            return true;
        }
    }

    return false;
}
