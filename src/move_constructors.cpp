/**
 * @file move_constructors.cpp
 * @author Abigale Kim (abigalek)
 * @brief Tutorial code for move constructors and move assignment operators.
 */

// Move constructors and move assignment operators are methods implemented
// inside of classes to effectively move resources from one object to the
// other, typically using std::move. These class methods take in another
// object of the same type, and move its resources to the instance
// where the method is called. In this file, we will explore implementing
// and using move constructors and move assignment operators.

// Includes std::cout (printing) for demo purposes.
#include <iostream>
// Includes the utility header for std::move.
#include <utility>
// Includes the C++ string library.
#include <string>
// Includes the header for uint32_t.
#include <cstdint>
// Includes the header for std::vector. We'll cover vectors more in
// containers.cpp, but what suffices to know for now is that vectors are
// essentially dynamic arrays, and the type std::vector<std::string> is an array
// of strings. Mainly, vectors take up a non-negligible amount of memory, and
// are here to show the performance benefits of using std::move.
#include <vector>

// Basic person class, with an implemented move constructor and move assignment
// operator, and a deleted copy constructor and copy assignment operator. This
// means that once an Person object is instantiated, it cannot be copied. It
// must be moved from one lvalue to another. Classes without copy operators are
// useful when it is imperative to only have one defined instance of a class.
// For instance, if a class manages a dynamically allocated memory block, then
// creating more than one instance of this class, without proper handling, can
// result in double deletion or memory leaks.
class Person {

 private:
  uint32_t age_;
  std::vector<std::string> nicknames_;
  // Keeping track of whether an object's data is valid, i.e. whether
  // all of its data has been moved to another instance.
  bool valid_;

 public:
  Person() : age_(0), nicknames_({}), valid_(true) {}

  // Keep in mind that this constructor takes in a std::vector<std::string>
  // rvalue. This makes the constructor more efficient because it doesn't deep
  // copy the vector instance when constructing the person object.
  Person(uint32_t age, std::vector<std::string> &&nicknames)
	  : age_(age), nicknames_(std::move(nicknames)), valid_(true) {}

  // Move constructor for class Person. It takes in a rvalue with type Person,
  // and moves the contents of the rvalue passed in as an argument to this
  // Person object instance. Note the usage of std::move. In order to ensure
  // that nicknames in object person is moved, and not deep copied, we use
  // std::move. std::move will cast the lvalue person.nicknames_ to an rvalue,
  // which represents the value itself. Also note that I don't call std::move
  // on the age_ field. Since it's an integer type, it's too small to incur a
  // significant copying cost. Generally, for numeric types, it's okay to copy
  // them, but for other types, such as strings and object types, one should
  // move the class instance unless copying is necessary.
  Person(Person &&person)
	  : age_(person.age_), nicknames_(std::move(person.nicknames_)),
		valid_(true) {
	std::cout << "Calling the move constructor for class Person.\n";
	// The moved object's validity tag is set to false.
	person.valid_ = false;
  }

  // Move assignment operator for class Person.
  Person &operator=(Person &&other) {
	std::cout << "Calling the move assignment operator for class Person.\n";
	age_ = other.age_;
	nicknames_ = std::move(other.nicknames_);
	valid_ = true;

	// The moved object's validity tag is set to false.
	other.valid_ = false;
	return *this;
  }

  // We delete the copy constructor and the copy assignment operator,
  // so this class cannot be copy-constructed.
  Person(const Person &) = delete;
  Person &operator=(const Person &) = delete;

  uint32_t GetAge() { return age_; }

  // This ampersand at the return type implies that we return a reference
  // to the string at nicknames_[i]. This also implies that we don't copy
  // the resulting string, and the memory address this returns under the
  // hood is actually the one pointing to the nicknames_ vector's memory.
  std::string &GetNicknameAtI(size_t i) { return nicknames_[i]; }

  void PrintValid() {
	if (valid_) {
	  std::cout << "Object is valid." << std::endl;
	} else {
	  std::cout << "Object is invalid." << std::endl;
	}
  }
};

class Holder {
 private:
  int *m_data;
  size_t m_size;

 public:
  Holder(int size) {
	m_data = new int[size];
	m_size = size;
  }

  ~Holder() {
	delete[] m_data;
  }

  // const will make sure that you are not updating the other in copy constructor.
  Holder(const Holder &other) {
	std::cout << "I'm triggered, CopyConstructor" << std::endl;
	// So it's good to put const otherwise not needed.
	//other.m_size = 10; // someone can accidental update if you don't put constant
	m_data = new int[other.m_size];
	std::copy(other.m_data, other.m_data + other.m_size, m_data);
	m_size = other.m_size;
  }

  // const will make sure that you are not updating the other in copy constructor.
  /**
   * First of all a little protection against self-assignment (1). Then, since we are replacing the content of this class with another one,
   * let's wipe out the current data (2).
   * What's left is just the same code we wrote in the copy constructor. By convention a reference to this class is returned (3).
   *
   The key point of the copy constructor and the assignment operator is that they both receive a const reference to an object in
   input and make a copy out of it for the class they belong to.
   The object in input, being a constant reference, is of course left untouched.
   *
   * You are copying the content from one object to another, any smarter or optimized way of doing it?
   */
  Holder &operator=(const Holder &other) noexcept {
	if (this == &other) return *this;// (1)
	delete[] m_data;                 // (2)
	m_data = new int[other.m_size];
	std::copy(other.m_data, other.m_data + other.m_size, m_data);
	m_size = other.m_size;
	return *this;//
  }

  // Move constructor
  Holder(Holder &&other) noexcept {
	std::cout << "I'm triggered, MoveConstuctor" << std::endl;

	m_data = other.m_data;// (1)
	m_size = other.m_size;
	other.m_data = nullptr;// (2)
	other.m_size = 0;
  }

  Holder &operator=(Holder &&other)// <-- rvalue reference in input
  {
	if (this == &other) return *this;

	delete[] m_data;// (1)

	m_data = other.m_data;// (2)
	m_size = other.m_size;

	other.m_data = nullptr;// (3)
	other.m_size = 0;

	return *this;
  }
};

/**
 * https://www.internalpointers.com/post/c-rvalue-references-and-move-semantics-beginners
 * We are returning Holder not Holder& ---> which returns object by value of Holder, complier has to creat a temporary - yet fully-fledged object(rvalue)
 * Now our holder is a heavy-object due to its internal memory allocation, which is very expensive task.
 * returning such things by value with our current class design would trigger multiple expensive memory allocations, which is rarely a great idea
 *
 * @param size
 * @return
 */
Holder createHolder(int size) {
  return Holder(size);
}

int main() {
  // Let's see how move constructors and move assignment operators can be
  // implemented and used in a class. First, we create an instance of the class
  // Person. Note that the object andy is a valid object.
  Person andy(15445, {"andy", "pavlo"});
  std::cout << "Printing andy's validity: ";
  andy.PrintValid();

  // To move the contents of the andy object to another object, we can use
  // std::move in a couple ways. This method calls the move assignment operator.
  Person andy1;
  andy1 = std::move(andy);

  // Note that andy1 is valid, while andy is not a valid object.
  std::cout << "Printing andy1's validity: ";
  andy1.PrintValid();
  std::cout << "Printing andy's validity: ";
  andy.PrintValid();

  // This method calls the move constructor. After this operation, the contents
  // of the original andy object have moved to andy1, then moved to andy2. The
  // andy and andy1 lvalues are effectively defunct (and should not be used,
  // unless they are re-initialized).
  Person andy2(std::move(andy1));

  // Note that andy2 is valid, while andy1 is not a valid object.
  std::cout << "Printing andy2's validity: ";
  andy2.PrintValid();
  std::cout << "Printing andy1's validity: ";
  andy1.PrintValid();

  // However, note that because the copy assignment operator is deleted, this code
  // will not compile. The first line of this code constructs a new object via the
  // default constructor, and the second line invokes the copy assignment operator
  // to re-initialize andy3 with the deep-copied contents of andy2. Try uncommenting
  // these lines of code to see the resulting compiler errors.
  // Person andy3;
  // andy3 = andy2;

  // Because the copy constructor is deleted, this code will not compile. Try
  // uncommenting this code to see the resulting compiler errors.
  // Person andy4(andy2);

  // If no copy construcutor then all the below will point to the same memory address
  Holder h1(5);
  Holder h2 = h1;// copy constructor
  Holder h3(h1);

  // Assignment operator, used to replace an existing object with another existing object.
  Holder h4(10);// Copy constructor
  Holder h5(20);
  h4 = h5;// when assignment operator is not implemented it just copy the reference and both points to the same reference
  h5 = h4;

  // A temporary object coming out from createHolder() is passed to the copy constructor.
  // According to our current design, the copy constructor allocates its own m_data pointer by copying the data from the temporary object.
  // Two expensive memory allocations:
  // 	a) during the creation of the temporary,
  // 	b) during the actual object copy-construct operation.
  //
  //The same copy procedure occurs within the assignment operator:
  Holder h = createHolder(100);// Let's say h is pointed to 0x100 memory

  // Here when you reassigned in old design it copies the data to from temp location which is created by createHolder function to 0x100
  // but with move constructor h location is now pointing to the temp location which is created by createHolder function.
  // So you just move the pointer.
  h = createHolder(50);// pointing to 0x200 now

  // When you call
  h = h3;// this will not trigger move assignment because compiler knows you have passed lvalue not rvalue.
  // You need to pass rvalue to trigger move semantics. createHolder is returning rvalue.

  Holder h6 = createHolder(2000);// move constructor (rvalue in input) (1)

  // Now you can think that move constructor helps to change the ownership of the pointers.
  // When you run the code you noticed move constructor is never getting called.
  // RVO --> return value optimization.

  // That's right. If you run the last snippet above you will notice how the move constructor does not get called during (1).
  // The regular constructor is called instead: this is due to a trick called Return Value Optimization (RVO).
  // Modern compilers are able to detect that you are returning an object by value, and they apply a sort of return shortcut
  // to avoid useless copies.

  // Why should I care implementing move semantics if the RVO does its optimization job by default?
  // RVO is only about return values (output), not function parameters (input).
  // There are many places where you may pass movable objects as input parameters,
  // which would make the move constructor and the move assignment operator come into play, if implemented.
  // The most important one: the Standard Library. During the upgrade to C++11 all the algorithms and containers in there were extended
  // to support move semantics. So if you use the Standard Library with classes that follow the Rule of Five you will
  // gain an important optimization boost.

  // Can I move lvalues?
  Holder h11(1000);
  Holder h22(h11);// this will not work, since h2 receives an lvalue in input, the copy cont. is being triggered.
  // We need to force the move cont. on h2 in order to make it steal from h1.

  // When you move h11 is now empty, pointing to NULL ptr
  Holder h33(std::move(h11));

  // add noexcept to move and copy so that no one can throw exceptions in the move semantics
  return 0;
}
