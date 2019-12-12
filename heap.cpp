#include <iostream>
#include <vector>
#include <algorithm>
template<typename T>
class MaxQuadraryHeap {
private:
	std::vector<T> content;
	inline unsigned int parent(const unsigned int &i) {
		return i > 0 ? (i - 1) / 4 : 0;
	}
	inline unsigned int child(const unsigned int &i, const unsigned int &which) {
		return i * 4 + which;
	}
	void siftUp(unsigned int i) {
		for (unsigned int p = parent(i); i > 0; p = parent(p)) {
			if (content[i] > content[p])
				std::swap(content[i], content[p]);
			else
				break;
			i = p;
		}
	}
	void siftDown(unsigned int i) {
		for (unsigned int maxChild = child(i, 1);; i = maxChild) {
			if (maxChild >= content.size())
				break;
			for (unsigned int j = 2; j <= 4; ++j) {
				unsigned int c = child(i, j);
				if (c >= content.size())
					break;
				if (content[c] > content[maxChild])
					maxChild = c;
			}
			if (content[i] < content[maxChild])
				std::swap(content[i], content[maxChild]);
			else
				break;
		}
	}
public:
	MaxQuadraryHeap() : content(std::vector<T>()) {}
	void push(T toInsert) {
		content.push_back(toInsert);
		siftUp(content.size() - 1);
	}
	void pop() {
		std::swap(content[0], content[content.size() - 1]);
		content.pop_back();
		siftDown(0);
	}
	void print() {
		for (unsigned int i = 0; i < content.size(); ++i)
			std::cout << content[i] << " ";
		std::cout << "\n";
	}
};
int main() {
	MaxQuadraryHeap<int> heap;
	heap.push(69);
	heap.push(12);
	heap.push(77);
	heap.push(20);
	heap.push(79);
	heap.push(8);
	heap.push(19);
	heap.push(6);
	heap.push(97);
	heap.push(9);
	heap.print();
	for (unsigned int i = 0; i < 10; ++i) {
		heap.pop();
		heap.print();
	}
	system("pause");
}