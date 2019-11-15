#include <iostream>
#include <chrono>
#include <random>
#include <list>
#include <algorithm>
#include <iterator>
#include <vector>
class card{
	private:
		char value;
		char suit;
	public:
		card(const char &v, const char &s) : value(v), suit(s) {}
		char getValue(){
			return value;
		}
		char getSuit(){
			return suit;
		}
};
class node{
	private:
		card c;
		node *next;
	public:
		node(const char &v, const char &s, node *n) : c(v, s), next(n) {}
		node* getNext(){
			return next;
		}
		void setNext(node **n){
			next = *n;
		}
		char getValue(){
			return c.getValue();
		}
		char getSuit(){
			return c.getSuit();
		}
};
void displayDeck(node *first){
	while(first != nullptr){
		if(first->getValue() < 8)
			std::cout << first->getValue() + 2;
		else if(first->getValue() == 8)
			std::cout << "T";
		else if(first->getValue() == 9)
			std::cout << "J";
		else if(first->getValue() == 10)
			std::cout << "Q";
		else if(first->getValue() == 11)
			std::cout << "K";
		else 
			std::cout << "A";
		if(first->getSuit() == 0)
			std::cout << "s ";
		else if(first->getSuit() == 1)
			std::cout << "c ";
		else if(first->getSuit() == 2)
			std::cout << "h ";
		else
			std::cout << "d ";
		first = first->getNext();
	}
}
template<typename T>
void displayDeck(T first, T end){
	while(first != end){
		if(first->getValue() < 8)
			std::cout << first->getValue() + 2;
		else if(first->getValue() == 8)
			std::cout << "T";
		else if(first->getValue() == 9)
			std::cout << "J";
		else if(first->getValue() == 10)
			std::cout << "Q";
		else if(first->getValue() == 11)
			std::cout << "K";
		else 
			std::cout << "A";
		if(first->getSuit() == 0)
			std::cout << "s ";
		else if(first->getSuit() == 1)
			std::cout << "c ";
		else if(first->getSuit() == 2)
			std::cout << "h ";
		else
			std::cout << "d ";
		++first;
	}
		std::cout << "\n";
}
template<typename T>
T mySelectionSort(T first, T end){
	T firstUnsorted = first;
	while(firstUnsorted != end){
		T min = firstUnsorted;
		T cur = min;
		while(cur != end){
			if(cur->getValue() < min->getValue())
				min = cur;
			++cur;
		}
		std::swap(*firstUnsorted, *min);
		++firstUnsorted;
	}
	return first;
}
int main(){
	unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 generator(seed);
	const unsigned int deckSize = 52;
	std::vector<card> deckToShuffle;
	for(unsigned int i = 0; i < deckSize; ++i)
		deckToShuffle.emplace_back(i / 4, i % 4);
	std::cout << "before shuffling: \n";
	displayDeck(deckToShuffle.begin(), deckToShuffle.end());
	std::shuffle(deckToShuffle.begin(), deckToShuffle.end(), generator);		
	node head(deckToShuffle.begin()->getValue(), deckToShuffle.begin()->getSuit(), nullptr);
	node *last = &head;
	std::cout << "after shuffling: \n";
	for(auto i = deckToShuffle.begin() + 1; i < deckToShuffle.end(); ++i){
		node newNode(i->getValue(), i->getSuit(), nullptr);
		last->setNext(&&newNode);
		last = &newNode;
	}
	std::cout << head.getValue() << head.getSuit();
}
