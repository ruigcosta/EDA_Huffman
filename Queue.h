#pragma once

// (Templated d-heap) (on dynamic array of pointers)
// priority queue
// min (root=min) ((balanced) d-tree on dynamic array) d-heap
template<class T>
class Queue
{
public:

	Queue( int d = 2 ); //constructor 
	~Queue( void ); //destructor
	void enq( T* ); //enqueue (to push)
	T* deq( void ); //dequeue (to pop)
	T* front( void ); //the front element
	bool empty( void ) const; //is empty?
	bool full( void ) const; //is full?

private:

	int back; //the last element in the queue
	T* *arr; //dynamic array
	int size; //current size of the queue array
	static const int SIZE = 10; //size increment step size  
	int D; //max number of children for a parent>=2 
		   //copy constructor and assignment are hidden to protect 
	Queue( const Queue & );
	const Queue & operator=( const Queue & );

	//utility functions to fix the heap
	//when an element added or removed 
	void reheapup( int, int ); //fix heap upward
	void reheapdown( int, int ); //fix heap downward
	void swap( T* &, T* & ); //swap f. needed by reheapup/down functions

}; //end class


   // constructor (creates a new queue) 
template<class T>
Queue<T>::Queue( int d )
{
	if ( d<2 ) d = 2; //min a 2-heap is supported
	D = d;
	back = 0;
	size = SIZE;
	arr = new T*[size];
}

// is queue empty?
template<class T>
bool Queue<T>::empty( void ) const
{
	return (back <= 0);
}

// is queue full?
template<class T>
bool Queue<T>::full( void ) const
{
	return (back >= size);
}

// the front element of the queue 
template<class T>
T* Queue<T>::deq( void )
{
	if ( empty() )
	{
		cerr << "deq error! exiting..." << endl;
		exit( 1 );
	}

	T* rval = arr[0];
	arr[0] = arr[back - 1]; //the element in the back moved to front
	--back;
	reheapdown( 0, back - 1 ); //reheapdown called to fix the order back 
	return rval;
}

// a copy of the front element is returned but the queue is not changed
template<class T>
T* Queue<T>::front( void )
{
	if ( empty() )
	{
		cerr << "deq error! exiting..." << endl;
		exit( 1 );
	}

	return arr[0];
}

// a new element to put in the queue
template<class T>
void Queue<T>::enq( T* foo )
{
	if ( full() ) //if the array is full then make it larger
	{
		int nsize = size + SIZE; //the size of the new array
		T* *narr = new T*[nsize]; //new array
		for ( int i = 0; i<size; ++i ) //copy old array to the new one
			narr[i] = arr[i];
		delete[] arr; //delete reserved old array mem
		arr = narr; //pointer update
		size = nsize; //size update
	}

	//the new element added to the back of the queue
	//and the reheapup called to fix the order back
	arr[back++] = foo; //arr[back]=foo;++back;
	reheapup( 0, back - 1 );
}

// this is a recursive function to fix back the order in the queue
// upwards after a new element added back (bottom) of the queue 
template<class T>
void Queue<T>::reheapup( int root, int bottom )
{
	int parent; //parent node (in the virtual tree) of the bottom element

	if ( bottom > root )
	{
		parent = (bottom - 1) / D;

		//compare the two node and if the order is wrong then swap them
		//and make a recursive call to continue upward in the virtual tree
		//until the whole tree heap order is restored   
		if ( *arr[parent] > *arr[bottom] )
		{
			swap( arr[parent], arr[bottom] );
			reheapup( root, parent );
		}
	}
}

// this is a recursive function to fix back the order in the queue
// downwards after a new element added front (root) of the queue 
template<class T>
void Queue<T>::reheapdown( int root, int bottom )
{
	int minchild, firstchild, child;

	firstchild = root*D + 1; //the position of the first child of the root

	if ( firstchild <= bottom ) //if the child is in the queue
	{
		minchild = firstchild; //first child is the min child (temporarily)

		for ( int i = 2; i <= D; ++i )
		{
			child = root*D + i; //position of the next child
			if ( child <= bottom ) //if the child is in the queue
			{
				//if the child is less than the current min child
				//then it will be the new min child
				if ( *arr[child] < *arr[minchild] )
				{
					minchild = child;
				}
			}
		}

		//if the min child found is less then the root(parent node)
		//then swap them and call reheapdown() recursively and
		//continue to fix the order in the virtual tree downwards
		if ( *arr[root] > *arr[minchild] )
		{
			swap( arr[root], arr[minchild] );
			reheapdown( minchild, bottom );
		}
	}
}

// the values of 2 variables will be swapped
template<class T>
void Queue<T>::swap( T* &a, T* &b )
{
	T* c;
	c = a;
	a = b;
	b = c;
}

// destructor (because default dest. does not erase the array)
template<class T>
Queue<T>::~Queue( void )
{
	delete[] arr;
}


