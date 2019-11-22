#include <iostream>
#include <cassert>
template<typename T>
class BinarySearchTree{
	private:
		struct Node{
			T value;
			Node *child_l, *child_r;
			Node(const T &v) : value(v), child_l(nullptr), child_r(nullptr) {}
		};
		Node *root;
		void _display(Node *r, const unsigned int &i = 0){
			for(unsigned int j = 0; j < i; ++j)
				std::cout << "|";
			std::cout << r->value << "\n";
			if(r->child_l)
				_display(r->child_l, i + 1);
			if(r->child_r)
				_display(r->child_r, i + 1);
		}
		bool _check(Node *r){
			if(r->child_l && !_check(r->child_l))
				return false;
			if(r->child_r && !_check(r->child_r))
				return false;
			if(r->child_l && r->child_r)
				return (r->child_l->value < r->value && r->child_r->value > r->value);
			if(r->child_l)
				return r->child_l->value < r->value;
			if(r->child_r)
				return r->child_r->value > r->value;
			return true;
		}
		void _delete(Node *r){
			if(r->child_l)
				_delete(r->child_l);
			if(r->child_r)
				_delete(r->child_r);
			delete r;
		}
	public:
		BinarySearchTree() : root(nullptr) {}
		int add(const T &val){
			Node *newNode = new Node(val);
			if(!root){
				root = newNode;
				return 0;
			}
			Node *parent = root;
			while(true){
				if(val < parent->value){
					if(!parent->child_l){
						parent->child_l = newNode;
						return 0;
					}
					else
						parent = parent->child_l;
				}
				else if(val == parent->value){
					delete newNode;
					return 1;
				}
				else{ 
					if(!parent->child_r){
						parent->child_r = newNode;
						return 0;
					}
					else
						parent = parent->child_r;
				}
			}
		}
		int remove(const T &val){
			if(!root)
				return 1;
			Node *parent = root;
			Node *grandparent = nullptr;
			while(parent->value != val){
				if(val < parent->value){
					if(parent->child_l){
						grandparent = parent;
						parent = parent->child_l;
					}
					else
						return 1;
				}
				else{
					if(parent->child_r){
						grandparent = parent;
						parent = parent->child_r;
					}
					else
						return 1;
				}
			}
			if(parent->child_l && parent->child_r){
				Node *replacement = parent->child_r;
				Node *replace_parent = parent;
				while(replacement->child_l){
					replace_parent = replacement;
					replacement = replacement->child_l;
				}
				replace_parent->child_l == replacement ? replace_parent->child_l = replacement->child_r : replace_parent->child_r = replacement->child_r;
				parent->value = replacement->value;
				delete replacement;
				return 0;
			}
			else if(parent->child_l || parent->child_r){
				Node *replacement = parent->child_l ? parent->child_l : parent->child_r;
				parent->value = replacement->value;
				parent->child_l = replacement->child_l;
				parent->child_r = replacement->child_r;
				delete replacement;
				return 0;
			}
			if(grandparent)
				grandparent->child_r == parent ? grandparent->child_r = nullptr : grandparent->child_l = nullptr;
			delete parent;
			return 0;
		}
		static void badTreeIntegrityTest(){
			BinarySearchTree<T> badTree;
			Node *n[5];
			for(unsigned int i = 0; i < 5; ++i)
				n[i] = new Node(i);
			
			badTree.root = n[0];
			badTree.root->child_l = n[1];
			assert(!badTree.checkIntegrity());

			badTree.root = n[4];
			badTree.root->child_r = n[3];
			assert(!badTree.checkIntegrity());

			n[0]->child_l = n[4]->child_r = nullptr;
			badTree.root = n[2];
			badTree.root->child_l = n[1];
			badTree.root->child_r = n[3];
			badTree.root->child_l->child_l = n[4];
			badTree.root->child_l->child_r = n[0];
			assert(!badTree.checkIntegrity());

			for(unsigned int i = 0; i < 5; ++i)
				delete n[i];
			badTree.root = nullptr;
		}
		bool checkIntegrity(){
			if(root)
				return _check(root);
			return true;
		}
		void display(){
			if(root)
				_display(root);
		}
		~BinarySearchTree(){
			if(root)
				_delete(root);
		}
};
int main(){
	BinarySearchTree<int> bst;

	bst.add(5);
	bst.add(-10);
	bst.add(15);
	bst.add(25);
	bst.add(7);
	bst.add(1);
	assert(bst.checkIntegrity());

	assert(bst.add(5) == 1);
	bst.display();

	bst.remove(7);
	assert(bst.checkIntegrity());

	bst.remove(15);
	assert(bst.checkIntegrity());

	bst.remove(5);
	assert(bst.checkIntegrity());

	bst.display();
	assert(bst.remove(5) == 1);

	BinarySearchTree<int>::badTreeIntegrityTest();
}
