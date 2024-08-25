//
// Created by Mehul Mistry on 8/25/24.
//

// https://www.internalpointers.com/post/constant-pointers-vs-pointer-constants-c-and-c

#include <iostream>
struct Object { int x; };

int main() {
  // Both the pointer and the object are writable. You can modify the object, e.g.
  // changing its x value and you can also modify the pointer, e.g. assign it a new object:
  Object* object1 = new Object{1};
  Object* object2 = new Object{2};

  Object* object_ptr = object1;
  object_ptr = object2;          // Modify pointer, OK
  object_ptr->x = 40;            // Modify object, OK

  const Object* object_ptr1 = object1;
  object_ptr1 = object2;                // Modify pointer, OK
  //object_ptr1->x = 40;                  // Modify object, ERROR

  Object* const object_ptr2 = object1;
  //object_ptr2 = object2;                // Modify pointer, ERROR
  object_ptr2->x = 40;                  // Modify object, OK

  const Object* const object_ptr3 = object1;  // Const pointer to const object
  //object_ptr3 = object2;                      // Modify pointer, ERROR
  //object_ptr3->x = 40;                        // Modify object, ERROR
  std::cout << object_ptr->x << "\n";        // Read object, OK
}