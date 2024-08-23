//
// Created by Mehul Mistry on 7/30/24.
//
#include <iostream>
#include <utility>
#include <vector>

/**
 * Reference Counting: We keep track of how many objects are using the same data.
 * Detaching: When you want to modify the data, you create a new copy if other objects are still using the original data.
 * Constructors and Destructors: Used for initializing and cleaning up resources.
 *
 * @return
 */

/**
 * StringData holds the actual string and a reference count.
 */
class CowString {
 private:
  struct StringData {
	char* data;      // Pointer to the actual string data
	int refCount;    // Number of references to this data
  };

  StringData* strData; // Pointer to our StringData structure

  void detach(); // Helper function to handle data copying

 public:
  CowString(const char* str = ""); // Constructor
  CowString(const CowString& other); // Copy Constructor
  ~CowString(); // Destructor

  CowString& operator=(const CowString& other); // Assignment Operator

  char& operator[](int index); // Subscript Operator (modifiable)
  const char& operator[](int index) const; // Subscript Operator (read-only)

  const char* c_str() const; // Function to get the C-style string
};

CowString::CowString(const char* str) {
  strData = new StringData; // Allocate memory for string data
  strData->data = new char[strlen(str) + 1];
  strcpy(strData->data, str); // Copy the string data
  strData->refCount = 1; // Initialize reference count to 1
}

CowString::~CowString() {
  if(--strData->refCount == 0) {
	delete[] strData->data;
	delete strData;
  }
}
CowString::CowString(const CowString& other) {
	strData = other.strData;
	++strData->refCount;
}
CowString &CowString::operator=(const CowString &other) {
  if (this != &other) { // Check for self-assignment
	if (--strData->refCount == 0) { // Decrement refCount and check if it is 0
	  delete[] strData->data; // Delete the string data
	  delete strData; // Delete the StringData structure
	}

	strData = other.strData; // Share the new data
	++strData->refCount; // Increment the reference count
  }

  return *this;
}

void CowString::detach() {
  if (strData->refCount > 1) { // If the data is shared
	--strData->refCount; // Decrement the reference count

	StringData* newData = new StringData; // Create new StringData
	newData->data = new char[strlen(strData->data) + 1]; // Allocate new memory
	strcpy(newData->data, strData->data); // Copy the data
	newData->refCount = 1; // Initialize reference count

	strData = newData; // Point to the new data
  }
}

/**
 *
 * Modifiable Operator: Calls detach to ensure unique data ownership before modification.
 * Read-only Operator: Provides access without modification.
 * @param index
 * @return
 */
char& CowString::operator[](int index) {
  detach(); // Ensure unique ownership before modifying
  return strData->data[index]; // Return reference to character
}

const char& CowString::operator[](int index) const {
  return strData->data[index]; // Return read-only reference
}

const char* CowString::c_str() const {
  return strData->data; // Return the string data
}


int main() {
  // Copy on write, only copy when write
  CowString s1("Hello");
  CowString s2 = s1;  // Share data with s1

  std::cout << "s1: " << s1.c_str() << std::endl;
  std::cout << "s2: " << s2.c_str() << std::endl;

  s2[1] = 'a';  // Detach and modify s2

  std::cout << "After modification:" << std::endl;
  std::cout << "s1: " << s1.c_str() << std::endl;
  std::cout << "s2: " << s2.c_str() << std::endl;

  return 0;


}
