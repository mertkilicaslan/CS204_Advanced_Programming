// Mert Kilicaslan - THE2 Circular Doubly Linked Playlist
#include <iostream>
#include <string>
using namespace std;

struct node{	
	string songName, singer, genre;
	int listenerCount, duration;

	node* next;
	node* prev;

	/* Default Constructor */
	node(){}

	/* Parameter Constructor */
	node(string SongName, string Singer, string Genre, int ListenerCount, int Duration,
		node* Next = NULL, node* Prev = NULL){

		songName = SongName;
		singer = Singer;
		genre = Genre;
		listenerCount = ListenerCount;
		duration = Duration;

		next = Next;
		prev = Prev;
	}
};

/* Displays option menu */
void displayMenu(){
	cout << "This program helps to create a music list and suggest you songs that you might like!" << endl;
	cout << "---" << endl;
	cout << "MENU" << endl;
	cout << "1. Add a song to the list" << endl;
	cout << "2. Delete a song from the list" << endl;
	cout << "3. Print the song list" << endl;
	cout << "4. Print the song list in reverse order" << endl;
	cout << "5. Show total duration of the list" << endl;
	cout << "6. Print songs from the same genre" << endl;
	cout << "7. Print songs from the same singer" << endl;
	cout << "8. Exit" << endl;
	cout << "---" << endl;
}

/* Checks wheter the given number in the range of menu option numbers */
bool isChoiceValid(const string & str){
	return str == "1" || str == "2" || str == "3" || str == "4" || str == "5" || str == "6" || str == "7" || str == "8";
}

/* Checks wheter the given song name in the linked list */
bool isSongInList(string sn, node* head){
	node* temp = head;
	
	/* Checking if the list is empty */
	if(temp == NULL)
		return false;
	
	/* Iterating the linkedlist for song match */
	do{
		if(temp->songName == sn)
			return true;

		temp = temp->next;
	} while (temp != head);

	/* if it could not find the songname in the list */
	return false;
}

/* Creates circular double linked list, also it adds new nodes with respect to their listener counts and if this is equal, their alphabetical order */
void addElementsDoubleCircular(string n, string s, string g, int d, int l, node* & head, node* & tail){
	node* ptr = new node(n,s,g,d,l); /* New node to be added */
	int number = ptr->listenerCount; /* New nodes listener count */
	string name = ptr->songName;	 /* New nodes song name */

	/* Creating a new circular doubly linked list */
	if(head == NULL){
		ptr->prev = ptr;
		ptr->next = ptr;

		head = ptr;
		tail = ptr;
	}

	/* Adds a new node to the begining of the list, if the listener counter of the new node
	is higher than the head (if they are equal, it checks the alphabetical order)*/
	else if(number > head->listenerCount || (number == head->listenerCount && head->songName.compare(name) > 0)){
		ptr->prev = tail;
		ptr->next = head;

		tail->next = ptr;
		head->prev = ptr;

		head = ptr;
	}

	/* Adds a new node to the end of the list, if the listener counter of the new node
	is lesser than the tail (if they are equal, it checks the alphabetical order)*/
	else if(number < tail->listenerCount || (number == tail->listenerCount && tail->songName.compare(name) < 0)){
		ptr->prev = tail;
		ptr->next = head;

		tail->next = ptr;
		head->prev = ptr;

		tail = ptr;
	}

	/* Adds a new node to  any point in the middle of the list, if the listener counter of the new node
	is higher than the node after (if they are equal, checks the alphabetical order of the songname)*/
	else{
		node* temp = head;

		while(temp->listenerCount > number || (number == temp->listenerCount && temp->songName.compare(name) < 0))
			temp = temp->next;

		ptr->prev = temp->prev;
		ptr->next = temp;

		temp->prev->next = ptr;
		temp->prev = ptr;
	}
}

/* Deletes given song name from the linked list */
void deleteSong(string sn, node* & head, node* & tail){
	node* temp = head;

	/* If the node will be deleted is the only node in the list */
	if(tail == head && head->songName == sn){
		delete head;
		head = NULL;
	}

	/* If the node will be deleted is the head of the list */
	else if(head->songName == sn){
		head = head->next;
		tail->next = head;
		head->prev = tail;
		delete temp;
	}
		
	/* If the node will be deleted is the tail of the list */
	else if(tail->songName == sn){
		while(temp->next != tail)
			temp = temp->next;

		temp->next = head;
		head->prev = temp;
		delete tail;
		tail = temp;
	}

	/* If the node will be deleted is in the any point of the list */
	else{
		while(temp->songName != sn && temp->next != tail)
			temp = temp->next;

		temp->prev->next = temp->next;
		temp->next->prev = temp->prev;
		delete temp;
	}		
}

/* Prints songs from ascending order of their listener counts */
void printSongListAscending(node* tail){
	node* temp = tail;

	do{
		cout << "Song Name: " << temp->songName << endl;
		cout << "Singer Name: " << temp->singer << endl;
		cout << "Genre: " << temp->genre << endl;
		cout << "Duration: " << temp->duration << endl;
		cout << "Listener Count: " << temp->listenerCount << endl;
		cout << "---" << endl;

		temp = temp->prev;
	} while(temp != tail);

}

/* Prints songs from descending order of their listener counts */
void printSongListDescending(node* head){
	node* temp = head;

	do{
		cout << "Song Name: " << temp->songName << endl;
		cout << "Singer Name: " << temp->singer << endl;
		cout << "Genre: " << temp->genre << endl;
		cout << "Duration: " << temp->duration << endl;
		cout << "Listener Count: " << temp->listenerCount << endl;
		cout << "---" << endl;

		temp = temp->next;
	} while(temp != head);

}

/* Displays the total duration of the songs in the linked list */
void displayTotalDuration(node* head){
	node* temp = head;
	int counter = 0;

	do{
		counter += temp->duration;
		temp = temp->next;

	} while(temp != head);

	cout << "Enjoy your next " << counter << " minutes with the songs in the list." << endl;
	cout << "---" << endl;
}

/* Displays the songs that are in the same genre */
void displaySameGenre(string genre, node* head){
	node* temp = head;

	cout << "List of All the Songs from Genre " << genre << endl;
	cout << "---" << endl;
	do{
		if(temp->genre == genre)
			cout << "- " << temp->songName << " from " << temp->singer << endl;

		temp = temp->next;
	} while(temp != head);

	cout << "---" << endl;
}

/* Displays the songs that are from the same singer */
void displaySameSinger(string singer, node* head){
	node* temp = head;

	cout << "List of All the Songs from Singer " << singer << endl;
	cout << "---" << endl; 
	do{
		if(temp->singer == singer)
			cout << "- " << temp->songName << endl;

		temp = temp->next;
	} while(temp != head);

	cout << "---" << endl;
}

/* Deletes all contentes of the double linked list and gives dynamically allocated memory back to heap */
void deleteLL(node* & head){

	/* Checks if the list is empty */
	if(head != NULL){
		node* temp;
		node* ptr;
		ptr = head->next;

		/* If ptr node does not equal to head, deletes ptr node and goes to the next node by temp until it becomes head */
		while(ptr != head){
			temp = ptr->next;
			delete ptr;
			ptr  = temp;
		}

		/* Deletes the head of the list*/
		delete head;
		head = NULL;
	}
}

/* Makes all letters of the string to upper case, removes all the tabs and adjacent whitespaces in it */
void toUpperString_removeWhiteSpace(string & str){
	string newStr = "";
	bool needSpace = false;

	/* It searches all the string, if the letter is lower makes it upper.
	Also, if there is a tab or multiple spaces, it skips it */
	for(unsigned int i = 0; i < str.length(); i++){
		if('a' <= str[i] && str[i] <= 'z'){
			str[i] -= ('a'-'A');

			newStr += str[i];
			needSpace = true;
		}

		/* If tab or multiple space, skips it */
		else if(str[i] == ' ' || str[i] == '\t'){
			if(needSpace){
				needSpace = false;
				newStr += " ";
			}
		}

		/* For the numbers and punctions in the string */
		else{
			newStr += str[i];
			needSpace = true;
		}
	}
	str = newStr;	
}


int main(){

	node* head = NULL, * tail = NULL;
	displayMenu();

	while(true){

		string choice;
		cout << "Enter your choice: ";
		getline(cin,choice);
		cout << "---" << endl;

		/* Input check validation */
		if(!isChoiceValid(choice)){
			cout << "Not a valid option." << endl;
			cout << "---" << endl;
			continue;
		}

		/* Adding a new song to the list (opt: 1)*/
		if(choice == "1"){

			string songName, singer, genre;
			int listenerCount, duration;

			cout << "Enter the name of the song: "; /* Songname */
			getline(cin, songName); 
			toUpperString_removeWhiteSpace(songName);

			/* If song is not in the list, take other inputs */
			if(!isSongInList(songName,head)){

				cout << "Enter singer name of the song: "; /* Singer */
				getline(cin, singer); 
				toUpperString_removeWhiteSpace(singer);

				cout << "Enter genre of the song: "; /* Genre */
				getline(cin, genre); 
				toUpperString_removeWhiteSpace(genre);

				cout << "Enter the listener count of the song: "; /* Listener Count */
				cin >> listenerCount;

				cout << "Enter the duration of the song: "; /* Duration */
				cin >> duration;

				cout << "---" << endl;
				addElementsDoubleCircular(songName,singer,genre,listenerCount,duration,head,tail);
				cout << "The song " << songName << " from " << singer << " is added to the song list." << endl;
				cout << "---" << endl;
	
				cin.ignore();
			}

			else{
				cout << "---" << endl;
				cout << "The song " << songName << " is already in the song list." << endl;
				cout << "---" << endl;
			}
		}

		/* Deleting a song from the list (opt: 2)*/
		else if(choice == "2"){

			if(head == NULL){
				cout << "The song list is empty." << endl;
				cout << "---" << endl;
			}
			
			else{
				string deleteSongName;

				cout << "Enter the name of the song: "; /* Songs will be deleted */
				getline(cin,deleteSongName);
				toUpperString_removeWhiteSpace(deleteSongName);
				cout << "---" << endl;

				/* Checks if the song will be deleted in the list or not */
				if(isSongInList(deleteSongName,head)){
					deleteSong(deleteSongName,head,tail);
					cout << "The song " << deleteSongName << " is deleted from the list." << endl;
					cout << "---" << endl;
				}
				else{
					cout << "The song " << deleteSongName << " could not be found in the list." << endl;
					cout << "---" << endl;
				}
			}
		}

		/* Print the song list DESCENDING order (opt: 3)*/
		else if(choice == "3"){

			if(head == NULL){
				cout << "The song list is empty." << endl;
				cout << "---" << endl;
			}

			else
				printSongListDescending(head);
		}

		/* Print the song list in ASCENDING order (opt: 4)*/
		else if(choice == "4"){

			if(head == NULL){
				cout << "The song list is empty." << endl;
				cout << "---" << endl;
			}

			else
				printSongListAscending(tail);
		}

		/* Show the total duration of the list (opt: 5) */
		else if(choice == "5"){

			if(head == NULL){
				cout << "The song list is empty." << endl;
				cout << "---" << endl;
			}

			else
				displayTotalDuration(head);
		}
	
		/* Print songs from the same genre (opt: 6) */
		else if(choice == "6"){

			if(head == NULL){
				cout << "The song list is empty." << endl;
				cout << "---" << endl;
			}

			else{
				string sameGenre;
				cout << "Please enter the genre of the songs you want to list: ";
				getline(cin,sameGenre);
				toUpperString_removeWhiteSpace(sameGenre);
				cout << "---" << endl;
				
				displaySameGenre(sameGenre, head);
			}
		}

		/* Print songs from the same singer (opt: 7) */
		else if(choice == "7"){

			if(head == NULL){
				cout << "The song list is empty." << endl;
				cout << "---" << endl;
			}

			else{
				string sameSinger;
				cout << "Please enter the singer name of the songs you want to list: ";
				getline(cin, sameSinger);
				toUpperString_removeWhiteSpace(sameSinger);
				cout << "---" << endl;

				displaySameSinger(sameSinger,head);
			}
		}

		/* Deleting list and exit (opt: 8) */
		else if(choice == "8"){
			deleteLL(head);
			cout << "Deleting song list..." << endl;
			cout << "Exiting the program..." << endl;
			break;
		}	
	}

	return 0;
}
