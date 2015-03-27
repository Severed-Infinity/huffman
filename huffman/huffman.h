//
//  huffman.h
//  huffman
//
//  Created by david swift on 13/03/2015.
//  Copyright (c) 2015 david swift. All rights reserved.
//


#ifndef __huffman__
#define __huffman__

#include <stdio.h>
#include <iostream>
#include <map>
#include <bitset>
#include <fstream>
#include <queue>
#include <iomanip>
#include <vector>

// http://rendon.x10.mx/huffman-coding/
// used as a reference for building the huffman code


/*
 *  a simple struct defining the huffman node
 */
struct HuffmanNode {
  char letter;
  int frequency;
  HuffmanNode* left;
  HuffmanNode* right;
  
  HuffmanNode(){
    letter = '\0';
    frequency = 0;
    left = nullptr;
    right = nullptr;
  }
  
  ~HuffmanNode() {}
};

/*
 *  Huffman Node Comparator is used in the priority queue to order the nodes
 */
struct HuffmanNodeComparator {
  bool operator()(const HuffmanNode* n1, const HuffmanNode* n2){
    return ((n1->left == nullptr && n1->right == nullptr) && (n1->frequency > n2->frequency)) || ((n1->frequency > n2->frequency) && n1->left == nullptr) || (n1->frequency > n2->frequency);
  }
};

/*
 *  namespace used for better access
 */
namespace huffman {
  
  /*
   *  defining a simple byte type that holds the string of 1's and 0's
   *  for the file compression and decompression.
   */
  typedef unsigned char byte;
  
//  development variables
  
//  static std::map<char, int> charFrequencies;
//  static std::map<char, int>::iterator charFrequenciesIterator;
//  static std::string message;
//  static std::priority_queue<HuffmanNode*, std::deque<HuffmanNode*>, HuffmanNodeComparator> huffmanPrioity;
//  static HuffmanNode* root;
//  static std::map<char, std::string> charCodes;
//  static std::map<char, std::string>::iterator charCodesIterator;
  
  /*
   *  @param std::ifstream
   *    read only file
   *  @return std::map<char, int>
   *    map of character frequencies
   *
   *  given a fie read through it and count the occurences
   *  of each character found and build a frequency map of
   *  the found characters then return that frequency map.
   */
  static std::map<char, int> buildFrequenciesMap(std::ifstream &file){
    std::map<char, int> charFrequencies;
    static std::string message = "";
    
    int totalFreq = 0;
    if(file.is_open()){
      file.seekg(0, std::ios::end);
      message.reserve(file.tellg());
      file.seekg(0, std::ios::beg);
      
      message.assign((std::istreambuf_iterator<char>(file)),
                     std::istreambuf_iterator<char>());
      
      for (int i = 0; i < message.length(); i++) {
        char c = message[i];
        int count = 1;
        for(int j = 0; j < i; j++){
          if(message[j] == c) {
            count--;
          }
        }
        for (int j = i+1; j < message.length(); j++) {
          if (message[j] == c) {
            count++;
          }
        }
        charFrequencies.insert({c,count});
        totalFreq += count;
      }
    }
    return charFrequencies;
  }
  
  /*
   *  @param const std::map<char, int> &
   *    const character frequency map reference
   *  @return void
   *
   *  given a const character frequency map reference, iterate
   *  through the map and print out each key/value pair found
   *  in the map.
   */
  static void printFrequencyMap(const std::map<char, int> &charFrequenciesRef){
    std::map<char, int> charFrequencies = charFrequenciesRef;
    std::map<char, int>::iterator charFrequenciesIterator;
    
    std::cout << "frequency map" << std::endl;
    for (charFrequenciesIterator = charFrequencies.begin(); charFrequenciesIterator != charFrequencies.end(); charFrequenciesIterator++) {
      std::cout << "char: " << charFrequenciesIterator->first << " freq: " << charFrequenciesIterator->second << std::endl;
    }
  }
  
  /*
   *  @param const std::map<char, int> &
   *    const character frequency map reference
   *  @return std::priority_queue<HuffmanNode*, std::deque<HuffmanNode*>, HuffmanNodeComparator>
   *    priority queue of huffmanNode pointers
   *
   *  given a const character frequency map reference, iterate
   *  through the map and and create new huffmanNode pointers
   *  using the key for the letter and value for the frequency
   *  then push the new huffmanNode pointer on the priority
   *  queue. return the priority queue when all key/value pairs
   *  have been pushed on to the priority queue as huffmanNode
   *  pointers.
   */
  static std::priority_queue<HuffmanNode*, std::deque<HuffmanNode*>, HuffmanNodeComparator> buildPriorityQueue(const std::map<char, int> &charFrequenciesRef){
    std::map<char, int> charFrequencies = charFrequenciesRef;
    std::map<char, int>::iterator charFrequenciesIterator;
    std::priority_queue<HuffmanNode*, std::deque<HuffmanNode*>, HuffmanNodeComparator> huffmanPriority;
    
    for (charFrequenciesIterator = charFrequencies.begin(); charFrequenciesIterator != charFrequencies.end(); charFrequenciesIterator++) {
      HuffmanNode* newNode = new HuffmanNode();
      newNode->letter = charFrequenciesIterator->first;
      newNode->frequency = charFrequenciesIterator->second;
      huffmanPriority.push( newNode );
    };
    
    return huffmanPriority;
  }
  
  /*
   *  @param const std::priority_queue<HuffmanNode*, std::deque<HuffmanNode*>, HuffmanNodeComparator> &
   *    priority queue of huffmanNode pointers
   *  @return void
   *
   *  given a consnt huffmanNode pointer priority queue reference
   *  iterate through the queue and print out the each letter and
   *  frequency in the queue.
   */
  static void printPriorityQueue(const std::priority_queue<HuffmanNode*, std::deque<HuffmanNode*>, HuffmanNodeComparator> &huffmanPrioityQueueRef){
    std::priority_queue<HuffmanNode*, std::deque<HuffmanNode*>, HuffmanNodeComparator> priorityQueue = huffmanPrioityQueueRef;
    
    std::cout << "priority que, size: " << priorityQueue.size() << std::endl;
    while (priorityQueue.size() > 0) {
      std::cout << "node: ";
      std::cout << priorityQueue.top()->letter << " " << priorityQueue.top()->frequency << std::endl;
      priorityQueue.pop();
    }
  }
  
  /*
   *  @const std::priority_queue<HuffmanNode*, std::deque<HuffmanNode*>, HuffmanNodeComparator> &
   *    priority queue of huffmanNode pointers
   *  @return huffmanNode*
   *    huffman node tree
   *
   *  given a consnt huffmanNode pointer priority queue reference
   *  iterate through the queue (as long as it has atleast one
   *  element) and take the top two elements and create a new
   *  huffmanNode pointer assigning the top two elements to the 
   *  new huffmanNode pointers left and right attribute in the 
   *  order they came off the queue. the new nodes frequency is 
   *  the combined frequency of its left and right children.
   *
   *  push the new node back on to the priority queue until
   *  only one node remains.
   *  return the single remaining node as the root of the tree.
   */
  static HuffmanNode* buildHuffmanTree(std::priority_queue<HuffmanNode*, std::deque<HuffmanNode*>, HuffmanNodeComparator> &huffmanPriority){
    HuffmanNode* root;
    if(!huffmanPriority.empty()){
      while (huffmanPriority.size() > 1) {
        HuffmanNode* newNode = new HuffmanNode();
        newNode->left = huffmanPriority.top();
        huffmanPriority.pop();
        newNode->right = huffmanPriority.top();
        newNode->frequency = newNode->left->frequency + newNode->right->frequency;
        huffmanPriority.pop();
        
        huffmanPriority.push(newNode);
      }
      root = huffmanPriority.top();
      huffmanPriority.pop();
    }
    return root;
  }
  
  /*
   *  @param HuffmanNode*
   *    root of the huffman node tree
   *  @param int
   *    depth of the node
   *  @param std::string
   *    position of the node
   *
   *  using post traversal of a binary print out all leaf node 
   *  letters alongside their frequency including the depth and
   *  position of the node.
   */
  static void traverseHuffmanTree(HuffmanNode* root, int depth, std::string position){
    if(root != nullptr){
      if (root->left == nullptr && root->right == nullptr) {
        std::cout << "char " << root->letter << std::setw(10) << "freq " << root->frequency << std::setw(10) << "depth " << depth << "\nposition " << position << std::endl;
        return;
      }
      if(root->left != nullptr){
        traverseHuffmanTree(root->left, depth+1, position+"left, ");
      }
      if(root->right != nullptr){
        traverseHuffmanTree(root->right, depth+1, position+"right, ");
      }
    }
  }
 
  /*
   *  @param HuffmanNode*
   *    root of the huffman node tree
   *  @param std::map<char, std::string> &
   *    character encode map
   *  @param std::string
   *    binary representation of the position
   *  @return std::map<char, std::string>
   *    character encode map
   *
   *  recurively
   *  given the root of the huffman tree and a character encode
   *  map travrse through the tree appending to code '0' if you
   *  go to the left child, or '1' if you go to the right child
   *
   *  if the root is a left node insert into the character
   *  encode map the roots letter as the key and built code as
   *  the value then return the character enode map.
   */
  static std::map<char, std::string> generateCharCodes(HuffmanNode* &root, std::map<char, std::string> &charCodes, std::string code){
    if (root != nullptr) {
      if(root->left == nullptr && root->right == nullptr){
        charCodes.insert({root->letter, code});
        goto charCodeExit;
      } else {
        if(root->left != nullptr)
          generateCharCodes(root->left, charCodes, code+"0");
        if(root->right != nullptr)
          generateCharCodes(root->right, charCodes, code+"1");
      }
    }
  charCodeExit:
    return charCodes;
  }
  
  /*
   *  @param HuffmanNode*
   *    root of the huffman node tree
   *  @param std::string
   *    binary representation of the position
   *  @return std::map<char, std::string>
   *    character encode map
   *
   *  a wrapper method for generateCharCodes that removes the
   *  requirement of the character encode map and simply
   *  creates an empty character encode map that returns a
   *  call to the generateCharCodes with the map.
   */
  static std::map<char, std::string> generateCharCodes(HuffmanNode* &root, std::string code){
    std::map<char, std::string> charCodes;
    return generateCharCodes(root, charCodes, "");
  }
  
  /*
   *  @param const std::map<char, std::string> &
   *    const character encode map reference
   *  @return void
   *
   *  given a const character encode map reference, iterate
   *  through the map and print out each key/value pair found
   *  in the map.
   */
  static void printCharCodes(const std::map<char, std::string> &charCodesRef){
    std::map<char, std::string> charCodes = charCodesRef;
    std::map<char, std::string>::iterator charCodesIterator;
    
    std::cout << "character encode map" << std::endl;
    for (charCodesIterator = charCodes.begin(); charCodesIterator != charCodes.end(); charCodesIterator++) {
      std::cout << "char: " << charCodesIterator->first << " code: " << charCodesIterator->second << std::endl;
    }
  }
  
  /*
   *  @param std::ifstream &
   *    read only file
   *  @param const std::map<char, std::string> &
   *    const character encode map reference
   *  @return std::ofstream
   *    compressed file
   *
   *  given a file and a character encode map, read in
   *  the files contents looking up each characters
   *  encode in the character encode map and append to
   *  an encoded string.
   *  using the encoded string write each byte out to
   *  a compressed file.
   *  then return the compressed file.
   */
  static std::ofstream generateEncodedFile(std::ifstream &file, const std::map<char, std::string> &charCodesRef){
    std::ofstream encodeFile ("/Volumes/My Passport for Mac/It-Tallaght/ADS2/huffman/huffman/huffman/EncodeFile", std::ios::trunc);
    std::map<char, std::string> charCodes = charCodesRef;
    std::map<char, std::string>::iterator charCodesIterator;
    std::string message = "";
    std::string encodeMessage = "";
    std::string compressedEncodeMessage = "";
    
    if (file.is_open()) {
      file.seekg(0, std::ios::end);
      message.reserve(file.tellg());
      file.seekg(0, std::ios::beg);
      
      message.assign((std::istreambuf_iterator<char>(file)),
                     std::istreambuf_iterator<char>());
      
      for (int i = 0; i < message.length(); i++) {
        charCodesIterator = charCodes.find(message[i]);
        encodeMessage+=charCodesIterator->second;
      }
      for (int i = 0; i < encodeMessage.length(); (i+=8)) {
        compressedEncodeMessage += (byte)std::bitset<8>(encodeMessage.substr(i, i+8)).to_ulong();
      }
      encodeFile << compressedEncodeMessage;
    }
    return encodeFile;
  }
  
  /*
   *  @param std::ifstream
   *    read only file
   *  @return std::ostream
   *    compressed file
   *
   *  a wrapper function that calls the following in order
   *  -buildFrequenciesMap
   *  -buildPriorityQueue
   *  -buildHuffmanTree
   *  -generateCharCodes
   *  -generateEncodeFile
   *
   *  return the compressed file when complete.
   */
  static std::ofstream encode(std::ifstream &file){
    std::map<char, int> charFrequencies = buildFrequenciesMap(file);
    //        printFrequencyMap(charFrequencies);
    
    std::priority_queue<HuffmanNode*, std::deque<HuffmanNode*>, HuffmanNodeComparator> huffmanPriority = buildPriorityQueue(charFrequencies);
    //        printPriorityQueue(huffmanPriority);
    
    HuffmanNode* root = buildHuffmanTree(huffmanPriority);
    //        std::cout << "printing huffman tree" << std::endl;
    //        traverseHuffmanTree(root, 0, "");
    std::map<char, std::string> charCodes = generateCharCodes(root, "");
    //        printCharCodes(charCodes);
    
    std::ofstream encyrptFile = generateEncodedFile(file, charCodes);
    file.close();
    return encyrptFile;
  }
  
  /*
   *  @param std::ifstream
   *    read only file
   *  @return huffmanNode*
   *    root of the huffman tree
   *
   *  an alterate encode function that is a wrapper 
   *  function that calls the following in order
   *  -buildFrequenciesMap
   *  -buildPriorityQueue
   *  -buildHuffmanTree
   *  -generateCharCodes
   *  -generateEncodeFile
   *
   *  and returns the root of the huffman tree rather than
   *  the compressed file when complete.
   */
  static HuffmanNode* encodeReturnTree(std::ifstream &file){
    std::map<char, int> charFrequencies = buildFrequenciesMap(file);
    //        printFrequencyMap(charFrequencies);
    
    std::priority_queue<HuffmanNode*, std::deque<HuffmanNode*>, HuffmanNodeComparator> huffmanPriority = buildPriorityQueue(charFrequencies);
    //        printPriorityQueue(huffmanPriority);
    
    HuffmanNode* root = buildHuffmanTree(huffmanPriority);
    //        std::cout << "printing huffman tree" << std::endl;
    //        traverseHuffmanTree(root, 0, "");
    std::map<char, std::string> charCodes = generateCharCodes(root, "");
    //        printCharCodes(charCodes);
    
    generateEncodedFile(file, charCodes);
    file.close();
    return root;
  }
  
  /*
   *  @param std::ifstream &
   *    read only encrypted file
   *  @param HuffmanNode*
   *    root of the huffman tree
   *  @return std::ofstream
   *    decompressed file
   *
   *  given an encoded file and a huffman tree, read in
   *  the files contents traversing the huffman tree and
   *  appending the character of the leaf node and append to
   *  a decoded string.
   *  then write the decoded string to a decoded file and
   *  return the decoded file.
   */
  static std::ofstream decode(std::ifstream &encodeFile, HuffmanNode* &root){
    std::ofstream decodedFile("/Volumes/My Passport for Mac/It-Tallaght/ADS2/huffman/huffman/huffman/DecodedFile", std::ios::trunc);
    std::string message = "";
    std::string decodedMessage = "";
    std::string decompressedMessage = "";
    
    if (encodeFile.is_open()) {
      encodeFile.seekg(0, std::ios::end);
      message.reserve(encodeFile.tellg());
      encodeFile.seekg(0, std::ios::beg);
      
      message.assign((std::istreambuf_iterator<char>(encodeFile)),
                     std::istreambuf_iterator<char>());
      
      for (int i = 0; i < message.length(); i++) {
        decompressedMessage += std::bitset<8>(message[i]).to_string();
      }
      
      //            http://stackoverflow.com/questions/21854069/decoding-huffman-tree
      //            took the following for statement straight from the top reply
      //            and modified it slightly
      
      HuffmanNode* node = root;
      for (int i = 0; i != decompressedMessage.size(); i++) {
        if (decompressedMessage[i] == '0') {
          node = node->left;
        } else if(decompressedMessage[i] == '1') {
          node = node->right;
        }
        if (node->left == nullptr && node-> right == nullptr)
        {
          decodedMessage += node->letter;
          node = root;
        }
      }
      for (int i = 0; i < decodedMessage.size()-1; i++) {
        decodedFile << decodedMessage[i];
      }
    }
    
    encodeFile.close();
    decodedFile.close();
    return decodedFile;
  }
}

#endif /* defined(__huffman__) */


