//
// Created by Mehul Mistry on 7/21/24.
//

#include <iostream>
#include <utility>
#include <vector>

namespace first {
  int x = 1;
}
namespace second {
  int x = 1;
}

// typedef is like your alias
// good practice to add _t which means typedef
typedef std::vector<std::pair<std::string, int>> pairlist_t;

// recommendation is use using keyword it works better with template
using pairlist_temp_t  = std::vector<std::pair<std::string, int>>;
double getTotalWithWarnings(double prices[]);
double getTotalWithSize(double prices[], int size);

/**
 *  what’s in the struct is going to be public whereas what’s in a class is going to be private.
 */
struct student {
  std::string name;
  double gpa;
  bool enrolled;
};


void mainTotalExample() {

  double prices[] = { 49.99, 123.123, 34.234};
  double total = getTotalWithWarnings(prices);

  int size = sizeof(prices)/sizeof(prices[0]);
  double total2 = getTotalWithSize(prices, size);

  std::cout << "$" << total << '\n' << "Total2: $" << total2;
}

// remember rule of 3, 5, 0
class Human {

 public:
  std::string name;
  std::string occupation;
  int age;

  Human(std::string name, int age, std::string occupation) {
	this->name = std::move(name);
	this->age = age;
	// this->occupation = occupation;
	// move helps to optimize and will move just the reference.
	this->occupation = std::move(occupation);
  }

  void eat() {
	std::cout << "eatting....";
  };

};



int main() {
  using std::cout;

  std::cout << "Hello, World!" << std::endl; // this will end it in a different line.
  std::cout << "Hello, optimized version of std::endl" << '\n';
  std::cout	<< "I like pizza!";
  cout << "name space brooo...";

  // const add some data security
  const double PI = 3.14159;
  int x = 0;
  // below is not allowed, we can create a namespace to allow.
  //int x = 1;

  // Entities can have same name if namespace is different
  std::cout << first::x << second::x;

  // if you don;t want to use first:: you can write
  using namespace second;
  std::cout << x; // this will show second x

  pairlist_t pairlist;
  pairlist_temp_t pairlist_temp;


  mainTotalExample();

  // pointers
  std::string name = "Bro";
  std::string *pName = &name;

  std::string freePizzas[5] = {"pizza1", "pizza2", "pizza5"};

  //std::string *pFreePizzas = &freePizzas; error
  // Instead of carrying all 5 pizzas you just pass the pointer.
  std::string *pFreePizzas = freePizzas; // freePizzas is already a memory address

  cout << *pName;
  cout << *pFreePizzas; // First pointer of the element.

  int *pointer = nullptr;
  int x1 = 123;

  if(pointer == nullptr) {
	pointer = &x1;
	std::cout << pointer;
  }

  // dynamic memory, it helps us assign the memory in heap not stack.

  int *pNum = NULL;
  pNum = new int;
  *pNum = 21;
  cout << "address: " << pNum << '\n';
  cout << "address: " << *pNum << '\n';

  // it's good to delete the mem ref from heap if not using.
  delete pNum;

  student student1;
  student1.name = "SpongeBob";
  student1.gpa = 3;


  Human human1;
  human1.name = "dino";
  human1.occupation = "eat people";
  human1.age = 60;

  // return 0 to tell programming language that your code executed successfully.
  return 0;
}

// the function will create pass by value, create a copy of the struct. If you want to work with the same then use &strud
void printStudent(student stud) {
  std::cout << &stud << '\n';
  std::cout << &stud << '\n';

}
/**
 *  Pointers: Sometimes it's easier to work with addresses, passing addresses compare to taking whole object and then passing.
 *  Just tell the functions where's the location to get the data instead of taking the whole data with you.
 *
 *  & address of operator
 *  * dereference operator

 * */

/**
In C++, when you pass an array to a function, you are actually passing a pointer to the first element of the array.
This means the function doesn't know the size of the array, only the address of the first element.

Why getTotalWithWarnings Fails
Pointer Decay: When you pass an array to a function, it decays to a pointer to the first element.
 This means sizeof(prices) inside the function will give you the size of the pointer, not the array.
 On most systems, this will be 8 bytes (for a 64-bit system), not the total size of the array.

Incorrect Size Calculation: Since sizeof(prices) gives the size of the pointer (typically 8 bytes),
 and sizeof(prices[0]) gives the size of a double (typically 8 bytes), the division sizeof(prices) / sizeof(prices[0]) will always be 1 (8/8),
 regardless of the actual size of the array.


 In C++, you cannot pass an array by value directly in the same way you pass primitives or objects by value. When you pass an array to a function,
 it decays into a pointer to its first element, which means the function receives a pointer, not a copy of the entire array. However,
 you can achieve pass-by-value semantics for arrays using a wrapper class or a std::vector which will create a copy of the array.


 why we need 8 bytes we can just save bool in one bit, no?
You're correct that theoretically, a bool only requires one bit to represent its two possible values (true and false). However, in practice, modern computer architectures are optimized for byte-addressable memory, meaning the smallest addressable unit of memory is one byte (8 bits). Here are some reasons why bool typically uses a whole byte:

Byte Addressability: Most computer architectures are designed to access memory at the byte level rather than the bit level.
 This means that the smallest chunk of memory that can be read or written is one byte.

Performance: Accessing individual bits would require additional instructions to mask and shift bits within a byte,
 which would complicate and slow down memory access. Using a whole byte for a bool simplifies and speeds up these operations.

Alignment and Padding: Using whole bytes can help with memory alignment and avoid potential issues with padding.
 Many architectures require data to be aligned on specific boundaries, and using a byte for each bool helps ensure proper alignment.

Standard Library and Compatibility: The C++ standard library and many system APIs expect bool to be a full byte.
 Ensuring that bool has a consistent size across different platforms and compilers makes it easier to write portable code.
 */

double getTotalWithSize(double prices[], int size) {
  //  this function has no idea of how big the array is, what's the size of the array so it would show warning.
  // so let's provide the size before calling the function.
  double total = 0;
  for(int i = 0; i < size; i++) {
	total += prices[i];
  }
  return total;
}

double getTotalWithWarnings(double prices[]) {
  //  this function has no idea of how big the array is, what's the size of the array so it would show warning.
  // so let's provide the size before calling the function.
  double total = 0;
  for(int i = 0; i < sizeof(prices)/sizeof(prices[0]); i++) {
	total += prices[i];
  }
  return total;
}


