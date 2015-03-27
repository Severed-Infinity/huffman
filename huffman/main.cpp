//
//  main.cpp
//  huffman
//
//  Created by david swift on 10/03/2015.
//  Copyright (c) 2015 david swift. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "huffman.h"


int main(int argc, const char * argv[]){
    
    std::ifstream file("/Volumes/My Passport for Mac/It-Tallaght/ADS2/huffman/huffman/huffman/File", std::fstream::in);
    HuffmanNode* tree = huffman::encodeReturnTree(file);
    
    std::ifstream encodefile("/Volumes/My Passport for Mac/It-Tallaght/ADS2/huffman/huffman/huffman/EncodeFile", std::fstream::in);
    huffman::decode(encodefile, tree);
    
    std::cout << "finished" << std::endl;
    return 0;
}


