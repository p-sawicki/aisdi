#include <iostream>
#include <chrono>
#include <utility>
#include <map>
#include <fstream>
#include <string>
#include <cassert>

/*!
 *  Narzedzie do zliczania czasu
 *
 *  Sposob uzycia:
 *
 *  {
 *    Benchmark<std::chrono::nanoseconds> b;
 *    // kod do zbadania
 *    size_t elapsed = b.elapsed();
 *  }
 *
 *  lub
 *
 *  {
 *    Benchmark<std::chrono::milliseconds> b(true);
 *    // kod do zbadania
 *  } // obiekt wypisze wartosc czasu w podanych jednostkach na stderr
 */
template<typename D = std::chrono::microseconds>
class Benchmark {
public:

	Benchmark(bool printOnExit = false) : m_print(printOnExit) {
		start = std::chrono::high_resolution_clock::now();
	}
	typename D::rep elapsed() const {
		auto end = std::chrono::high_resolution_clock::now();
		auto result = std::chrono::duration_cast<D>(end - start);
		return result.count();
	}
	~Benchmark() {
		auto result = elapsed();
		if (m_print)
		{
			std::cerr << "Czas: " << result << "\n";
		}
	}
private:
	std::chrono::high_resolution_clock::time_point start;
	bool m_print = true;
};




/*
 * Slownik
 *
 */
template<typename KeyType, typename ValueType>
class TreeMap
{
private:
	struct Node {
		Node(KeyType tkey, ValueType tvalue) : left(nullptr), right(nullptr), balance(0), parent(nullptr), key(tkey), value(tvalue) {}
		Node* left;
		Node* right;
		Node* parent;
		KeyType key;
		ValueType value;
		int balance;
	};

	Node* root;
	int treeSize;

	bool _check(Node *r) {
		if (r->left && !_check(r->left))
			return false;
		if (r->right && !_check(r->right))
			return false;
		if (r->left && r->right)
			return (r->left->key < r->key && r->right->key > r->key);
		if (r->left)
			return r->left->key < r->key;
		if (r->right)
			return r->right->key > r->key;
		return true;
	}
	int _height(Node *r) {
		int leftHeight = 0;
		int rightHeight = 0;
		if (r->left)
			leftHeight = _height(r->left);
		if (r->right)
			rightHeight = _height(r->right);
		int balance = rightHeight - leftHeight;
		assert(balance == r->balance);
		assert(balance > -2 && balance < 2);
		return leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
	}
public:
	using key_type = KeyType;
	using mapped_type = ValueType;
	using value_type = std::pair<const key_type, mapped_type>;

	TreeMap() : treeSize(0), root(nullptr) {};    // konstruktor trywialny
	/*!
	 * true jezeli slownik jest pusty
	 */
	bool isEmpty() const
	{
		return root == nullptr;
	}
	/*!
	 * dodaje wpis do slownika
	 */
	inline bool isLeftChild(Node *toCheck) {
		return toCheck->key < toCheck->parent->key;
	}

	Node *rotateLeft(Node *toRotate) {
		Node *oldParent = toRotate->parent;
		toRotate->parent = oldParent->parent;
		if (oldParent) {
			oldParent->parent = toRotate;
			oldParent->right = toRotate->left;
			oldParent->balance = 0;
		}
		if (toRotate->left)
			toRotate->left->parent = oldParent;
		toRotate->left = oldParent;
		toRotate->balance = 0;
		return toRotate;
	}
	Node *rotateRight(Node *toRotate) {
		Node *oldParent = toRotate->parent;
		toRotate->parent = oldParent->parent;
		if (oldParent) {
			oldParent->parent = toRotate;
			oldParent->left = toRotate->right;
			oldParent->balance = 0;
		}
		if (toRotate->right)
			toRotate->right->parent = oldParent;
		toRotate->right = oldParent;
		toRotate->balance = 0;
		return toRotate;
	}
	Node *rotateRightLeft(Node *toRotate) {
		Node *oldParent = toRotate->parent;
		Node *left = toRotate->left;
		toRotate->left = left->right;
		if (left->right)
			left->right->parent = toRotate;
		left->right = toRotate;
		toRotate->parent = left;
		oldParent->right = left->left;
		if (left->left)
			left->left->parent = oldParent;
		left->left = oldParent;
		oldParent->parent = left;
		if (left->balance > 0) {
			if (oldParent)
				oldParent->balance = -1;
			toRotate->balance = 0;
		}
		else {
			if (left->balance == 0) {
				if (oldParent)
					oldParent->balance = 0;
				toRotate->balance = 0;
			}
			else {
				if (oldParent)
					oldParent->balance = 0;
				toRotate->balance = 1;
			}
		}
		left->balance = 0;
		return left;
	}
	Node *rotateLeftRight(Node *toRotate) {
		Node *oldParent = toRotate->parent;
		Node *right = toRotate->right;
		toRotate->right = right->left;
		if (right->left)
			right->left->parent = toRotate;
		right->left = toRotate;
		toRotate->parent = right;
		oldParent->left = right->right;
		if (right->right)
			right->right->parent = oldParent;
		right->right = oldParent;
		oldParent->parent = right;
		if (right->balance < 0) {
			if (oldParent)
				oldParent->balance = 1;
			toRotate->balance = 0;
		}
		else {
			if (right->balance == 0) {
				if (oldParent)
					oldParent->balance = 0;
				toRotate->balance = 0;
			}
			else {
				if (oldParent)
					oldParent->balance = 0;
				toRotate->balance = -1;
			}
		}
		right->balance = 0;
		return right;
	}
	void myDestroy(Node *toDestroy) {
		if (toDestroy->left)
			myDestroy(toDestroy->left);
		if (toDestroy->right)
			myDestroy(toDestroy->right);
		if (toDestroy->left)
			delete toDestroy->left;
		if (toDestroy->right)
			delete toDestroy->right;
	}

	~TreeMap() {
		if (root) {
			myDestroy(root);
			delete root;
		}
	}


	void insert(const key_type& key, const mapped_type &value)
	{
		Node *insertHere = root;
		Node *toInsert = new Node(key, value);
		if (!root) {
			root = toInsert;
			++treeSize;
		}
		while (insertHere) {
			if (key < insertHere->key) {
				if (insertHere->left)
					insertHere = insertHere->left;
				else {
					insertHere->left = toInsert;
					toInsert->parent = insertHere;
					treeSize++;
					break;
				}
			}
			else if (key > insertHere->key) {
				if (insertHere->right)
					insertHere = insertHere->right;
				else {
					insertHere->right = toInsert;
					toInsert->parent = insertHere;
					treeSize++;
					break;
				}
			}
			else {                                   /* nadpisuje element o podanym kluczu jeżeli już istnieje */
				insertHere->key = key;
				insertHere->value = value;
				delete toInsert;
				return;
			}
		}
		Node *currentRoot = toInsert;
		Node *currentRootParent = currentRoot->parent;
		Node *grandparent = nullptr;
		for (; currentRootParent; currentRootParent = currentRootParent->parent) {
			if (!isLeftChild(currentRoot)) {
				if (currentRootParent->balance > 0) {
					grandparent = currentRootParent->parent;
					if (currentRoot->balance < 0) {
						currentRoot = rotateRightLeft(currentRoot);
					}
					else {
						currentRoot = rotateLeft(currentRoot);
					}
				}
				else {
					if (currentRootParent->balance < 0) {
						currentRootParent->balance = 0;
						break;
					}
					++currentRootParent->balance;
					currentRoot = currentRoot->parent;
					continue;
				}
			}
			else {
				if (currentRootParent->balance < 0) {
					grandparent = currentRootParent->parent;
					if (currentRoot->balance > 0) {
						currentRoot = rotateLeftRight(currentRoot);
					}
					else {
						currentRoot = rotateRight(currentRoot);
					}
				}
				else {
					if (currentRootParent->balance > 0) {
						currentRootParent->balance = 0;
						break;
					}
					--currentRootParent->balance;
					currentRoot = currentRoot->parent;
					continue;
				}
			}
			currentRoot->parent = grandparent;
			if (grandparent) {
				currentRootParent->key < grandparent->key ? grandparent->left = currentRoot : grandparent->right = currentRoot;
				break;
			}
			else {
				root = currentRoot;
				break;
			}
		}
	}

	/*!
	 * dodaje wpis do slownika przez podanie pary klucz-wartosc
	 */
	void insert(const value_type &key_value)
	{
		insert(key_value.first, key_value.second);
	}

	/*!
	 * zwraca referencje na wartosc dla podanego klucza
	 *
	 * jezeli elementu nie ma w slowniku, dodaje go
	 */
	mapped_type& operator[](const key_type& key)
	{
		Node* currentNode = root;

		// Find requested key and return reference to it
		while (currentNode != nullptr) {
			if (currentNode->key == key)
				return currentNode->value;
			else if (key < currentNode->key)
				currentNode = currentNode->left;
			else
				currentNode = currentNode->right;
		}

		// Add key to tree if it doesn't exist
		insert(key, 0);

		// Find requested key and return reference to it
		currentNode = root;
		while (currentNode != nullptr) {
			if (currentNode->key == key)
				return currentNode->value;
			else if (key < currentNode->key)
				currentNode = currentNode->left;
			else
				currentNode = currentNode->right;
		}

		throw std::runtime_error("insert() failed to add new node");
	}

	/*!
	 * zwraca wartosc dla podanego klucza
	 */
	const mapped_type& value(const key_type& key) const
	{
		Node* currentNode = root;
		while (currentNode != nullptr) {
			if (currentNode->key == key)
				return currentNode->value;
			else if (key < currentNode->key)
				currentNode = currentNode->left;
			else
				currentNode = currentNode->right;
		}
		throw std::runtime_error("KeyError: Requested key was not found in tree");
	}

	/*!
	 * zwraca informacje, czy istnieje w slowniku podany klucz
	 */
	bool contains(const key_type& key) const {
		Node* currentNode = root;
		while (currentNode != nullptr) {
			if (currentNode->key == key)
				return true;
			else if (key < currentNode->key)
				currentNode = currentNode->left;
			else
				currentNode = currentNode->right;
		}
		return false;
	}

	/*!
	 * zwraca liczbe wpisow w slowniku
	 */
	size_t size() const {
		return treeSize;
	}

	bool checkIntegrity() {
		if (root)
			return _check(root);
		return true;
	}
	int checkBalance() {
		if (root)
			return _height(root);
		else
			return 0;
	}
};

#include "tests.h"
#include <fstream>

int main()
{
	// Perform unit tests
	unit_test();

	// Perform comparision with stl implementation
	int testsSizes[] = { 10, 100, 500, 1000, 1500, 3000, 7500, 15000, 30000, 69000 };
	std::string word;
	std::fstream f("pan-tadeusz.txt");

	for (int i = 0; i < sizeof(testsSizes) / sizeof(*testsSizes); i++) {
		// Insertion
		Benchmark<std::chrono::nanoseconds> b1;
		TreeMap<std::string, int> ourTree;

		f.seekg(0);
		for (int j = 0; j < testsSizes[i]; j++) {
			f >> word;
			ourTree.insert(word, 0);
		}
		size_t elapsed = b1.elapsed();
		assert(ourTree.checkIntegrity());
		int height = ourTree.checkBalance();
		//std::cout << height << "\n";

		Benchmark<std::chrono::nanoseconds> b2;
		std::map<std::string, int> stlTree;

		f.seekg(0);
		for (int j = 0; j < testsSizes[i]; j++) {
			f >> word;
			stlTree.insert({ word, 0 });
		}
		size_t elapsed2 = b1.elapsed();
		std::cout << "Size " << testsSizes[i] << " ; ourTree " << elapsed << " ; stlTree " << elapsed2 << std::endl;

		// Search for some word
		f.seekg(0);
		for (int j = 0; j < testsSizes[i] / 2; j++)
			f >> word;

		Benchmark<std::chrono::nanoseconds> s1;
		auto match = ourTree[word];
		size_t finding = s1.elapsed();

		Benchmark<std::chrono::nanoseconds> s2;
		auto match2 = stlTree[word];
		size_t finding2 = s2.elapsed();
		std::cout << "Searching: ourTree " << finding << " ; stlTree " << finding2 << std::endl;
	}
	system("pause");
	return 0;
}
