#include<iostream>
#include<unistd.h>
using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////////

// Generic Class Node for Singly LinkedLists.

////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
class NodeS
{
	public:
		T sData;
		NodeS *nextPtr;
	
	public:
		NodeS(T sValue)
		{
			nextPtr = NULL;
			sData 	= sValue;
		}		
	
};

////////////////////////////////////////////////////////////////////////////////////////////////////////

// Generic Class Node for Doubly LinkedLists.

////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
class NodeD
{
	
	public:
		T dData;
		NodeD *nextPtr;
		NodeD *prevPtr;
	
	public:
		NodeD(T dValue)
		{
			nextPtr = NULL;
			prevPtr = NULL;
			dData 	= dValue;
		}

};

////////////////////////////////////////////////////////////////////////////////////////////////////////

// Generic Class LinkedList for all LinkedLists containing all pure virtual function definitions.

////////////////////////////////////////////////////////////////////////////////////////////////////////

template<class T>
class LinkedList
{
	public:
		virtual void insertFirst(T) = 0;
		virtual void insertLast(T) = 0;
		
		virtual void deleteFirst(void) = 0;
		virtual void deleteLast(void) = 0;
		
		virtual void insertAtPos(T,int) = 0;
		virtual void deleteAtPos(int) = 0; 
		
};

////////////////////////////////////////////////////////////////////////////////////////////////////////

// Generic Class SinglyLinearLL

////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
class SinglyLinearLL : public LinkedList<T>
{
	public:
		int iCount;
		NodeS<T> *first;

	public:
		SinglyLinearLL();
		~SinglyLinearLL();	
		
		void insertFirst(T);
		void insertLast(T);
		
		void deleteFirst(void);
		void deleteLast(void);
		
		void insertAtPos(T,int);
		void deleteAtPos(int);
		
		void displayNodes(void);
		int  countNodes(void);
		
};

template <class T>
SinglyLinearLL<T> :: SinglyLinearLL()
{
	iCount = 0;
	first  = NULL;
}

template <class T>
void SinglyLinearLL<T> :: displayNodes()
{
	
	int iCnt = 0;
	NodeS<T> *tempPtr = first;
	
	cout<<"\n";
	for(iCnt = 1; iCnt <= iCount; iCnt++)
	{
		cout<<"|"<<tempPtr->sData<<"|->";
		tempPtr = tempPtr -> nextPtr;	
	}
	cout<<"NULL"<<endl;
	
}

template <class T>
int SinglyLinearLL<T> :: countNodes()
{
	return iCount;
}
	
template <class T>
void SinglyLinearLL<T> :: insertFirst(T value)
{	
	NodeS<T> *newNode = NULL;
	
	newNode = new NodeS<T>(value);
	 	
	if(first == NULL)
	{
		first = newNode;
	}
	else
	{
		newNode->nextPtr = first;
		first = newNode;
	}
	
	iCount++;	
}

template <class T>
void SinglyLinearLL<T> :: insertLast(T value)
{
	NodeS<T> *newNode = NULL,*tempPtr = first;
	
	newNode = new NodeS<T>(value);
	 	
	if(first == NULL)
	{
		first = newNode;
	}
	else
	{
			
		while((tempPtr->nextPtr) != NULL)
		{
			tempPtr = tempPtr -> nextPtr;
		}
		
		tempPtr->nextPtr = newNode;
	}
	
	iCount++;	
}

template <class T>
void SinglyLinearLL<T> :: deleteFirst(void)
{
	if(iCount == 1)
	{
		if(first)
		{
			delete first;
		}
		first = NULL;
	}		
	else
	{
		NodeS<T> *tempPtr = first;
		
		first = first->nextPtr;
		
		if(tempPtr)
		{
			delete tempPtr;
		}	
	}
	iCount--;
}

template <class T>
void SinglyLinearLL<T> :: deleteLast(void)
{
	if(iCount == 1)
	{
		if(first)
		{
			delete first;
		}
		first = NULL;
	}		
	else
	{
		NodeS<T> *tempPtr = first;
		
		while((tempPtr->nextPtr->nextPtr) != NULL)
		{
			tempPtr = tempPtr->nextPtr;
		}
		if(tempPtr -> nextPtr)
		{
			delete tempPtr->nextPtr;
		}
		tempPtr -> nextPtr = NULL;
	}
	iCount--;
}
 
template <class T>
void SinglyLinearLL<T> :: insertAtPos(T value,int iPos)
{
	if((iPos < 1) || (iPos > iCount + 1))
	{
		fprintf(stderr,"Invalid Position\n");
		return;
	}

	if(iPos == 1)
	{
		insertFirst(value);
	}
	else if(iPos == iCount + 1)
	{
		insertLast(value);
	}
	else
	{
		int iCnt = 0;
		NodeS<T> *newNode = NULL,*tempPtr = first;
		
		for(iCnt = 1; iCnt < iPos - 1; iCnt++)
		{
			tempPtr = tempPtr -> nextPtr;
		}	
		
		newNode = new NodeS<T>(value);
		
		newNode->nextPtr = tempPtr->nextPtr;
		tempPtr->nextPtr = newNode;
		
		iCount++;
	}
}

template <class T>
void SinglyLinearLL<T> :: deleteAtPos(int iPos)
{	
	if((iPos < 1) || (iPos > iCount))
	{
		fprintf(stderr,"Invalid Position\n");
		return;
	}

	if(iPos == 1)
	{
		deleteFirst();
	}
	else if(iPos == iCount)
	{
		deleteLast();
	}
	else
	{
		int iCnt = 0;
		
		NodeS<T> *tempPtr  = first;
		NodeS<T> *tempPtrX = NULL;	
			
		for(iCnt = 1; iCnt < iPos - 1; iCnt++)
		{
			tempPtr = tempPtr -> nextPtr;
		}	
		
		tempPtrX = tempPtr->nextPtr;
		
		tempPtr->nextPtr = tempPtr->nextPtr->nextPtr;
		
		if(tempPtrX)
		{
			delete tempPtrX;
		}
									
		iCount--;
	}

}

template <class T>
SinglyLinearLL<T> :: ~SinglyLinearLL()
{
	int iCnt = 0;
	
	NodeS<T> *tempPtr = first;
	
	for(iCnt = 1; iCnt <= iCount; iCnt++)
	{
		tempPtr = first;
		first 	= first->nextPtr;
	
		if(tempPtr)
		{
			delete tempPtr;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

// Generic Class DoublyLinearLL.

////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
class DoublyLinearLL : public LinkedList<T>
{
	public:
		int iCount;
		NodeD<T> *first;

	public:
		DoublyLinearLL();
		~DoublyLinearLL();	
		
		void insertFirst(T);
		void insertLast(T);
		
		void deleteFirst(void);
		void deleteLast(void);
		
		void insertAtPos(T,int);
		void deleteAtPos(int);
		
		void displayNodes(void);
		int  countNodes(void);

};	

template<class T>
DoublyLinearLL<T> :: DoublyLinearLL()
{
	iCount = 0;
	first  = NULL;	
}

template <class T>
void DoublyLinearLL<T> :: displayNodes()
{
	
	int iCnt = 0;
	NodeD<T> *tempPtr = first;
	
	cout<<"\n";
	for(iCnt = 1; iCnt <= iCount; iCnt++)
	{
		cout<<"|"<<tempPtr->sData<<"|<=>";
		tempPtr = tempPtr -> nextPtr;	
	}
	cout<<endl;	
}

template <class T>
int DoublyLinearLL<T> :: countNodes()
{
	return iCount;
}

template<class T>
void DoublyLinearLL<T> :: insertFirst(T value)
{
	NodeD<T> *newNode = NULL;
	
	newNode = new NodeD<T>(value);

	if(iCount == 0)
	{
		first = newNode;
	}
	else
	{
		newNode->nextPtr = first;
		first->prevPtr = newNode;
		first = newNode;
	}	
	iCount++;
}

template<class T>
void DoublyLinearLL<T> :: insertLast(T value)
{	
	NodeD<T> *newNode = NULL;
	
	newNode = new NodeD<T>(value);

	if(iCount == 0)
	{
		first = newNode;
	}
	else
	{
		NodeD<T> *tempPtr = first;
		while(tempPtr->nextPtr != NULL)
		{
			tempPtr = tempPtr->nextPtr;
		}
		newNode->prevPtr = tempPtr;
		tempPtr->nextPtr = newNode;
	}	
	iCount++;
}

template<class T>
void DoublyLinearLL<T> :: deleteFirst()
{
	if(iCount == 0)
	{
		fprintf(stderr,"\nLinked list is empty\n");
		_exit(EXIT_FAILURE);		
	}
	else if(first->nextPtr == NULL)
	{
		if(first)
		{
			delete first;
		}
		first = NULL;
	}
	else
	{
		
		first = first->nextPtr;
		
		if(first->prevPtr)
		{
			delete first->prevPtr;
		}
		first->prevPtr = NULL;
	}
	iCount--;	

}

template<class T>
void DoublyLinearLL<T> :: deleteLast()
{
	if(iCount == 0)
	{
		fprintf(stderr,"\nLinked list is empty\n");
		_exit(EXIT_FAILURE);
	}
	else if(first->nextPtr == NULL)
	{
		if(first)
		{
			delete first;
		}
		first = NULL;
	}
	else
	{
		NodeD<T> *tempPtr = first;
		while(tempPtr->nextPtr->nextPtr != NULL)
		{
			tempPtr = tempPtr->nextPtr;
		}
		if(tempPtr->nextPtr)
		{
			delete tempPtr->nextPtr;
		}
		tempPtr->nextPtr = NULL;	
	}
	iCount--;	

}

template<class T>
void DoublyLinearLL<T> :: insertAtPos(T value,int iPos)
{
	if((iPos < 1) || (iPos > iCount+1))
	{
		fprintf(stderr,"\nInvalid position\n");
		return;
	}
	
	if(iPos == 1)
	{
		insertFirst(value);
	}
	else if(iPos == iCount + 1)
	{
		insertLast(value);
	}
	else
	{
		NodeD<T> *tempPtr = first;	
		
		int iCnt = 0;
		
		for(iCnt = 1; iCnt < iPos-1; iCnt++)
		{
			tempPtr = tempPtr->nextPtr;
		}
		
		NodeD<T> *newNode = NULL;
		
		newNode = new NodeD<T>(value);
		
		newNode->nextPtr = tempPtr->nextPtr;
		tempPtr->nextPtr->prevPtr = newNode;
		
		tempPtr->nextPtr = newNode;
		newNode->prevPtr = tempPtr;
		
		iCount++;
	}
}

template<class T>
void DoublyLinearLL<T> :: deleteAtPos(int iPos)
{	
	if((iPos < 1) || (iPos > iCount))
	{
		fprintf(stderr,"\nInvalid position\n");
		return;
	}
	
	if(iPos == 1)
	{
		deleteFirst();
	}
	else if(iPos == iCount)
	{
		deleteLast();
	}
	else
	{
		NodeD<T> *tempPtr  = first;
		NodeD<T> *tempPtrX = NULL;
		
		for(int iCnt = 1; iCnt < iPos - 1; iCnt++)
		{
			tempPtr = tempPtr->nextPtr;
		}
		
		tempPtrX = tempPtr->nextPtr->nextPtr;
		
		if(tempPtr -> nextPtr)
		{
			delete tempPtr->nextPtr;
		}
		
		tempPtr->nextPtr = tempPtrX;
		
		iCount--;
	}
}

template <class T>
DoublyLinearLL<T> :: ~DoublyLinearLL()
{
	int iCnt = 0;
	
	NodeD<T> *tempPtr = first;
	
	for(iCnt = 1; iCnt <= iCount; iCnt++)
	{
		tempPtr = first;
		first 	= first->nextPtr;
	
		if(tempPtr)
		{
			delete tempPtr;
		}
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////////////

// Generic Class SinglyCircularLL.

////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
class SinglyCircularLL : LinkedList<T>
{
	
	public:
		int iCount;
		NodeS<T> *first;
		NodeS<T> *last;		
	public:
		
		SinglyCircularLL();
		~SinglyCircularLL();
		
		void insertFirst(T);
		void insertLast(T);
	
		void deleteFirst(void);
		void deleteLast(void);
		
		void insertAtPos(T,int);
		void deleteAtPos(int);	

		void displayNodes(void);
		int  countNodes(void);
};

template <class T>
SinglyCircularLL<T> :: SinglyCircularLL()
{
	iCount = 0;
	first  = NULL;	
	last   = NULL;
}

template <class T>
void SinglyCircularLL<T> :: displayNodes()
{
	
	int iCnt = 0;
	
	NodeS<T> *tempPtr = first;
	
	cout<<"\n->";
	for(iCnt = 1; iCnt <= iCount; iCnt++)
	{
		cout<<"|"<<tempPtr->sData<<"|->";
		tempPtr = tempPtr->nextPtr;
	}
	cout<<"\n";
}

template <class T>
int SinglyCircularLL<T> :: countNodes()
{
	return iCount;
}

template <class T>
void SinglyCircularLL<T> :: insertFirst(T value)
{
	NodeS<T> *newNode = NULL;
	
	newNode = new NodeS<T>(value);
	
	if(iCount == 0)
	{
		first = newNode;
		last  = newNode;
	}	
	else
	{
		newNode->nextPtr = first;
		first = newNode;
	}
	last->nextPtr = first;
	
	iCount++;
}

template <class T>
void SinglyCircularLL<T> :: insertLast(T value)
{
	NodeS<T> *newNode = NULL;
	
	newNode = new NodeS<T>(value);
	
	if(iCount == 0)
	{
		first = newNode;
		last  = newNode;
	}	
	else
	{
		last->nextPtr = newNode;
		last = newNode;
	}
	last->nextPtr = first;
	
	iCount++;

}

template <class T>
void SinglyCircularLL<T> :: deleteFirst()
{
	if(iCount == 0)
	{
		cout<<"\nLinked list is empty\n";
		_exit(EXIT_FAILURE);
	}
	else if(iCount == 1)
	{
		if((first) || (last))
		{
			delete first;
			delete last;
		}
		first = NULL;
		last  = NULL;
	}
	else
	{
		first = first->nextPtr;
		
		if(last->nextPtr)
		{
			delete last->nextPtr;
		}
		
		last->nextPtr = first;
	}
	iCount--;
}

template <class T>
void SinglyCircularLL<T> :: deleteLast()
{
	if(iCount == 0)
	{
		fprintf(stderr,"\nLinked list is empty\n");
		_exit(EXIT_FAILURE);
	}
	else if(iCount == 1)
	{
		if(first)
		{
			delete first;
		}
		else
		{
			if(last)
			{
				delete last;
			}
		}
		
		first = NULL;
		last  = NULL;
	}
	else
	{
		NodeS<T> *tempPtr = first;
		
		while((tempPtr -> nextPtr) != last)
		{
			tempPtr = tempPtr -> nextPtr;
		}
		if(last)
		{
			delete last;
		}
		
		last = tempPtr;
		
		last->nextPtr = first;
	}
	iCount--;
}

template <class T>
void SinglyCircularLL<T> :: insertAtPos(T value,int iPos)
{

	if((iPos < 1) || (iPos > iCount + 1))
	{
		fprintf(stderr,"\nInvalid position\n");
		return;	
	}
	
	if(iPos == 1)
	{
		insertFirst(value);
	}
	else if(iPos == iCount + 1)
	{
		insertLast(value);	
	}
	else
	{
		int iCnt = 0;
		NodeS<T> *tempPtr = first;
		NodeS<T> *newNode = NULL;
		
		for(iCnt = 1; iCnt < iPos -1; iCnt++)
		{
			tempPtr = tempPtr -> nextPtr;
		}

		newNode = new NodeS<T>(value);
		
		newNode->nextPtr = tempPtr->nextPtr;
		tempPtr->nextPtr = newNode;
		
		iCount++;
	}

}

template <class T>
void SinglyCircularLL<T> :: deleteAtPos(int iPos)
{
	if((iPos < 1) || (iPos > iCount))
	{
		fprintf(stderr,"\nInvalid position\n");
		return;	
	}
	
	if(iPos == 1)
	{
		deleteFirst();
	}
	else if(iPos == iCount)
	{
		deleteLast();	
	}
	else
	{
		int iCnt = 0;
		
		NodeS<T> *tempPtr  = first;
		NodeS<T> *tempPtrX = NULL;
		
		for(iCnt = 1; iCnt < iPos - 1; iCnt++)
		{
			tempPtr = tempPtr -> nextPtr;
		}
		
		tempPtrX = tempPtr->nextPtr;
		
		tempPtr->nextPtr = tempPtr->nextPtr->nextPtr;
		
		if(tempPtrX)
		{
			delete tempPtrX;
		}
		
		iCount--;
	}

}

template <class T>
SinglyCircularLL<T> :: ~SinglyCircularLL()
{
	int iCnt = 0;
	
	NodeS<T> *tempPtr = first;
	
	for(iCnt = 1; iCnt <= iCount; iCnt++)
	{
		tempPtr = first;
		first 	= first->nextPtr;

		if(tempPtr)
		{
			delete tempPtr;
		}
	}
	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

// Generic Class DoublyCircularLL

////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
class DoublyCircularLL : public LinkedList<T>
{
	public:
		int iCount;
		NodeD<T> *first;
		NodeD<T> *last;	
	
	public:
		
		DoublyCircularLL();
		~DoublyCircularLL();
			
		void insertFirst(T);
		void insertLast(T);
		
		void deleteFirst(void);
		void deleteLast(void);
		
		void insertAtPos(T,int);
		void deleteAtPos(int);
		
		void displayNodes(void);
		int countNodes(void);
					
};

template <class T>
DoublyCircularLL<T> :: DoublyCircularLL()
{
	iCount = 0;
	first  = NULL;
	last   = NULL;
}

template <class T>
void DoublyCircularLL<T> :: displayNodes()
{
	int iCnt = 0;
	NodeD<T> *tempPtr = first;
	
	cout<<"\n<=>";
	for(iCnt = 1;iCnt <= iCount; iCnt++)
	{
		cout<<"|"<<tempPtr->dData<<"|<=>";
		tempPtr = tempPtr->nextPtr;
	}
	cout<<"\n";
}

template <class T>
int DoublyCircularLL<T> :: countNodes()
{
	return iCount;
}

template <class T>
void DoublyCircularLL<T> :: insertFirst(T value)
{
	NodeD<T> *newNode = NULL;
	newNode = new NodeD<T>(value);
	
	if(iCount == 0)
	{
		first = newNode;
		last  = newNode;
	}
	else
	{
		newNode->nextPtr = first;
		first->prevPtr   = newNode;
		
		first = newNode;
	}
	
	last->nextPtr = first;
	first->prevPtr = last;
	
	iCount++;
}

template <class T>
void DoublyCircularLL<T> :: insertLast(T value)
{
	NodeD<T> *newNode = NULL;
	newNode = new NodeD<T>(value);
	
	if(iCount == 0)
	{
		first = newNode;
		last  = newNode;
	}
	else
	{
		last->nextPtr = newNode;
		newNode->prevPtr = last;
		
		last = newNode;
	}
	
	last->nextPtr = first;
	first->prevPtr = last;
	
	iCount++;
}

template <class T>
void DoublyCircularLL<T> :: deleteFirst()
{
	if(iCount == 0)
	{
		fprintf(stderr,"\nLinked list is empty\n");
		_exit(EXIT_FAILURE);
	}	
	else if(iCount == 1)
	{
		if(first)
		{
			delete first;
		}
		else
		{
			if(last)
			{
				delete last;
			}
		}
	}
	else
	{
		first = first->nextPtr;
		if(last->nextPtr)
		{
			delete last->nextPtr;
		}
		
		first->prevPtr = last;
		last->nextPtr  = first;		
	}
	iCount--;
}

template <class T>
void DoublyCircularLL<T> :: deleteLast()
{
	if(iCount == 0)
	{
		fprintf(stderr,"\nLinked list is empty\n");
		_exit(EXIT_FAILURE);
	}	
	else if(iCount == 1)
	{
		if(first)
		{
			delete first;
		}
		else
		{
			if(last)
			{
				delete last;
			}
		}
	}
	else
	{
		last = last->prevPtr;
		if(last->nextPtr)
		{
			delete last->nextPtr;
		}
		
		first->prevPtr = last;
		last->nextPtr  = first;
	}
	iCount--;
}

template <class T>
void DoublyCircularLL<T> :: insertAtPos(T value,int iPos)
{
	
	if((iPos < 1) || (iPos > iCount + 1))
	{
		fprintf(stderr,"\nInvalid position\n");
		return;
	}	
	
	if(iPos == 1)
	{
		insertFirst(value);
	}
	else if(iPos == iCount + 1)
	{
		insertLast(value);
	}
	else
	{
	
		int iCnt = 0;
		NodeD<T> *newNode = NULL;
		NodeD<T> *tempPtr = first;
		
		for(iCnt = 1; iCnt < iPos - 1; iCnt++)
		{
			tempPtr = tempPtr -> nextPtr;
		}
		
		newNode = new NodeD<T>(value);
		
		newNode->nextPtr = tempPtr->nextPtr;
		tempPtr->nextPtr->prevPtr = newNode;
		
		tempPtr->nextPtr = newNode;
		newNode->prevPtr = tempPtr;
		
		iCount++;
				
	}

}

template <class T>
void DoublyCircularLL<T> :: deleteAtPos(int iPos)
{
	
	if((iPos < 1) || (iPos > iCount))
	{
		fprintf(stderr,"\nInvalid position\n");
		return;
	}	
	
	if(iPos == 1)
	{
		deleteFirst();
	}
	else if(iPos == iCount)
	{
		deleteLast();
	}
	else
	{
		int iCnt = 0;
		NodeD<T> *tempPtr = first;
		
		for(iCnt = 1; iCnt < iPos - 1; iCnt++)
		{
			tempPtr = tempPtr->nextPtr;
		}
		tempPtr->nextPtr = tempPtr->nextPtr->nextPtr;
	
		if(tempPtr->nextPtr->prevPtr)
		{
			delete tempPtr->nextPtr->prevPtr;
		}
		
		tempPtr->nextPtr->prevPtr = tempPtr;
			
		iCount--;			
	}

}

template <class T>
DoublyCircularLL<T> :: ~DoublyCircularLL()
{
	int iCnt = 0;
	NodeD<T> *tempPtr = first;
	
	for(iCnt = 1; iCnt <= iCount; iCnt++)
	{
		tempPtr = first;
		first 	= first->nextPtr;
		
		if(tempPtr)
		{
			delete tempPtr;
		}	
			
	}
}

template <class T>
class Stack
{
	public:
		int iCount;
		NodeS<T> *first;
		
	public:
		Stack();
		~Stack();		
				
		void pushElem(T);
		int  popElem();
		
		void displayNodes();
		int  countNodes();
		
};

template <class T>
Stack<T> :: Stack()
{
	iCount = 0;
	first  = NULL;
}

template <class T>
void Stack<T> :: displayNodes()
{

	if(iCount == 0)
	{
		fprintf(stderr,"Stack is empty\n");
		_exit(EXIT_FAILURE);
	}
	
	int iCnt = 0;
	NodeS<T> *tempPtr = first;
	
	cout<<"Elements in stack are : \n";
	for(iCnt = 1; iCnt <= iCount; iCnt++)
	{
		cout<<tempPtr->sData<<"\n";
		tempPtr = tempPtr->nextPtr;
	}

}

template <class T>
int Stack<T> :: countNodes()
{
	return iCount;
}

template <class T>
void Stack<T> :: pushElem(T value)
{
	
	NodeS<T> *newNode = NULL;
	newNode = new NodeS<T>(value);
	
	if(iCount == 0)
	{
		first = newNode;
	}
	else
	{
		NodeS<T> *tempPtr = first;
		
		while((tempPtr->nextPtr) != NULL)
		{
			tempPtr = tempPtr->nextPtr; 
		}
		tempPtr->nextPtr = newNode;
	}
	iCount++;
}

template <class T>
int Stack<T> :: popElem()
{
	int iValue = 0;

	if(iCount == 0)
	{
		fprintf(stderr,"Stack is empty\n");
		_exit(EXIT_FAILURE);
	}
	else if(iCount == 1)
	{		
		iValue = first->sData;
		
		if(first)
		{
			delete first;
		}
		
	}
	else
	{
		NodeS<T> *tempPtr = first;
		
		while((tempPtr->nextPtr->nextPtr) != NULL)
		{
			tempPtr = tempPtr->nextPtr;
		}
		iValue = tempPtr->nextPtr->sData;
		
		if(tempPtr->nextPtr)
		{
			delete tempPtr->nextPtr;
		}
		
		tempPtr->nextPtr = NULL;	
	}
	iCount--;
	
	return iValue;	
}

template <class T>
Stack<T> :: ~Stack()
{
	int iCnt = 0;
	NodeS<T> *tempPtr = first;
	
	for(iCnt = 1; iCnt <= iCount; iCnt++)
	{
		tempPtr = first;
		first   = first->nextPtr;
		
		if(tempPtr)
		{
			delete tempPtr;
		}
		
	}
	
}

template <class T>
class Queue
{

	public:
		int iCount;
		NodeS<T> *first;
		
	public:
		Queue();
		~Queue();
		
		void enQueue(T);
		int  deQueue(void);
		
		void displayNodes(void);
		int  countNodes(void);

};

template <class T>
Queue<T> :: Queue()
{
	iCount = 0;
	first  = NULL;
}

template <class T>
void Queue<T> :: displayNodes()
{
	int iCnt = 0;
	NodeS<T> *tempPtr = first;
	
	for(iCnt = 1; iCnt <= iCount; iCnt++)
	{
		cout<<tempPtr->sData<<"\n";
		tempPtr = tempPtr->nextPtr;
	}
	

}

template <class T>
int Queue<T> :: countNodes()
{
	return iCount;
}

template <class T>
void Queue<T> :: enQueue(T value)
{
	NodeS<T> *newNode = NULL;
	newNode = new NodeS<T>(value);
	
	if(iCount == 0)
	{
		first = newNode;
	}		
	else
	{
		NodeS<T> *tempPtr = first;
		
		while((tempPtr->nextPtr) != NULL)
		{
			tempPtr = tempPtr->nextPtr;
		}
		
		tempPtr->nextPtr = newNode;
	}
	iCount++;
}

template <class T>
int Queue<T> :: deQueue()
{

	int iValue = 0;
	
	if(iCount == 0)
	{
		fprintf(stderr,"Queue is empty\n");
		_exit(EXIT_FAILURE);
	}
	else if(iCount == 1)
	{
		iValue = first->sData;
	
		if(first)
		{
			delete first;
		}
	}
	else
	{
		iValue = first->sData;
		NodeS<T> *tempPtr = first;
		
		first = first->nextPtr;
		
		if(tempPtr)
		{
			delete tempPtr;
		}	
		
	}
	iCount--;
	
	return iValue;
}

template <class T>
Queue<T> :: ~Queue()
{
	int iCnt = 0;
	NodeS<T> *tempPtr = first;
	
	for(iCnt = 1; iCnt <= iCount; iCnt++)
	{
		tempPtr = first;
		first   = first->nextPtr;
		
		if(tempPtr)
		{
			delete tempPtr;
		}
		
	}
	
}


int main(int argc,char *argv[])
{
									
	exit(EXIT_SUCCESS);	
}	
