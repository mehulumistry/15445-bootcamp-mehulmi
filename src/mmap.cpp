//
// Created by Mehul Mistry on 7/31/24.
//


#include <vector>
#include <mutex>
#include <iostream>
#include <sys/mman.h> // For mmap()
#include <sys/stat.h> // For struct stat
#include <fcntl.h>    // For file control options (O_RDONLY)
#include <unistd.h>   // For close()
#include <cstring>    // For memcpy()

class Complex {

 private:
  double real_;
  double imag_;

 public:
  Complex(double real, double imag) : real_(real), imag_(imag) {
	// shortcut to directly initialize private members. real_(real), imag_(imag)
  }

// You cannot edit variables in readonly functions
//  Complex operator+(const Complex &other) const {
//    // return Complex(real_ + other.real_, imag_ + other.imag_);
//	return {real_ + other.real_, imag_ + other.imag_};
//  }

  int max(int a, int b);
  // Mostly used in generics when you don't know the type to return.
  auto maxNewWay(int a, int b) -> int;


  auto operator+(const Complex &other) -> Complex {
	// return Complex(real_ + other.real_, imag_ + other.imag_);
	real_= real_ + 5;
	return {real_ + other.real_, imag_ + other.imag_};
  }

  // const member functions
  void Println() const {
	std::cout << real_ << " + " << imag_ << "i" << std::endl;
  }
// When you declare const the function becomes read-only you cannot edit it.
//  int getRealNum() const {
//	real_ = real_ + 5;
//	return real_;
//  }

};


template <typename T>
class CustomArray {

 private:
  int size_;
  T *data_;

 public:
  CustomArray(int size): size_(size), data_(new T[size_]) {}

  ~CustomArray() {delete[] data_;}

  auto Size() const -> int {
	return size_;
  }
  // You mentioned auto otherwise if you need to return a reference use int&
  auto operator[](int index) -> T &{
	if (index < 0 || index >= size_) {
	  throw std::out_of_range("Index out of bounds");
	}
	return data_[index];
  }

  // Subscript operator for modifying the element, not using auto, if using auto you need to add trailing type. Add
  // type at the end. -> T {}. If you want to return reference then use -> T &{}
  int& operator[](size_t index) {
	if (index >= size_) {
	  throw std::out_of_range("Index out of range");
	}
	return data_[index]; // Returns a reference to allow modification
  }


  // Subscript operator to return a copy of the element
  int operator[](size_t index) const {
	if (index >= size_) {
	  throw std::out_of_range("Index out of bounds");
	}
	return data_[index]; // Returns a copy of the element
  }

  /**
   * In C++, objects are passed by value by default, which means a copy of the object is made unless
   * you explicitly use references or pointers to avoid copying.

   In Java, objects are passed by reference value, meaning that the reference (or address) to the object is copied,
   but the object itself is not cloned. Changes to the object through one reference will be reflected in others.
   */

};


/*

C++ is default pass by value and does shallow copy if copy constructor is not provided in case of objects/heap based.

If the copy constructor is not explicitly defined in C++, the compiler provides a default copy constructor that performs a shallow copy
of the object's data members. This means that each data member is copied directly from the source object to the target object.

Here's what happens when you do not define a copy constructor:

Default Copy Constructor
Shallow Copy: The compiler-generated copy constructor performs a member-wise copy of the object's non-static data members.
Pointers and Dynamic Memory: If your class contains pointers to dynamic memory, the default copy constructor
copies the pointer values, leading to potential issues such as double deletion and dangling pointers.
 */
class GeneralClass {
 public:
  int value;
  GeneralClass(int value): value(value) {}
};

void modifyObjectByValue(GeneralClass gc) {
  gc.value = 20; // Only modifies the local copy
}

// If you want to modify the actual object value then use reference
void modifyObjectByReference(GeneralClass &gc) {
  gc.value = 20; // Modifies the original object
}

// Function that takes a pointer to an object
// Use this when you want to reassign pointers
void modifyObjectByPointer(GeneralClass *obj) {
  obj->value = 20; // Modifies the original object
}

/**
 * Use References: When you want a cleaner syntax, guaranteed non-nullability, or to pass large objects efficiently (especially with const).
Use Pointers: When you need nullability, dynamic memory management, array manipulation, or the ability to reassign the parameter within the function.
 *
 * @param obj
 */
void reassignPointer(GeneralClass *&obj) {
  obj = new GeneralClass(4); // Change where obj points
}


int main() {

  /**
   * As we discussed in the previous articles.
   * Till the string length of 15 (16 -1 null char), we store the string in the stack, beyond 15 we move the string to the heap.
   * So the default capacity is 15
   */
  const char *filePath = "/Users/mehulmistry/CLionProjects/15445-bootcamp/src/example.txt"; // File to be memory-mapped
  int fileDescriptor = open(filePath, O_RDWR);

  if(fileDescriptor < 0) {
	std::cerr << "Error opening file!" << std::endl;
	return 1;
  }

  struct stat fileInfo;
  if(fstat(fileDescriptor, &fileInfo) < 0) {
	std::cerr << "Error getting file size!" << std::endl;
	close(fileDescriptor);
	return 1;
  }

  size_t filesize = fileInfo.st_size;

  //Memory map file
  char *mappedData = static_cast<char*>(mmap(nullptr, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0));

  if(mappedData == MAP_FAILED) {
	std::cerr << "Error mapping file!" << std::endl;
	close(fileDescriptor);
	return 1;
  }

  // Read from memory mapped area
  std::cout << "Original file content: " << std::endl;
  std::cout << mappedData << std::endl;

  const char *newContent = "This is new content";
  memcpy(mappedData, newContent, strlen(newContent)); // Overwrite with new content

  // Unmap and close the file
  if (munmap(mappedData, filesize) < 0) {
	std::cerr << "Err unmapping file!" << std::endl;
  }
  close(fileDescriptor);
  std::cout << "File content updated!" << std::endl;


  // CustomArray
  CustomArray<int> arr(5);

  for(int i = 0; i < arr.Size(); ++i) {
	arr[i] = i * 10;
  }

  // Pass by value, reference, pointer

  GeneralClass myObj(10);
  modifyObjectByPointer(&myObj);

  modifyObjectByReference(myObj);
  modifyObjectByValue(myObj);

  std::cout << myObj.value << std::endl; // Outputs 20


  // brace initialization
  int x { 5 };
  Complex c {3.0, 4.0};
  std::vector<int> vec {1, 2, 3, 4, 5};

  // You can pass this string view, read only of string.
  std::string_view s{"Hello world"};

  float i = 21.4;
  int x1 , y;
  x1 = i;      //Type conversion from float to int

  // use dynamic_cast for classes.
  y = static_cast<int>(i);
  std::cout<< x <<"\n" << y;


  return 0;
}














