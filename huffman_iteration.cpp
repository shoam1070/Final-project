// C++ program for Huffman Coding
#include<iterator>
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
    char data;

    // Frequency of the character
    unsigned freq;

    // Left and right child
    MinHeapNode *left, *right;

    MinHeapNode(char data, unsigned freq)

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
void printCodes(struct MinHeapNode* root, string str, std::map<char,std::string> &mymap)
{

    if (!root)
        return;

    if (root->data != '$'){
//        cout << root->data << ": " << str << "\n";
        mymap[root->data] = str ;
    }

    printCodes(root->left, str + "0", mymap);
    printCodes(root->right, str + "1", mymap);
}

// The main function that builds a Huffman Tree and
// print codes by traversing the built Huffman Tree
std::map<char,std::string> HuffmanCodes(char data[], int freq[], int size)
{
    struct MinHeapNode *left, *right, *top;

    // Create a min heap & inserts all characters of data[]
    priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare> minHeap;

    for (int i = 0; i < size; ++i)
      if (freq[i] != 0)
        minHeap.push(new MinHeapNode(data[i], freq[i]));

    // Iterate while size of heap doesn't become 1
    while (minHeap.size() != 1) {

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
        top = new MinHeapNode('$', left->freq + right->freq);

        top->left = left;
        top->right = right;

        minHeap.push(top);
    }

    // Print Huffman codes using
    // the Huffman tree built above
    std::map<char,std::string> mymap;
    printCodes(minHeap.top(), "", mymap);
    // while (!minHeap.empty()) {
    //   MinHeapNode* temp = minHeap.top();
    //   free(temp);
    //   minHeap.pop();
    // }
    return mymap;
}



// Driver program to test above functions
int main(int argc, char* argv[]) {
    if(argc != 3){
      //  if((argv[1][0] == '-') && (argv[1][1] == 'd'))
        printf ("Error opening file");
        exit (EXIT_FAILURE);
    }
    FILE* myfile = fopen(argv[1],"r");
    if (myfile!=NULL)
    {

      std::ifstream ifs(argv[1]);
      std::string content( (std::istreambuf_iterator<char>(ifs) ),
                           (std::istreambuf_iterator<char>()));

    
    char arr[256] = {};
    int freq[256] = {};
    size_t count = 0 ;
    for (unsigned i=0; i<content.length(); ++i)
    {
      if (arr[content.at(i)] == 0) count++;
      arr[content.at(i)] = content.at(i);
      freq[content.at(i)] ++;
    }
    map <char , int > indexs;
    char data[count];
    int freq_data[count];
    int index = 0 ;
    for (size_t i = 0; i < 256; i++) {
      if (arr[i] != 0){
        data[index] = arr[i];
        indexs[arr[i]] = index ;
        freq_data[index] = freq[i];
        index++;
      }

    }
    // while (){}
    int size = sizeof(data) / sizeof(data[0]);
    map <char , std::string> mymap;
    mymap = HuffmanCodes(data, freq_data, size);  // the map with all the code word

    // that the place to start to write to the file.
    //std::cout << "/* a =  */"<< mymap.find('a')->second << '\n';

    fclose(myfile);
    FILE* myfile = fopen(argv[2],"w+");
    char nextByte = 0;
   int bitCounter = 0;
   size_t iteration = 0 ;
    for (size_t i = 0; i < content.length(); i++) {
            iteration++;
            std::string str = mymap.find(content.at(i))->second;
    for(size_t j = 0; j < str.size(); j++, bitCounter++){
        if(bitCounter == 8){
            fwrite(&nextByte , sizeof(char) , 1 , myfile );
            nextByte = 0;
            bitCounter = 0;
        }
        if(str[j] == '1')
            nextByte = nextByte | (0x01 << bitCounter);
    }
    freq_data[indexs.find(content.at(i))->second]--;
    if (iteration == 100){
      if (i != content.size()-1)
        mymap = HuffmanCodes(data, freq_data, size);

       iteration = 0;
  }
}
    if (bitCounter != 0)
        fwrite(&nextByte , sizeof(char) , 1 , myfile );
    fclose(myfile);
}
std::cout << "end main" << '\n';                                   // end if file not null for use content
    return 0;
}

// This code is contributed by Aditya Goel
