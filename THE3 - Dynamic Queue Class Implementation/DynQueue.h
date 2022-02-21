// Mert Kilicaslan - THE3 Dynamic Queue Class Implementation
#ifndef DYNQUEUE_H
#define DYNQUEUE_H
using namespace std;

template <class Type>
struct QueueNode{
	Type value;
	QueueNode* next;

    QueueNode(){}
    QueueNode(Type num, QueueNode* ptr = NULL){
		value = num;
		next = ptr;
	}
};

template <class Type>
class DynQueue{

private:
	// These track the front and rear of the queue
	QueueNode<Type>* front;
	QueueNode<Type>* rear;

public:
	
	DynQueue();						  // Constructor
	DynQueue(const DynQueue<Type> &); // Deep Copy Constructor
	~DynQueue();					  // Destructor
	
	// Member functions
	void enqueue(Type);
	void dequeue(Type &);
	bool isEmpty() const;
	void clear();

	const DynQueue<Type> & operator+=(const Type &);
	const DynQueue<Type> & operator*=(const int &);
	const DynQueue<Type> & operator=(const DynQueue<Type> &);

	// Friend functions
	template <class Type2> friend ifstream & operator>>(ifstream &, DynQueue<Type2> &);
	template <class Type2> friend ostream & operator<<(ostream &, const DynQueue<Type2> &);
	template <class Type2> friend DynQueue<Type2> operator+(const DynQueue<Type2> &, const DynQueue<Type2> &);
};



// Constructor
template <class Type>
DynQueue<Type>::DynQueue(){
	front = NULL;
	rear = NULL;   
}

// Function isEmpty returns true if the queue is empty, and false otherwise
template <class Type>
bool DynQueue<Type>::isEmpty() const{
	return front == NULL;
}

// Function clear dequeues all the elements in the queue
template <class Type>
void DynQueue<Type>::clear(){
	Type v;   // Dummy variable for dequeue

	while(!isEmpty())
		dequeue(v);
}

// Function enqueue inserts the value in num at the rear of the queue
template <class Type>
void DynQueue<Type>::enqueue(Type num){
	// If the queue is empty, make it the first element (DEEP COPYING)
    if(isEmpty()){
		front = new QueueNode<Type>(num); // It creates dynamically allocated memory 'deep copy'
		rear = front;
	}

	// If the queue is not empty, add it after rear (DEEP COPYING)
    else{
		rear->next = new QueueNode<Type>(num); // It creates dynamically allocated memory 'deep copy'
		rear = rear->next;
	} 
}

// Function dequeue removes the value at the front of the queue, and copies it into num
template <class Type>
void DynQueue<Type>::dequeue(Type & num){
	if(!isEmpty()){
		num = front->value;
		QueueNode<Type>* temp = front->next;
		delete front;
		front = temp;

		if(front == NULL)
			rear = NULL;
	}
}
 
// Extraction >>
template <class Type>
ifstream & operator>>(ifstream & is, DynQueue<Type> & obj){
	Type num;

	while(is >> num)
		obj.enqueue(num);

	return is;
}

// Insertion <<
template <class Type>
ostream & operator<<(ostream & os, const DynQueue<Type> & obj){
	QueueNode<Type>* temp = obj.front;

	while(temp != NULL){
		os << temp->value << " ";
		temp = temp->next;
	}
	return os;
}

// Operator +
template <class Type>
DynQueue<Type> operator+(const DynQueue<Type> & lhs, const DynQueue<Type> & rhs){
	QueueNode<Type>* temp = rhs.front;
	QueueNode<Type>* temp2 = lhs.front;
	DynQueue<Type> result;

	while(temp != NULL){
		result.enqueue(temp->value + temp2->value);

		temp = temp->next;
		temp2 = temp2->next;
	}
	return result;
}

// Operator +=
template <class Type>
const DynQueue<Type> & DynQueue<Type>::operator+=(const Type & rhs){
	enqueue(rhs); // Inserts the value in rhs at the rear of the queue
	return *this; // Assignments must return lhs
}

// Operator *=
template <class Type>
const DynQueue<Type> & DynQueue<Type>::operator*=(const int & rhs){
	QueueNode<Type>* temp = front;

	while(temp != NULL){
		int counter = 0; // Counter for how many times it going to add
		Type tempVal = temp->value; // Temp value for inital state of the elements

		while(counter < rhs-1){ // Uses (rhs-1) since '1*1 = 1'
			temp->value += tempVal;
			counter++;
		}
		temp = temp->next;
	}
	return *this; // Assignments must return lhs
}

// Operator =
template <class Type>
const DynQueue<Type> & DynQueue<Type>::operator=(const DynQueue<Type> & rhs){
	QueueNode<Type>* temp = rhs.front;

	if(this != &rhs){ // Guard for 'a' = 'a'
		clear(); // Deletes the lhs - new data is coming, so one old must go

		while(temp != NULL){
			enqueue(temp->value); // Deep copy happens in enqueue member function
			temp = temp->next;
		}
	}
	return *this; // All assingments should return lhs for 'a = b = c'
}

// Deep Copy Constructor
template <class Type>
DynQueue<Type>::DynQueue(const DynQueue<Type> & copy){
	QueueNode<Type>* temp = copy.front;

	front = rear = NULL; // Queue front & rear initialization to enter the enqueue function
	
	// Deep copy happens in the enqueue member function, it creates new dynamically allocated memory and adds node to it
	while(temp != NULL){
		enqueue(temp->value);
		temp = temp->next;
	}
}

// Destructor
template <class Type>
DynQueue<Type>::~DynQueue(){
	QueueNode<Type>* temp = front;

	while(front != NULL){
		temp = temp->next;
		delete front;
		front = temp;
	}
}

#endif
