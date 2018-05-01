// C++ program for Huffman Coding
#include<iterator>
#include <sstream>
#include <fstream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <queue>
#include <map>
using namespace std;

// A Huffman tree node
struct MinHeapNode {

	// One of the input characters
	std::string data;

	// Frequency of the character
	int freq;

	// Left and right child
	MinHeapNode *left, *right;

	MinHeapNode(std::string data, int freq)

	{

		left = right = NULL;
		this->data = data;
		this->freq = freq;
	}
};

// For comparison of
// two heap nodes (needed in min heap)
struct compare {

	bool operator()(MinHeapNode* l, MinHeapNode* r)

	{
		return (l->freq > r->freq);
	}
};

// Prints huffman codes from
// the root of Huffman Tree.
void printCodes(struct MinHeapNode* root, string str,
		std::map<std::string, std::string> &mymap) {

	if (!root)
		return;

	if (root->data != "$") {
		//     cout << root->data << ": " << str << "\n";
		mymap[root->data] = str;
	}

	printCodes(root->left, str + "0", mymap);
	printCodes(root->right, str + "1", mymap);
}

void delete_heap(MinHeapNode* root) {
	if (root == NULL)
		return;
	delete_heap(root->left);
	delete_heap(root->right);
	if (root != NULL)
		delete (root);

}

// The main function that builds a Huffman Tree and
// print codes by traversing the built Huffman Tree
std::map<std::string, std::string> HuffmanCodes(map<std::string, int> &words) {
	struct MinHeapNode *left, *right, *top;

	// Create a min heap & inserts all characters of data[]
	priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare> minHeap;
	for (auto it = words.begin(); it != words.end(); it++) {

		if (it->second != 0) {
			minHeap.push(new MinHeapNode(it->first, it->second));
		}
	}
	// Iterate while size of heap doesn't become 1
	while (minHeap.size() > 1) {

		// Extract the two minimum
		// freq items from min heap

		left = minHeap.top();
		minHeap.pop();

		right = minHeap.top();
		minHeap.pop();

		// Create a new internal node with
		// frequency equal to the sum of the
		// two nodes frequencies. Make the
		// two extracted node as left and right children
		// of this new node. Add this node
		// to the min heap '$' is a special value
		// for internal nodes, not used
		top = new MinHeapNode("$", left->freq + right->freq);

		top->left = left;
		top->right = right;

		minHeap.push(top);
	}

	// Print Huffman codes using
	// the Huffman tree built above
	std::map<std::string, std::string> mymap;
		printCodes(minHeap.top(), "", mymap);

	delete_heap(minHeap.top());
	return mymap;
}

// Driver program to test above functions
int main(int argc, char* argv[]) {
	if (argc != 3) {
		printf("Error opening file");
		exit(EXIT_FAILURE);
	}
	FILE* myfile = fopen(argv[1], "r");
	if (myfile != NULL) {

		std::ifstream ifs(argv[1]);
		std::string content((std::istreambuf_iterator<char>(ifs)), // file input to string - content
				(std::istreambuf_iterator<char>()));
		////////////////////////////////////////////////////////////////////////
		std::istringstream iss(content);
		std::vector<std::string> results(
				(std::istream_iterator<std::string>(iss)), // results is words vector
				std::istream_iterator<std::string>());
		map<std::string, int> words; // words is a map with word (key) and frequency (val)
		for (size_t i = 0; i < results.size(); i++) {
			if (words.find(results[i]) == words.end())
				words[results[i]] = 1;
			else
				words.find(results[i])->second += 1;
		}
		///////////////////////////////////////////////////////////////

		map<std::string, std::string> mymap;
		mymap = HuffmanCodes(words);  // the map with all the code word

		// that the place to start to write to the file.

		fclose(myfile);
		FILE* myfile = fopen(argv[2], "w+");
		char nextByte = 0;
		int bitCounter = 0;
		int iteration = 0;
		for (size_t i = 0; i < results.size(); i++) {
			iteration++;
			std::string str = mymap.find(results[i])->second;
			for (size_t j = 0; j < str.size(); j++, bitCounter++) {
				if (bitCounter == 8) {
					fwrite(&nextByte, sizeof(char), 1, myfile);
					nextByte = 0;
					bitCounter = 0;
				}
				if (str[j] == '1')
					nextByte = nextByte | (0x01 << bitCounter);
			}
			if (iteration == 1) {                         // the iteration
//				std::cout << "Alice = " << words.find("Alice")->second;
				mymap = HuffmanCodes(words);
			words.find(results[i])->second--;
				iteration = 0;
			}
		}
		if (bitCounter != 0)
			fwrite(&nextByte, sizeof(char), 1, myfile);
		fclose(myfile);
	}
	std::cout << "end main" << '\n';     // end if file not null for use content
	return 0;
}

// This code is contributed by Aditya Goel
