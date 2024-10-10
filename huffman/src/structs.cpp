//--- 2022-2023 Fall Semester Data Structure Assignment 2 ---//
//--------------------------//
//---Name & Surname: Erol Koçoğlu
//---Student Number: 150200107
//--------------------------//

//-------------Do Not Add New Libraries-------------//
//-------------All Libraries Needed Were Given-------------//
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>

#include "structs.h"

using namespace std;

//-------------Complete The Functions Below-------------//

//-------------Initialize Priority Queue-------------//
PriorityQueue::PriorityQueue(){
    //TODO
    head=NULL;
};



//-------------Insert New Node To Priority Queue-------------//
void PriorityQueue::enque(Node* newnode){
    //TODO
    if(head==NULL){
        head=newnode;
        return;
    }

    Node* traversing=head;
    if(newnode->token.val<head->token.val){
        newnode->next=head;
        head=newnode;
        return;
    }

    while(newnode->token.val>=traversing->token.val){
        if(traversing->next==NULL){
            break;
        }

        if(traversing->next->token.val>newnode->token.val){
            break;
        }
        traversing=traversing->next;
        
        
    }
    newnode->next=traversing->next;
    traversing->next=newnode;
};

//-------------Remove Node From Priority Queue-------------//
Node* PriorityQueue::dequeue(){
    //TODO
    Node* temp=head;
    head=head->next;
    return temp;     
};

//-------------Initialize Tree-------------//
Tree::Tree(){
    //TODO

    root =NULL;
};

//-------------Delete Tree Nodes From Memory-------------//
Tree::~Tree(){
    //TODO
    deleteTree(root);

};

//-------------Delete Tree From Memory-------------//
void Tree::deleteTree(Node* node){
    //-------------This Function Is Not Necessary-------------//
    //-----------------Use It At Your Own Will----------------//

    if(node==NULL){
        return;
    }

    deleteTree(node->left);
    deleteTree(node->right);

    delete node;
    
    return;
}

//-------------Merges Two Node Into One-------------//
Node* Tree::mergeNodes(Node* temp1, Node* temp2){
   //TODO
   if(temp1==NULL || temp2==NULL){
    return NULL;
   }
   Node* merged=new Node();
   merged->token.symbol=temp1->token.symbol + temp2->token.symbol;
   merged->token.val=temp1->token.val + temp2->token.val;
   merged->left=temp1;
   merged->right=temp2;
   
   return merged;
};

void Tree::printTree(Node* traverse, int level){
    cout << level << "\t";
    for (int i = 0; i < level; i++)
        cout << "\t"; 

    cout << traverse->token.symbol << "(" << traverse->token.val << ")" << endl;

    if(traverse->left)
        printTree(traverse->left, level + 1);  
    if(traverse->right)
        printTree(traverse->right, level + 1);
};