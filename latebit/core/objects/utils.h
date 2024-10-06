#pragma once

#include <memory>
#include <vector>
#include "Object.h"

// Set of helper to work with object abstracting the underlying container
namespace lb {

// To prevent continuous allocations in vectors, we exeted them in chunks 
const int CHUNK_SIZE = 100;

void remove(vector<Object*>& l, Object* o);
auto contains(vector<Object*> l, Object* o) -> bool;
auto contains(const vector<unique_ptr<Object>>& l, Object* o) -> bool;
void insert(vector<Object*>& l, Object* o);
void insert(vector<unique_ptr<Object>>& l, unique_ptr<Object> o);

}