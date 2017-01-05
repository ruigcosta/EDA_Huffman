#pragma once

#include "stdafx.h"
#include<iostream>
#include<fstream>
#include<iomanip> //for width()
#include<string>


using namespace std;

#define width_unit 5

// Huffman Tree
class Tree
{
private:
	class Node
	{
	public:
		unsigned int freq;
		unsigned char ch;
		Node *left, *right;
		//constructor
		Node( void )
			:freq( 0 ), ch( '\0' ), left( NULL ), right( NULL )
		{}
	};

	Node *root;

	//copy cons. and assign. op. overload prototypes are private to
	//prevent them to be used
	Tree( const Tree & ); //copy constructor
	const Tree & operator=( const Tree & ); //assignment oper. overload
	void chop( Node * N ); //destroys the tree
	void print( ostream &, Node *, int ) const; //prints the tree
	void print( Node *, int ) const; //prints the tree

public:
	Tree( void ); //constructor
	~Tree( void ); //destructor
	friend ostream & operator<<( ostream &, const Tree & );
	//utility functions to get or set class members
	unsigned int get_freq( void ) const;
	unsigned char get_char( void ) const;
	void set_freq( unsigned int );
	void set_char( unsigned char );
	Node* get_left( void ) const;
	Node* get_right( void ) const;
	void set_left( Node * );
	void set_right( Node * );
	Node* get_root( void ) const;
	//comparison operator overloads to compare
	//2 objects of the class
	bool operator==( const Tree & ) const;
	bool operator!=( const Tree & ) const;
	bool operator<( const Tree & ) const;
	bool operator>( const Tree & ) const;
	bool operator<=( const Tree & ) const;
	bool operator>=( const Tree & ) const;

	//to get H. string of a given char
	void huf( Node *, unsigned char, string, string & ) const;
	//outputs the H. char-string pairs list
	void huf_list( Node *, string ) const;
	//to get char equivalent of a H. string (if exists)
	bool get_huf_char( string, unsigned char & ) const;
	string print_char( Node * ) const; //prints chars 
};

//constructor
Tree::Tree( void )
{
	Node* N = new Node;
	root = N;
}

//recursive func to delete the whole tree
void Tree::chop( Node *N )
{
	if ( N )
	{
		chop( N->left );
		chop( N->right );
		delete N;
	}
}

//destructor for tree objects
Tree::~Tree( void )
{
	chop( root );
}

unsigned int Tree::get_freq( void ) const
{
	return root->freq;
}

unsigned char Tree::get_char( void ) const
{
	return root->ch;
}

void Tree::set_freq( unsigned int f )
{
	root->freq = f;
}

void Tree::set_char( unsigned char c )
{
	root->ch = c;
}

Tree::Node* Tree::get_left( void ) const
{
	return root->left;
}

Tree::Node* Tree::get_right( void ) const
{
	return root->right;
}

void Tree::set_left( Node* N )
{
	root->left = N;
}

void Tree::set_right( Node* N )
{
	root->right = N;
}

Tree::Node* Tree::get_root( void ) const
{
	return root;
}

//the recursive tree output (w/ respect to its graph) fn.
void Tree::print( ostream & ost, Node * curr, int level ) const
{
	if ( curr ) //if the current node is not null then
	{
		print( ost, curr->right, level + 1 ); //try to go to right node
											  //output the node data w/ respect to its level
		ost << setw( level*width_unit ) << print_char( curr ) << ":"
			<< curr->freq << endl;
		print( ost, curr->left, level + 1 ); //try to go to left node
	}
}

//the recursive tree print (w/ respect to its graph) fn.
void Tree::print( Node * curr, int level ) const
{
	if ( curr ) //if the current node is not null then
	{
		print( curr->right, level + 1 ); //try to go to right node
										 //print the node data w/ respect to its level
		cout << setw( level*width_unit ) << print_char( curr ) << ":"
			<< curr->freq << endl;
		print( curr->left, level + 1 ); //try to go to left node
	}
}

//utility fn to output a tree
ostream & operator<<( ostream &ost, const Tree &t )
{
	t.print( ost, t.root, 1 );
	return ost;
}

//the comparison operator overloads to compare 2 Huffman trees

bool Tree::operator==( const Tree & T ) const
{
	return (root->freq == T.root->freq);
}

bool Tree::operator!=( const Tree & T ) const
{
	return (root->freq != T.root->freq);
}

bool Tree::operator<( const Tree & T ) const
{
	return (root->freq < T.root->freq);
}

bool Tree::operator>( const Tree & T ) const
{
	return (root->freq > T.root->freq);
}

bool Tree::operator<=( const Tree & T ) const
{
	return (root->freq <= T.root->freq);
}

bool Tree::operator>=( const Tree & T ) const
{
	return (root->freq >= T.root->freq);
}

//Huffman string finder (recursive func.)
//input : a tree node to start the search, a char to find its
//        Huffman string equivalent, current Huffman string according to
//        position on the Huffman tree, and a string (by reference) to
//        copy the resulted full Huffman string end of the search
//return: none (except Huffman string by reference)
void Tree::huf( Node* N, unsigned char c, string str, string & s ) const
{
	if ( N ) //if the node is not null
	{
		//compare char of the leaf node and the given char
		if ( !N->left && !N->right && N->ch == c )
		{
			s = str; //if the char is found then copy the H. string
		}
		else
		{
			//continue to search if the same char still not found
			huf( N->left, c, str + "0", s );
			huf( N->right, c, str + "1", s );
		}
	}
}

//Huffman char-string lister (recursive func.)
//input : a tree node to start the search, current Huffman string according to
//        position on the Huffman tree
//output: whole list of char-H. string code list of the H. tree
void Tree::huf_list( Node* N, string str ) const
{
	if ( N ) //if the node is not null
	{
		if ( !N->left && !N->right ) //if it is a leaf node
			cout << print_char( N ) << " " << str << endl;
		else
		{
			//continue to search
			huf_list( N->left, str + "0" );
			huf_list( N->right, str + "1" );
		}
	}
}

//char finder with given Huffman string
//input : a Huffman string to traverse on the H. tree and
//        a u. char by ref. to copy the char found
//return: true if a char is found else false
bool Tree::get_huf_char( string s, unsigned char & c ) const
{
	Node * curr = root;
	for ( unsigned int i = 0; i<s.size(); ++i )
	{
		if ( s[i] == '0' ) //go to left in the H. tree
			curr = curr->left;
		if ( s[i] == '1' ) //go to right in the H. tree
			curr = curr->right;
	}

	bool found = false;

	if ( !curr->left && !curr->right ) //if it is a leaf node
	{
		found = true;
		c = curr->ch;
	}

	return found;
}

//input : a H. tree node
//return: the same char as string or if the char is not printable
//        then its octal representation in the ASCII char set
string Tree::print_char( Node * N ) const
{
	string s = "";

	if ( !N->left && !N->right ) //if it is a leaf node
	{
		unsigned char c = N->ch;

		//if the char is not printable then output its octal ASCII code
		if ( iscntrl( c ) || c == 32 ) //32:blank char
		{
			//calculate the octal code of the char (3 digits)
			char* cp = new char;
			for ( int i = 0; i<3; ++i )
			{
				printf( cp, "%i", c % 8 );
				c -= c % 8;
				c /= 8;
				s = (*cp) + s;
			}
			s = '/' + s; // adds \ in front of the octal code
		}
		else
			s = c;
	}
	return s;
}

