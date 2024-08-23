/**
 * @file references.cpp
 * @author Abigale Kim (abigalek)
 * @brief Tutorial code for references.
 */

// A reference in C++ is a method of creating an alias to a variable, where
// these aliases refer to the same data in memory. References are useful for
// keeping track of state, passing arguments into functions, and for general
// performance improvements. In general, it is important to understand
// references to do well in this class.

// Includes std::cout (printing) for demo purposes.
#include <iostream>

// A function that takes an int reference and adds 3 to it.
void add_three(int &a) { a = a + 3; }
// both are same
void add_three_fn(int& a) { a = a + 3; }

// passes a pointer (called reference in java) to an integer
void call_by_value(int *p) { // :1
  p = NULL;
}

// passes an integer
void call_by_value(int p) { // :2
  p = 42;
}

// passes an integer by reference
void call_by_reference(int & p) { // :3
  p = 42;
}

// this is the java style of passing references. NULL is called "null" there.
void call_by_value_special(int *p) { // :4
  *p = 10; // changes what p points to ("what p references" in java)
  // only changes the value of the parameter, but *not* of
  // the argument passed by the caller. thus it's pass-by-value:
  p = NULL;
}

/**
 * A unique_ptr does not share its pointer. It cannot be copied to another unique_ptr ,
 * passed by value to a function,
 * or used in any C++ Standard Library algorithm that requires copies to be made.
 * A unique_ptr can only be moved
 * @return
 */
int main() {
  // Take this expression. Note that b has type int& (int reference),
  // since it is a reference to a. This means that a and b both refer to the
  // same data. You can declare references by setting your variables type via
  // the single ampersand syntax.
  int x = 10;
  int &ref = x;

  int *ptr = &x;

  // As stated, if we try to print b, we will get 10.
  std::cout << "b is " << ref << std::endl;
  std::cout << "ptr is " << ptr << std::endl;

  // References can also be passed into functions. Take the function add_three,
  // which takes in an int reference and adds 3 to it. If we call this function,
  // on a, since a is being taken as a reference, then a's value in the caller
  // context will change value.
  add_three(x);
  std::cout << "a is " << x << std::endl;

  // --------------------- //
  int value = 10;
  int * pointer = &value;

  call_by_value(pointer); // :1, java primitives
  assert(pointer == &value); // pointer was copied

  call_by_value(value); // :2
  assert(value == 10); // value was copied

  call_by_reference(value); // :3
  assert(value == 42); // value was passed by reference, call by sharing?

  call_by_value_special(pointer); // :4, Java does this with object.
  // pointer was copied but what pointer references was changed.
  assert(value == 10 && pointer == &value);
  return 0;
}


/*
 *
+------+     +----+
| Name |     | x  |
+------+     +----+
|  x   | --> | 10 |
+------+

+------+     +----+
| Name |     | x  |
+------+     +----+
|  x   | --> | 10 |
|  ref | ----^
+------+

 +------+    +----+    +------+
| Name |     | x  |    | ptr  |
+------+     +----+    +------+
|  x   | --> | 10 |    | &x   | ---> address of x (let's say 0x100)
|  ref | ----^         +------+
+------+               | 0x100|
                       +------+

 *
 *
 *
 */