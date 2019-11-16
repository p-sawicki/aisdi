#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>
#include <iterator>
#include <vector>
#include <cassert>
class Deck{
	private:
		struct Card{
			char value;
			char suit;
			Card *next;
		};
		Card *head;
	public:
		Deck() : head(nullptr) {}
		Deck(const std::vector<int> &v){
			Card *last = nullptr;
			for(auto i = v.begin(); i < v.end(); ++i){
				Card *newCard = new Card;
				newCard->value = *i / 4;
				newCard->suit = *i % 4;
				newCard->next = nullptr;
				if(last != nullptr){
					last->next = newCard;
					last = last->next;
				}
				else{
					head = newCard;
					last = head;
				}
			}
		}
		unsigned int getPos(const unsigned int &i){
			Card *cur = head;
			unsigned int pos = 0;
			while(cur->value != i / 4 || cur->suit != i % 4){
				if(cur->value == i / 4)
					++pos;
				cur = cur->next;
			}
			return pos;
		}
		void display(){
			Card *first = head;
			while(first != nullptr){
				if(first->value < 8)
					std::cout << first->value + 2;
				else if(first->value == 8)
					std::cout << "T";
				else if(first->value == 9)
					std::cout << "J";
				else if(first->value == 10)
					std::cout << "Q";
				else if(first->value == 11)
					std::cout << "K";
				else 
					std::cout << "A";
				if(first->suit == 0)
					std::cout << "s ";
				else if(first->suit == 1)
					std::cout << "c ";
				else if(first->suit == 2)
					std::cout << "h ";
				else
					std::cout << "d ";
				first = first->next;
			}
			std::cout << "\n";
		}
		void sort(){
			Card *lastSorted = nullptr;
			Card *firstUnsorted = head;
			while(firstUnsorted != nullptr){
				Card *min = firstUnsorted;
				Card *cur = min;
				while(cur != nullptr){
					if(cur->value < min->value)
						min = cur;
					cur = cur->next;
				}
				if(min == firstUnsorted){
					lastSorted->next = min;
					lastSorted = lastSorted->next;
					firstUnsorted = firstUnsorted->next;
					lastSorted->next = firstUnsorted;
					continue;
				}
				Card *prev = firstUnsorted;
				Card *next = min->next;
				while(prev->next != min) 
					prev = prev->next;
				if(lastSorted == nullptr){
					lastSorted = min;
					head = lastSorted;
				}
				else{
					lastSorted->next = min;
					lastSorted = lastSorted->next;
				}
				lastSorted->next = firstUnsorted;
				prev->next = next;
			}
		}
		~Deck(){
			while(head != nullptr){
				Card *next = head->next;
				delete head;
				head = next;
			}
		}	
};	
int main(){
	unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 generator(seed);
	const unsigned int deckSize = 52;
	std::vector<int> deckToShuffle;
	for(unsigned int i = 0; i < deckSize; ++i)
		deckToShuffle.emplace_back(i);
	Deck unschuffled(deckToShuffle);
	std::cout << "before shuffling: \n";
	unschuffled.display();
	std::shuffle(deckToShuffle.begin(), deckToShuffle.end(), generator);		
	Deck shuffled(deckToShuffle);
	Deck unsorted(deckToShuffle);
	std::cout << "after shuffling: \n";
	shuffled.display();
	shuffled.sort();
	std::cout << "after sorting: \n";
	shuffled.display();
	for(unsigned int i = 0; i < deckSize; ++i){
		unsigned int posInSorted = shuffled.getPos(i);
		unsigned int posInUnsorted = unsorted.getPos(i);
		assert(posInSorted == posInUnsorted);
	}
	std::cout << "all asserts OK\n";
}
