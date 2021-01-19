#include <iostream>
#include <string.h>
#include <fstream>
#include <stdlib.h>
using namespace std;

typedef struct Node
{
   char colour;
   Node* left;
   Node* right;
   Node* parent;
   char data[40];

}Node;

Node* root=(Node*)malloc(sizeof(Node));

Node* LoadRBTree(char data[])
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->data,data);
    newNode->left=NULL;
    newNode->right=NULL;
    newNode->parent=NULL;
    newNode->colour='R';
    return newNode;
}
Node* InsertRBTree(Node* newNode, Node* root)
{
    if(root==NULL)
        root=newNode;
    else if(strcasecmp(newNode->data,root->data)<0)
        {
            root->left=InsertRBTree(newNode,root->left);
            root->left->parent=root;
        }
    else if(strcasecmp(newNode->data,root->data)>0)
        {
            root->right=InsertRBTree(newNode,root->right);
            root->right->parent=root;
        }
    return root;
}
void PrintRBTree(Node* root)
{
    if(root!=NULL)
    {
    PrintRBTree(root->left);
    printf("%s\n",root->data);
    PrintRBTree(root->right);
    }
}
void FIX_COLOUR(Node* newNode)
{
    newNode->parent->parent->colour='R';
    newNode->parent->colour='B';
}
Node*& ROTATE_RIGHT(Node* newNode,Node*& root)
{
    Node* leftChild=newNode->left;
    newNode->left=leftChild->right;

    if(leftChild->right!=NULL)
        leftChild->right->parent=newNode;

    leftChild->parent=newNode->parent;
    if(leftChild->parent==NULL)
        root=leftChild;
    else if(newNode==newNode->parent->right)
        newNode->parent->right=leftChild;
    else
        newNode->parent->left=leftChild;

    leftChild->right=newNode;
    newNode->parent=leftChild;

    return root;
}
Node*& ROTATE_LEFT(Node* newNode,Node*& root)
{
    Node* rightChild=newNode->right;
    newNode->right=rightChild->left;

    if(rightChild->left!=NULL)
        rightChild->left->parent=newNode;

    rightChild->parent=newNode->parent;
    if(rightChild->parent==NULL)
        root=rightChild;
    else if(newNode==newNode->parent->left)
        newNode->parent->left=rightChild;
    else
        newNode->parent->right=rightChild;

    rightChild->left=newNode;
    newNode->parent=rightChild;

    return root;
}
int HeightRBTree(Node *root)
{
    if (root == NULL)
        return -1;
    else
    {
        int l = HeightRBTree(root->left);
        int r = HeightRBTree(root->right);
        return max(l,r)+1;
    }
}
int SearchRBTree(Node* root,char str[])
{
    if(root==NULL)
    {
        return 0;
    }
    if(strcmp(root->data,str)==0)
    {
         return 1;
    }
    else if(strcasecmp(str,root->data)<0)
        SearchRBTree(root->left,str);
    else if(strcasecmp(str,root->data)>0)
        SearchRBTree(root->right,str);
}
int SizeRBTree(Node* root)
{
    if(root==NULL)
        return 0;
    else
        return (SizeRBTree(root->left) + 1 + SizeRBTree(root->right));
}
Node*& FIX_VIOLATION(Node* newNode,Node*& root)
{
    Node* uncle;
    while(root!=newNode && newNode->parent->colour=='R' && newNode->colour!='B')
    {
        if(newNode->parent==newNode->parent->parent->left)
            uncle=newNode->parent->parent->right;
        else
            uncle=newNode->parent->parent->left;

        if(uncle!=NULL && uncle->colour=='R')
        {
            newNode->parent->colour='B';
            uncle->colour='B';
            newNode->parent->parent->colour='R';
            newNode=newNode->parent->parent;
        }
        else
        {
            if(newNode->parent==newNode->parent->parent->left)
            {
                if(newNode==newNode->parent->right)
                   {
                      newNode=newNode->parent;
                      root=ROTATE_LEFT(newNode,root);
                   }
                FIX_COLOUR(newNode);
                root=ROTATE_RIGHT(newNode->parent->parent,root);
            }
            else if(newNode->parent==newNode->parent->parent->right)
            {
                if(newNode==newNode->parent->left)
                   {
                       newNode=newNode->parent;
                       root=ROTATE_RIGHT(newNode,root);
                   }
                FIX_COLOUR(newNode);
                root=ROTATE_LEFT(newNode->parent->parent,root);
            }
        }
    }
    root->colour='B';
    return root;
}
void deleteRBTree(Node*& root)
{
    if(!root)
        return ;
    else
    {
        deleteRBTree(root->left);
        deleteRBTree(root->right);
        free(root);
    }
}
void READ_FILE(char str[])
{
    ifstream f;
    char temp[40];
    f.open(str);
    root->left=root->right=root->parent=NULL;
    root->colour='B';
    if(f.is_open())
    {
        f >> temp;
        strcpy(root->data,temp);
        while(!f.eof())
    {
        f >> temp;
        Node* TempNode;
        TempNode=LoadRBTree(temp);
        root=InsertRBTree(TempNode, root);
        root=FIX_VIOLATION(TempNode,root);
    }
    }
    f.close();
    int h=HeightRBTree(root);
    cout << "Height of Tree = " << h << "\n";
}
int main()
{
    int flag;
    int check;
    char word[40];
    READ_FILE("words.txt");
    cout << "----------------------------------\n";
    cout << "PLEASE SELECT A NUMBER: \n";
    cout << "1. Insert a new word in dictionary.\n";
    cout << "2. Search for a word in dictionary.\n";
    cout << "3. View the current size of dictionary.\n";
    cout << "4. View the entire dictionary and the tree height.\n";
    cout << "-----------------------------------\n";
    cin >> check;
    if(check == 1)
    {cout << "Enter the word you want to insert: \n";
    cin >> word;
    flag=SearchRBTree(root,word);
    if(!flag)
    {
        Node* newWord=LoadRBTree(word);
        root=InsertRBTree(newWord,root);
        root=FIX_VIOLATION(newWord,root);
        cout << "Done!\n";
    }
    else
        cout << "Error! Word already in dictionary.\n";
    }
    if(check==2)
    {
        cout << "Enter the word you want to search for: \n";
        cin >> word;
        flag=SearchRBTree(root, word);
        if(flag)
            cout << "Word Found!\n";
        else cout << "Word Not Found.\n";
    }
    if(check == 3)
    {
        int sizeoftree=SizeRBTree(root);
        cout << "Size of Tree = " << sizeoftree << "\n";
    }
    if(check == 4)
    {
        PrintRBTree(root);
        int h=HeightRBTree(root);
        cout << "Height of Tree = " << h << "\n";
    }
    deleteRBTree(root);
    return 0;
}
