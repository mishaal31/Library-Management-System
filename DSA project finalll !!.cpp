#include <iostream>
#include <string>
using namespace std;
// Book class to represent book details
class Book
{
    public:
       string isbn, title, author;
       bool available;  // Book availability
       
    // Constructor 
    Book(string isbn, string title, string author) : isbn(isbn), title(title), author(author), available(true) {}
    
    // Function to Display book details
    void display() 
	{ 
	   cout<<"ISBN:"<<isbn<<", Title:"<<title<<", Author:"<<author<<", Available:"<<(available?"Yes":"No")<<endl; 
	}
};
// BookNode class in the book tree binary search tree for books
class BookNode
{ 
    public:
        Book* book;
        BookNode* left;
        BookNode* right;
    
	// Constructor to initialize node
    BookNode(Book* book) : book(book), left(nullptr), right(nullptr) {}
};

   // User class for user details
class User
{
    public:
       string userId, name;
       
    // Constructor to initialize user
    User(string userId, string name) : userId(userId), name(name) {}
    
    // Display user details
    void display() 
	{ 
	  cout<<"User ID:"<<userId<<", Name:"<<name<<endl; 
	}
};

  // UserNode class for linked list in user hash table
class UserNode
{
    public:
         User* user;
         UserNode* next;
         
   // Constructor to initialize node with user
    UserNode(User* user) : user(user), next(nullptr) {}
};

  // UserHashTable for managing users
class UserHashTable
{
    private:
        static const int SIZE = 100;  // Size of the hash table
        UserNode* table[SIZE];  // Array to hold user nodes
        
    // Hash function to calculate index for userId
    int hashFunction(string userId)
    {
        int hash = 0;
        for (char c : userId)
            hash = (hash * 31 + c) % SIZE;
        return hash;
    }
   public:
   	
    // Constructor
    UserHashTable() 
	{ 
	  for (int i = 0; i < SIZE; ++i) 
	    table[i] = nullptr; 
	}
	
    // Destructor to clean up the memory
    ~UserHashTable()
    {
        for (int i = 0; i < SIZE; ++i)
        {
            while (table[i])
            {
                UserNode* temp = table[i];
                table[i] = table[i]->next;
                delete temp;
            }
        }
    }
    
       // Insert user into hash table
    void insert(User* user)
    {
        int index = hashFunction(user->userId);  // Get the index using hash function
        UserNode* newNode = new UserNode(user);  // Create a new user node
        newNode->next = table[index];  // Link the new node
        table[index] = newNode;  // Update the table
    }
    
      // Search for user by userId
    User* search(string userId)
    {
        int index = hashFunction(userId);  // Get the index using hash function
        UserNode* temp = table[index];  // Start at the head of the linked list
        while (temp)
        {
            if (temp->user->userId == userId) 
			return temp->user;  // If found, return the user
            temp = temp->next;  // Move to the next node
        }
        return nullptr;  // Return nullptr if user not found
    }
};

 // Queue class to manage book issue queue
class Queue
{
   private:
    struct Node
    {
        string userId;  // User ID
        Node* next;  // Pointer to next node
        
        // Constructor to initialize the node with userId
        Node(string userId) : userId(userId), next(nullptr) {}
    };
    Node* front;  // Front of the queue
    Node* rear;  // Rear of the queue
    
   public:
   	
    // Constructor 
    Queue() : front(nullptr), rear(nullptr) {}
    
    // Destructor to clean up the memory
    ~Queue()
    {
        while (front)
        {
            Node* temp = front;
            front = front->next;
            delete temp;
        }
    }
    
     // Add user to the queue
    void enqueue(string userId)
    {
        Node* newNode = new Node(userId);  // Create a new node
        if (rear == nullptr)
            front = rear = newNode;  // If the queue is empty, set both front and rear
        else
        {
            rear->next = newNode;  // Add the new node to the rear
            rear = newNode;  // Update the rear
        }
    }
    
  // Remove user from the queue
    string dequeue()
    {
        if (front == nullptr) 
		{ 
		  cout<<"Request queue is empty!"<<endl;
		  return ""; 
		}
        Node* temp = front;
        string userId = temp->userId;
        front = front->next;  // Move front to the next node
        if (front == nullptr) 
		rear = nullptr;  // If the queue becomes empty, reset rear
        delete temp;  // Delete the old node
        cout<<"User ID "<<userId<<" removed from the issue queue."<<endl;
        return userId;  // Return the removed userId
    }
};

// Stack class to handle undo functionality
class Stack
{
   private:
    struct Node
    {
        string userId, isbn, action;  // Action details
        Node* next;  // Pointer to the next node
		        
		// Constructor to initialize the node
        Node(string userId, string isbn, string action) : userId(userId), isbn(isbn), action(action), next(nullptr) {}
    };
      Node* top;  // Top of the stack
   public:
    // Constructor to initialize the stack
    Stack() : top(nullptr) {}
    
    // Push an action into the stack
   void push(string action, string isbn, string userId = "")
   {	
      Node* newNode = new Node(userId, isbn, action);  // Create a new node with userId
      newNode->next = top;  // Link the new node to the top
      top = newNode;  // Update the top
   }
   
    // Pop an action from the stack
    bool pop(string& userId, string& isbn, string& action)
    {
        if (!top) return false;  // Return false if the stack is empty
        Node* temp = top;
        userId = temp->userId;
        isbn = temp->isbn;
        action = temp->action;
        top = top->next;  // Move top to the next node
        delete temp;  // Delete the old node
        return true;  // Return true indicating the action was popped successfully
    }
    
    // Check if the stack is empty
    bool isEmpty() 
	{ 
	  return !top; 
	}
};

 // LibraryManagementSystem class to manage library operations
class LibraryManagementSystem
{
    private:
       BookNode* bookRoot;  // Root of the book binary tree
       UserHashTable userHashTable;  // Hash table for users
       Queue issueQueue;  // Queue for book issues
       Stack undoStack;  // Stack for undo actions
    
	// Function to insert a book node in the binary search tree
    void insertBookNode(BookNode*& node, Book* book)
    {
        if (node == nullptr) 
		node = new BookNode(book);  // If the node is empty, create a new node
        else if (book->isbn < node->book->isbn) 
		insertBookNode(node->left, book);  // Search in the left subtree
        else insertBookNode(node->right, book);  //Search in the right subtree
    }
    
    // Function to search for a book node by ISBN
    Book* searchBookNode(BookNode* node, string isbn)
    {
        if (node == nullptr) // If the node is empty, return nullptr
		return nullptr; 
        if (node->book->isbn == isbn) // If ISBN matches, return the book
		return node->book;  
        if (isbn < node->book->isbn) 
		return searchBookNode(node->left, isbn); // Search in the left subtree
      return searchBookNode(node->right, isbn);  // Search in the right subtree
    }
    
    // Function to display book details in the binary search tree
    void displayBookNode(BookNode* node)
    {
        if (node != nullptr)
        {
            displayBookNode(node->left);  // Traverse left subtree
            node->book->display();  // Display book details
            displayBookNode(node->right);  // Traverse right subtree
        }
    }
   public:
    // Constructor
    LibraryManagementSystem() : bookRoot(nullptr) {}
    
    // Add a new book to the system
    void addBook(string isbn, string title, string author)
    {
        Book* book = new Book(isbn, title, author);
        if (searchBookNode(bookRoot, isbn)) 
		cout<<"Book with this ISBN already exists!"<<endl;
        else
        {
            insertBookNode(bookRoot, book);
            undoStack.push("add", isbn);  // Push add action to undo stack
            cout<<"Book added successfully!"<<endl;
        }
    }
    
     // Display all books in the system
    void displayBooks()
    {
        if (bookRoot == nullptr) 
		  cout<<"No books available to display!"<<endl;
        else displayBookNode(bookRoot);  // Display all books in the tree
    }
    
      // Add a user to the system
    void addUser(string userId, string name)
    {
        User* user = new User(userId, name);
        userHashTable.insert(user);  // Insert user into the hash table
        cout<<"User "<<name<<" ("<<userId<<") added successfully!"<<endl;
    }
	     
	 // Issue a book to a user
    void issueBook(string userId, string isbn)
   {
	   // Search for user
       User* user = userHashTable.search(userId);  
       // Search for book
       Book* book = searchBookNode(bookRoot, isbn);  
    if (user == nullptr) 
    { 
        cout<<"User not found!"<<endl; 
        return; 
    }
    if (book == nullptr) 
    { 
        cout<<"Book not found!"<<endl; 
        return; 
    }
    if (!book->available) 
    { 
        cout<<"Book is not available!"<<endl; 
        return; 
    }
    book->available = false;  // Mark the book as issued
    issueQueue.enqueue(userId);  // Add user to the issue queue
    undoStack.push("issue", isbn, userId);  // Push "issue" action with userId to undo stack
    cout<<"Book issued to user: "<<userId<<endl;
}
    // Return a book
    void returnBook(string userId, string isbn)
    {
        Book* book = searchBookNode(bookRoot, isbn);
        if (book && !book->available) 
		{ 
		  book->available = true; 
		  undoStack.push("return", isbn); 
		  cout<<"Book returned successfully!"<<endl; 
		}
        else 
		  cout<<"Book not issued or already available!"<<endl;
       }
   void undo()
   {
       string userId, isbn, action;
    
      // Check if an action exists in the stack
      if (undoStack.pop(userId, isbn, action))  
     {
    	// Find the book associated with the ISBN
        Book* book = searchBookNode(bookRoot, isbn);  
        if (!book) 
        { 
            cout<<"Undo failed! Book not found."<<endl;
            return;
        }
        // Handle the 'add' action
        if (action == "add") 
        {
            // Undo the add action by removing the book from the tree
            BookNode* parent = nullptr;
            BookNode* current = bookRoot;
            while (current != nullptr && current->book->isbn != isbn) 
            {
                parent = current;
                if (isbn < current->book->isbn) 
                    current = current->left;
                else 
                    current = current->right;
            }
            if (current == nullptr) 
            {
                cout<<"Undo failed! Book not found in tree."<<endl;
                return;
            }
            // Remove the book node
            if (parent == nullptr) 
                bookRoot = nullptr;  // If it's the root node
            else if (parent->left == current) 
                parent->left = nullptr;
            else 
                parent->right = nullptr;

            delete current->book;
            delete current;  // Delete the book node from the tree
            cout<<"Undo: Book addition undone."<<endl;
        }
        // Handle the 'issue' action
        else if (action == "issue") 
        {
            book->available = true;  // Undo: Make the book available again
            cout<<"Undo: Book issue undone."<<endl;
        }
        // Handle the 'return' action
        else if (action == "return") 
        {
            book->available = false;  // Undo: Mark the book as unavailable after return
            cout<<"Undo: Book return undone."<<endl;
        }
        else 
        {
            cout<<"Undo failed! Unknown action: "<<action<<endl;
        }
     }
      else 
       {
          cout<<"No actions to undo!"<<endl;
       }
   }
   
    // Search for a book by ISBN
    void searchBook(string isbn)
    {
        Book* book = searchBookNode(bookRoot, isbn);
        if (book) 
		{ 
		  cout<<"Book found:"<<endl; 
		  book->display(); 
		}
        else cout<<"Book with ISBN "<<isbn<<" not found!"<<endl;
    }
};

  // library management system Operations 
int main()
{
    LibraryManagementSystem lms;   // Create a LibraryManagementSystem object
    int option;
    string isbn, title, author, userId, name, userType;
    do
    {     // Display the menu
        cout<<"\n===============================\n  LIBRARY MANAGEMENT SYSTEM  \n===============================\n";
        cout<<"\nSelect an option:\n";
        cout<<"----------------------------------\n";
        cout<<"1. Add Book\n2. Add User\n3. Display Books\n4. Issue Book\n5. Return Book\n";
        cout<<"6. Search Book\n7. undo\n8. Exit\n9. Clear screen\n";
        cout<<"Enter your choice:";
		cin>>option;
        
		switch (option)
        {
        case 1:
            cout<<"Enter ISBN:";
			cin>>isbn; 
			cin.ignore();
            cout<<"Enter Book Title:";
			getline(cin,title);
            cout<<"Enter Author's Name:";
			getline(cin,author);
            lms.addBook(isbn, title, author);  // Add book to the system
            break;
        case 2:
            cout<<"Select role:\n1. Faculty\n2. Student\nChoice:";
			cin>>userType;
            if (userType == "1" || userType == "2")
            {
                cout<<"Enter User ID:"; 
				cin>>userId; 
				cin.ignore();
                cout<<"Enter Name:"; 
				getline(cin, name);
                lms.addUser(userId, name);  // Add user to the system
                if (userType == "1") 
				cout<<"Faculty "<<name<<" ("<<userId<<") registered."<<endl;
                else 
				cout<<"Student "<<name<<" ("<<userId<<") registered."<<endl;
            }
            else cout<<"Invalid input!"<<endl;
            break;
        case 3:
            lms.displayBooks();  // Display all books in the system
            break;
        case 4:
            cout<<"Enter ISBN to issue:";
			cin>>isbn;
            cout<<"Enter User ID:";
			cin>>userId;
            lms.issueBook(userId, isbn);  // Issue a book to the user
            break;
        case 5:
            cout<<"Enter User ID:"; 
			cin>>userId;
            cout<<"Enter ISBN to return:"; 
			cin>>isbn;
            lms.returnBook(userId, isbn);  // Return a book
            break;
        case 6:
            cout<<"Enter ISBN to search:";
			cin>>isbn;
            lms.searchBook(isbn);  // Search for a book
            break;
        case 7:
            lms.undo();
            break;
        case 8:
            cout<<"Exiting...\n";  // Exit the system
            break;
        case 9:
            system("cls");  // Clear the screen
            break;
        default:
            cout<<"Invalid choice! Try again.\n";  // Handle invalid choice
        }
    } 
	while (option != 8);  // Continue until exit is chosen
    return 0; 
}