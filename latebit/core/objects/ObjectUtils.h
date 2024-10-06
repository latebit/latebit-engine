#pragma once

#include <memory>
#include <vector>
#include "Object.h"

// Set of helper to work with object abstracting the underlying container
namespace lb {

// To prevent continuous allocations in vectors, we exeted them in chunks 
const int CHUNK_SIZE = 100;

// Remove an object from a vector
void remove(vector<Object*>& l, Object* o);

// Return true if the object is in the vector
auto contains(vector<Object*> l, Object* o) -> bool;

// Return true if the object is in the vector
auto contains(const vector<unique_ptr<Object>>& l, Object* o) -> bool;

// Insert an object in the vector if it is not already there
// If the vector is full, it will be extended by CHUNK_SIZE
void insert(vector<Object*>& l, Object* o);

// Insert an object in the vector if it is not already there
// If the vector is full, it will be extended by CHUNK_SIZE
void insert(vector<unique_ptr<Object>>& l, unique_ptr<Object> o);

}