/**
 * Original code from http://stackoverflow.com/questions/7184183/c-templates-with-interface-objects
 *
 * See also: http://stackoverflow.com/questions/2203388/c-templates-polymorphism
 */
#include <iostream>
#include <type_traits>
using namespace std;

/**
 * Interface for sortable Object
 */
class Sortable {
public:
	virtual ~Sortable() {};
	virtual bool operator<(Sortable& arg)=0;
};


/**
 * It's a kind of magic..
 */
template<typename T, class Enable = void>
class SortedLinkedList;

struct Empty {
};

template<typename T>
struct base_for_group_t {
	typedef SortedLinkedList<Sortable> type;
};

template<>
struct base_for_group_t<Sortable> {
	typedef Empty type;
};


/**
 * My Simple Recursive Bubble Sorted Linked List..
 * Any application using this List must manage the memory allocation and pointer managing.
 * Objects are not copied, only referenced.
 */
template<typename T>
class SortedLinkedList<T, typename enable_if<is_base_of<Sortable, T>::value>::type>
		: public base_for_group_t<T>::type{

	T * element;
	SortedLinkedList* nextNode;

public:

	SortedLinkedList() {
		element = 0;
		nextNode = 0;
	}

	void insert(T * e) {
		if (element == 0) {
			element = e;
			nextNode = new SortedLinkedList<T>();
		} else if (*e < *element) {
			nextNode->insert(element);
			element = e;
		} else {
			nextNode->insert(e);
		}
	}
	T* get(int pos) {
		if (pos == 0) {
			return element;
		} else if (nextNode) {
			pos--;
			return nextNode->get(pos);
		}
	}

};
//END OF MAGIC



/*
 * Declaration of a sortable Object
 */
class MyInteger: public Sortable {
	int val;
public:
	MyInteger() {
		val = 0;
	}
	MyInteger(int i) {
		val = i;
	}
	int value() {
		return this->val;
	}

	virtual bool operator<(Sortable& arg) {
		return this->value() < ((MyInteger&)arg).value();
	}

	virtual ~MyInteger(){};
};

/*
 * Declaration of Non sortable Object
 */
class SomeClass {
};

int main(int argc, char **argv) {
	SortedLinkedList<Sortable> *g = 0;

	g = new SortedLinkedList<MyInteger>();

	MyInteger* a = new MyInteger(1);
	MyInteger* b = new MyInteger(10);
	MyInteger* c = new MyInteger(5);
	MyInteger* d = new MyInteger(3);
	g->insert(a);
	g->insert(b);
	g->insert(c);
	g->insert(d);

	for (int i = 0; i < 4; ++ i) {
		int out = ((MyInteger*)g->get(i))->value();
		std::cout << out << std::endl;
	}
	//	g = new SortableLinkedList<SomeClass>(); //Will not work
}
