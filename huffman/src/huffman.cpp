//--- 2022-2023 Fall Semester Data Structure Assignment 2 ---//
//--------------------------//
//---Name & Surname: Erol Koçoğlu
//---Student Number: 150200107
//--------------------------//

//-------------Do Not Add New Libraries-------------//
//-------------All Libraries Needed Were Given-------------//
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <bits/stdc++.h>

#include "huffman.h"
#include "structs.h"

using namespace std;

//-------------Complete The Functions Below-------------//

//-----------------------------------------------------//
//-----------Reads the key from text file--------------//
//------------DO NOT CHANGE THIS FUNCTION--------------//
//-----------------------------------------------------//
void Huffman::readKey(const char* argv){
    ifstream readKeyFile;
    readKeyFile.open(argv);

    if(readKeyFile.is_open()){
        while(!readKeyFile.eof()){
            readKeyFile >> key;
        }
    }
    readKeyFile.close();
};
//-----------------------------------------------------//

//-----------------------------------------------------//
//---------Sorts the key in an alpabetic order---------//
//------------DO NOT CHANGE THIS FUNCTION--------------//
//-----------------------------------------------------//
void Huffman::sortKey(){
    sortedKey = key;
    sort(sortedKey.begin(), sortedKey.end());
};
//-----------------------------------------------------//


//-----------------------------------------------------//
//-------Finds the frequency of the characters---------//
//-----------------------------------------------------//
void Huffman::findFrequency(){
    //DO NOT CHANGE THIS
    sortKey();
    //DO NOT CHANGE THIS
    int typeCount=1;
    char currentSymbol=sortedKey[0];

    for(int i=1;(unsigned)i<sortedKey.length();i++){
        if(sortedKey[i]!=currentSymbol){
            typeCount++;
            currentSymbol=sortedKey[i];
        }
    }//Find how many letters this alphabet contains

    int currentIndex=0;
    Node* nodes[typeCount];
    nodes[0]=new Node();
    nodes[0]->token.symbol=sortedKey[0];
    nodes[0]->token.val++;

    for(int i=1;(unsigned)i<sortedKey.length();i++){
        if(nodes[currentIndex]->token.symbol.compare(sortedKey.substr(i,1))==0){
            nodes[currentIndex]->token.val++;
        }else{
            queue.enque(nodes[currentIndex]);
            currentIndex++;
            nodes[currentIndex]=new Node();
            nodes[currentIndex]->token.symbol=sortedKey[i];
            nodes[currentIndex]->token.val++;
        }
    }//Fill the priority queue

    queue.enque(nodes[currentIndex]);


    //TODO
};
//-----------------------------------------------------//


//-----------------------------------------------------//
//----------------Creates Huffman Tree-----------------//
//-----------------------------------------------------//
void Huffman::createHuffmanTree(){
    //TODO
    findFrequency();
    Node* merged;
    while(queue.head->next){
        Node* temp1=queue.dequeue();
        Node* temp2=queue.dequeue();
        merged=huffmanTree.mergeNodes(temp1,temp2);
        
        queue.enque(merged);
        
        
    }
    huffmanTree.root=merged;

};
//-----------------------------------------------------//

//-----------------------------------------------------//
//---------------Prints the Huffman Tree---------------//
//------------DO NOT CHANGE THIS FUNCTION--------------//
//-----------------------------------------------------//
void Huffman::printHuffmanTree(){
    huffmanTree.printTree(huffmanTree.root, 0);
};
//-----------------------------------------------------//

//-----------------------------------------------------//
//-------------Finds and returns the binary------------//
//----------------value to given character-------------//
//-----------------------------------------------------//
string Huffman::getTokenBinary(char tokenChar, Node* traverse, string tokenBinary){
    //TODO
    while(traverse->left!=NULL && traverse->right!=NULL){
       if(traverse->right->token.symbol.find(tokenChar)!=std::string::npos){
            tokenBinary+= "1";//If right child contains the char, we move to right
            traverse=traverse->right;
        }else if(traverse->left->token.symbol.find(tokenChar)!=std::string::npos){
            tokenBinary+= "0";//If left child contains the char, we move to left
            traverse=traverse->left;
        } 
    }
    
    return tokenBinary;
    
};
//-----------------------------------------------------//

//-----------------------------------------------------//
//--------------Encodes the given password-------------//
//-----------------------------------------------------//
void Huffman::encodePassword(string password){
    //TODO
    string part="";
    for(int i=0;(unsigned)i<password.length();i++){

        part=getTokenBinary(password[i],huffmanTree.root, part);
        encodedValPassword+=to_string(part.length());//Add the depth of node
        encodedBinaryPassword+=part;
        part="";
    }
};
//-----------------------------------------------------//

//-----------------------------------------------------//
//-------------Prints the encoded password-------------//
//------------DO NOT CHANGE THIS FUNCTION--------------//
//-----------------------------------------------------//
void Huffman::printEncodedPassword(){
    cout << "Encoded Password Binary: " << encodedBinaryPassword << endl;
    cout << "Encoded Password Value: " << encodedValPassword << endl;
};

//-----------------------------------------------------//
//--------------Decodes the given password-------------//
//-----------------------------------------------------//
void Huffman::decodePassword(string encodedBinaryPassword, string encodedValPassword){
    //TODO
    int arr[encodedValPassword.length()];
    for(int i=0;(unsigned)i<encodedValPassword.length();i++){
        arr[i]=stoi(encodedValPassword.substr(i,1));
    }

    string temp=encodedBinaryPassword;
    int total=0;

    for(int i=0;(unsigned)i<encodedValPassword.length();i++){
        temp=temp.substr(0,arr[i]);
        decodeToken(temp);
        total+=arr[i];
        temp=encodedBinaryPassword.substr(total);

    }
};

//-----------------------------------------------------//

//-----------------------------------------------------//
//---------------Decodes the given binary--------------//
//-----------------------------------------------------//
void Huffman::decodeToken(string encodedToken){
    //TODO
    Node* traverse=huffmanTree.root;
    int i=0;
    while((unsigned)i<encodedToken.length()){
        if(encodedToken[i]=='1'){
            traverse=traverse->right;
        }else if(encodedToken[i]=='0'){
            traverse=traverse->left;
        }
        i++;
    }
    
    decodedPassword+=traverse->token.symbol;
    
};
//-----------------------------------------------------//

//-----------------------------------------------------//
//-------------Prints the decoded password-------------//
//------------DO NOT CHANGE THIS FUNCTION--------------//
//-----------------------------------------------------//
void Huffman::printDecodedPassword(){
    cout << "Decoded Password: " << decodedPassword << endl;
};