#include <iostream>
#include <chrono>
#include <random>
#include <list>
#include <algorithm>
#include <iterator>
#include <vector>
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
		Deck(std::vector<int> v){
			Card *last = nullptr;
			for(auto i = v.begin() + 1; i < v.end(); ++i){
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
				Card *prev = firstUnsorted;
				Card *next = min->next;
				while(prev->next != min) 
					prev = prev->next;
				if(lastSorted == nullptr)
					lastSorted = min;
				else{
					lastSorted->next = min;
					lastSorted = lastSorted->next;
				}
				lastSorted->next = firstUnsorted;
				prev->next = next;
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
	std::cout << "after shuffling: \n";
	shuffled.display();
}
