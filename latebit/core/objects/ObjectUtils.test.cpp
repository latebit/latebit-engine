#include "ObjectUtils.h"
#include "Object.h"
#include <memory>
#include <vector>
#include "test/lib/test.h"

using namespace std;

auto main() -> int {
    test("remove", []() {
        vector<Object*> objects;
        Object obj1, obj2, obj3;
        objects.push_back(&obj1);
        objects.push_back(&obj2);
        objects.push_back(&obj3);

        remove(objects, &obj2);

        assertEq("size after removal", objects.size(), 2);
        assert("first object", objects[0] == &obj1);
        assert("second object", objects[1] == &obj3);
    });

    test("contains (Object*)", []() {
        vector<Object*> objects;
        Object obj1, obj2;
        objects.push_back(&obj1);

        assert("contains obj1", contains(objects, &obj1));
        assert("does not contain obj2", !contains(objects, &obj2));
    });

    test("contains (unique_ptr<Object>)", []() {
        vector<unique_ptr<Object>> objects;
        auto obj1 = make_unique<Object>();
        auto ptr1 = obj1.get();
        auto obj2 = make_unique<Object>();
        auto ptr2 = obj2.get();
        objects.push_back(std::move(obj1));

        assert("contains obj1", contains(objects, ptr1));
        assert("does not contain obj2", !contains(objects, ptr2));
    });

    test("insert (Object*)", []() {
        vector<Object*> objects;
        Object obj1, obj2;

        insert(objects, &obj1);
        insert(objects, &obj2);
        insert(objects, &obj1); // Duplicate, should not be inserted

        assertEq("size after insertions", objects.size(), 2);
        assert("first object", objects[0] == &obj1);
        assert("second object", objects[1] == &obj2);

        vector<unique_ptr<Object>> temp;
        for (int i = 0; i < CHUNK_SIZE; i++) {
          auto object = make_unique<Object>();
          insert(objects, object.get());
          temp.push_back(std::move(object));
        }

        assertEq("extends the list by chunk", objects.size(), CHUNK_SIZE + 2);
    });

    test("insert (unique_ptr<Object>)", []() {
        vector<unique_ptr<Object>> objects;
        auto obj1 = make_unique<Object>();
        auto obj2 = make_unique<Object>();
        auto id1 = obj1->getId();
        auto id2 = obj2->getId();


        insert(objects, std::move(obj1));
        insert(objects, std::move(obj2));

        assertEq("size after insertions", objects.size(), 2);
        assertEq("first object id", objects[0]->getId(), id1);
        assertEq("second object id", objects[1]->getId(), id2);
    });

    return report();
}