#include <stdio.h>
#include <iostream>
#include <string>
// R - # of columns
// L - # of rows
// N - player moves

// Inventory - F F  //items that player can use
// Bag - G D G G    //items in player bag

/*
Inventory MOVES: Queue
F:Flashlight
M: Magnet, dig on all the L, smallest L first, if special do it 

Special effect on map
B: Bomb
C:Lucky Clover
P: Pig

Player Inventory: Stack
G: Gold
D: Daimond

Player moves:
DIG # 
USE 
*/
using namespace std;
const int MAX_SIZE = 100;

class STACK {
private:
    string arr[MAX_SIZE];
    int size;

public:
    STACK() {
        size = 0;
    }

    bool stack_empty() {
        if (size == 0) {
            return true;
        } else {
            return false;
        }

    }

    bool full() {
        return size == MAX_SIZE;
    }

    string top() {
        if (!stack_empty()) {
            return arr[size-1];
        }
        // Handle the case when the stack is empty
        // You can return a default value or throw an exception here
        return ""; // For simplicity, returning an empty string
    }

    void push(string element) {
        if (!full()) {
            arr[size++] = element; // Insert the new element at the end of the array
        } else {
            // cout << "Stack full";
            return;
        }
    }

    void pop() {
        if (!stack_empty()) {
            size--; // Decrease the size to remove the last element
        }
    }

    void printStack() {
        for (int i = 0; i <size; i++) {
            cout << arr[i] << " ";
        }
        cout << endl;
    }
};

class Queue{
    private:
        string arr[MAX_SIZE];
        int front;
        int rear;
        
    public:
        Queue(){
            front = 0;
            rear = -1;
        }
        
        bool queue_empty(){
            //cout<< "The front is"<<" "<<front<<"The rear is: "<< " "<<rear<<endl;
            if(front > rear){
                //cout<<"The empty function is true"<<endl;
                return true;
            }else{
                //cout<<"The empty function is false"<<endl;
                return false;
            }
        }
        
        bool full(){
            if(rear == MAX_SIZE - 1){
                return true;
            }else{
                return false;
            }
        }
        
        void enqueue(string element){
            rear++;
            //add element to rear
            arr[rear] = element;
        }
        
        string dequeue(){
            //cout<<"In here"<<endl;
            //cout<<arr[front]<<endl;
            string dequeue_element = arr[front];
            front++;
            //cout<<"Can not return the string"<<endl;
            //cout<<dequeue_element;
            return dequeue_element;
        }
        
        void printQueue(){
            for (int i = front; i <= rear; ++i) {
                cout << arr[i] << " ";
            }
            cout <<endl;
        }

        void addNewData(string newData) {
            // Move existing data to the right to make space for new data
            for (int i = rear; i >= front; i--) {
                arr[i + 1] = arr[i];
            }
            //Add the new value to front
            arr[front] = newData;

            // Update rear index
            rear ++;
            //printQueue();
        }
};

int highest_level_finder(string** map, int L , int R) {
    int highest_level = 0;
    for (int i = L - 1; i >= 0; i--) {
        for (int j = 0; j < R; j++) {
            if (map[i][j] != "_") {
                //cout<<"This is the highest level"<<highest_level<<endl;
                highest_level = i;
            }
        }
    }
    return highest_level;
}

bool array_is_empty(string** array, int L, int R) {
    for (int i = 0; i < L; i++) {
        for (int j = 0; j < R; j++) {
            if (array[i][j] != "_") {
                return false; // Array is not empty
            }
        }
    }
    return true; // Array is empty
}

void bomb_function(string** map, int L , int R, int i, int move_value){
    //i == L , move_value == R
    if (move_value - 1 >= 0)
        map[i][move_value - 1] = "_"; // left
    if (move_value + 1 < R)
        map[i][move_value + 1] = "_"; // right
    if (i - 1 >= 0)
        map[i - 1][move_value] = "_"; // top
    if (i + 1 < L)
        map[i + 1][move_value] = "_"; // bottom
    if (i - 1 >= 0 && move_value + 1 < R)
        map[i - 1][move_value + 1] = "_"; // top-right
    if (i - 1 >= 0 && move_value - 1 >= 0)
        map[i - 1][move_value - 1] = "_"; // top-left
    if (i + 1 < L && move_value + 1 < R)
        map[i + 1][move_value + 1] = "_"; // bottom-right
    if (i + 1 < L && move_value - 1 >= 0)
        map[i + 1][move_value - 1] = "_"; // bottom-left
}

bool checking_row_empty(string **map, int L , int R, int row){
    //cout<<"In here"<<endl;
    for (int j = 0; j < R; j++) {   
        //cout<<map[row][j]<<endl;
        if (map[row][j] != "_") {
            //cout<<"In here 2"<<endl;
            return false; // At least one item found in the row
        }
    }
    return true; // No item found in the row
}

int main(){
    //Initial inputs
    int R, L, N;
    cin >> R >> L >> N;

    // Declare a 2D array
    string** map = new string*[L];
    for (int i = 0; i < L; ++i) {
        map[i] = new string[R];
    }

    // Input loop
    for (int i = 0; i < L; i++){
        for (int j = 0; j < R; j++)
        {
            string userInput;
            //cout << << i << ", " << j ;
            cin >> userInput;
            //cout<< userInput<<endl;
            if (userInput == "F" || userInput == "M" || userInput == "B" ||
                userInput == "C" || userInput == "G" || userInput == "D" ||
                userInput == "P" ) {
                //cout<<userInput;
                map[i][j] = userInput;
            //debug whitespace
            } else if (userInput.empty() || userInput == " "){
                // If not, save underscore "_" to the array
                map[i][j] = '_';
            }else{
                map[i][j] = '_';
            }
        }
    }
    // Test
    /*
    for (int i = 0; i < L; i++){
        for (int j = 0; j < R; j++){
            cout << map[i][j] << " ";
        }
        cout << endl;
    }
    */    
    Queue moves_queue;
    STACK money_bag;
    Queue player_use;
    
    while(N>0){
        //char new_map[R][L];
        string move;
        cin>>move;
        if(move == "USE"){
            moves_queue.enqueue(move);
        }else{
            string number;
            cin>>number;
            moves_queue.enqueue(number);
            
        }
        N = N -1;
    }
    //cout<<"After getting all the inputs from user the queue is:"<<endl;
    //moves_queue.printQueue();
    //cout<<endl;
    //moves_queue.printQueue();
    /*Testing
    moves_queue.printQueue();
    moves_queue.dequeue();
    moves_queue.dequeue();
    moves_queue.dequeue();
    moves_queue.printQueue();
    
    money_bag.push("GOLD");
    money_bag.push("GOLD");
    money_bag.push("GOLD");
    money_bag.push("DIAMOND");
    money_bag.printStack();
    money_bag.pop();
    money_bag.pop();
    money_bag.pop();
    money_bag.printStack();
    cout << moves_queue.dequeue()<<endl;
    */
    //cout<< "Before the 2nd while loop"<<endl;
    //cout<<"The empty function is returning right now"<<endl;
    //cout<<moves_queue.queue_empty()<<endl;
    //moves_queue.printQueue();
    //cout<<endl;
    while (!moves_queue.queue_empty()) {

        //moves_queue.printQueue();
        string command = moves_queue.dequeue();
        //cout<<"The command is: "<<endl;
        //cout<<command<<endl;
        //cout<<command<<endl;
        //moves_queue.printQueue();
        //cout<<endl;
        //cout<< "WTF" <<endl;
        /*
        for (int i = 0; i < L; i++){
            for (int j = 0; j < R; j++){
                cout << map[i][j] << " ";
            }
            cout << endl;
        }
        cout<<endl;
        */
        //cout<< "WTF 1" <<endl;
        //moves_queue.printQueue();
        //cout<<endl;
        //moves_queue.printQueue();
        
        if (command != "USE") {
            //cout<<"WTF 2"<<endl;
            //cout<<command<<endl;
            int move_value = stoi(command);
            //cout << "WTF 3"<<endl;
            //cout << move_value << endl;
            //moves_queue.printQueue();
            if(move_value < R ){
                //cout<<"Going in"<<endl;
                for (int i = 0; i < L; i++) {
                    //cout<<map[i][move_value]<<endl;
                    if (map[i][move_value] != "_") {
                        //money_bag.printStack();
                        //cout<<map[i][move_value]<< "The i index at:"<<i << "The move value index at: "<< move_value <<endl;
                        //money_bag.printStack();
                        if(map[i][move_value] == "F" || map[i][move_value] == "M"){
                            //cout<<map[i][move_value]<<endl;
                            //cout<< "The second M"<<endl;
                            player_use.enqueue(map[i][move_value]);
                            map[i][move_value] = "_";
                        }else if(map[i][move_value] == "B"){
                            bomb_function(map,L,R,i,move_value);
                            map[i][move_value] = "_";
                            //cout<<"Bomb"<<endl;
                        }else if(map[i][move_value] == "C"){
                            map[i][move_value] = "_";
                            //cout<<"Lucky clover"<<endl; 
                            //[i][move_value] = "_";
                            /*
                            for (int i = 0; i < L; i++){
                                for (int j = 0; j < R; j++){
                                    cout << map[i][j] << " ";
                                }
                                cout << endl;
                            }
                            */
                            // check there atleast exists an item that row. 
                            bool row_has_item = !checking_row_empty(map,L,R,i);
                            //cout<<row_has_item<<endl;
                            //cout<<map[i][move_value]<<endl;
                            //cout<< i << move_value<<endl;

                            map[i][move_value] = "_";
                            if(!money_bag.stack_empty() && row_has_item){
                                //map[i][move_value] = '_';
                                //cout<<"IN LUCKY"<<endl;
                                //money_bag.printStack();
                                // Calculate the dimensions for the new map
                                int new_map_rows = 0;
                                if(i < 3){
                                    new_map_rows = L + 3 -i;
                                }else if (i>2){
                                    new_map_rows = L;
                                }
                                //int new_map_rows = L + 3 - i;
                                //cout<< "New map rows="<<new_map_rows<<endl;
                                int new_map_cols = R;
                                //cout<< new_map_cols << "The new map L"<<new_map_rows<<endl;
                                //cout<< "The old R,L,i value" << R<<L<<i<<endl;
                                // Dummy 2D array in case of C 
                                // Create a new map with valid size full of "_"
                                //cout<<"New map rows, cols"<<new_map_rows<<new_map_cols<<endl;
                                //cout<<i<<L<<endl;
                                
                                string **new_map = new string *[new_map_rows];
                                for (int x = 0; x < new_map_rows; x++) {
                                    new_map[x] = new string[new_map_cols];
                                    for (int y = 0; y < new_map_cols; y++) {
                                        new_map[x][y] = "_"; // Fill with "_"
                                    }
                                }
                                
                                //cout<<new_map_rows<<new_map_cols<<endl;
                                /*
                                for (int x = 0; x < new_map_rows; x++){
                                    for (int y = 0; y < new_map_cols; y++){
                                        //cout<< x <<y<<endl;
                                        cout << new_map[x][y] << " ";
                                    }
                                    cout << endl;
                                }
                                */
                                // Fill in the old values from the original map
                                if(i < 3){
                                    for (int x = 0; x < L; x++) {
                                        for (int y = 0; y < R; y++) {
                                            new_map[x + 3 - i][y] = map[x][y];
                                        }
                                }    
                                }else if (i>2){
                                    for (int x = 0; x < L; x++) {
                                        for (int y = 0; y < R; y++) {
                                            new_map[x][y] = map[x][y];
                                    }
                                }    
                                }
                                /*
                                for (int x = 0; x < new_map_rows; x++){
                                    for (int y = 0; y < new_map_cols; y++){
                                        cout << new_map[x][y] << " ";
                                    }
                                    cout << endl;
                                }
                                */
                                //cout<<new_map_rows-L<< i<<endl;
                                //t+1, t+2, t+3
                                
                                // Calculate the indices for columns x-2 to x+2 in the new map
                                int col_start = max(0, move_value - 2);  // Ensure we don't go below column index 0
                                int col_end = min(new_map_cols - 1, move_value + 2);  // Ensure we don't go beyond the last column index
                                
                                // Fill in the loot items at the correct positions
                                if(i < 3){
                                    for(int k = 0;k<3;k++){
                                        new_map[k][move_value] = money_bag.top();
                                        for (int col = col_start; col <= col_end; col++) {
                                            new_map[k][col] = money_bag.top();
                                        }
                                    }
                                }else if (i>2){
                                    new_map[i-1][move_value] = money_bag.top();
                                    new_map[i-2][move_value] = money_bag.top();
                                    new_map[i-3][move_value] = money_bag.top();

                                    for (int col = col_start; col <= col_end; col++) {
                                            new_map[i-1][col] = money_bag.top();
                                            new_map[i-2][col] = money_bag.top();
                                            new_map[i-3][col] = money_bag.top();
                                        }
                                }
                                //money_bag.printStack();
                                //cout<<money_bag.top()<<endl;
                                //[i-2][move_value] = money_bag.top();
                                //new_map[i-3][move_value] = money_bag.top();
                                // Replace the old map with the new map
                                for (int x = 0; x < L; x++) {
                                    delete[] map[x];
                                }
                                delete[] map;
                                // Update R,L,i
                                R = new_map_cols;
                                L = new_map_rows;
                                i = 0;
                                
                                //cout<< "New R,L, i values"<<R<<L<<i<<endl;
                                // creating map again with correct dimensions
                                map = new string*[L];
                                for (int x = 0; x < L; x++) {
                                    map[x] = new string[R];
                                }
                                
                                //cout<<"HEre is the problem"<<endl;
                                //updating the new map to the map
                                for (int x = 0; x < L; x++) {
                                    for (int y = 0; y < R; y++) {
                                        map[x][y] = new_map[x][y];
                                    }
                                }
                                
                                for (int x = 0; x < L; x++) {
                                    delete[] new_map[x];
                                }
                                delete[] new_map;
                            }
                        }else if(map[i][move_value] == "P"){
                            //cout<<"In pig"<<endl;
                            map[i][move_value] = "_";
                            //cout<<"The i is:  "<<i<<" "<< "The move_value is:  "<<move_value<<endl;
                            while (!money_bag.stack_empty() && money_bag.top() != "D") {
                            // Pop the stack
                            //cout<<"TOP VALUE value:  "<<money_bag.top()<<endl;
                                //money_bag.printStack();
                                money_bag.pop();
                                //money_bag.printStack();
                            }
                            //money_bag.printStack();
                            //map[i][move_value] = '_';
                            //cout<<"PIG"<<endl; //Pop until D
                        }else{// GOLD or DIAMOND
                            money_bag.push(map[i][move_value]);
                            map[i][move_value] = "_";
                        }
                        
                        break;  // Move to the next column after updating one position
                    } 
                    // if _ continue to iterate
                }
            }
        } else {//if the command is USE
            //player_use.printQueue();
            if(!player_use.queue_empty()){
                int highest_level = 0;
                if(player_use.dequeue() == "F"){
                    highest_level = highest_level_finder(map, L , R);
                    bool array_is_empty_bool = array_is_empty(map,L,R);
                    //cout<<array_is_empty_bool<<endl;
                    if(array_is_empty_bool){
                        cout << "MINE LEVEL:1" << endl;
                        for (int j = 0; j < R; j++) {
                            //cout<< "The highest level index is:  " << highest_level<<"The j value   " <<j<<endl;
                            //cout<<"The map value:   ";
                            cout <<"_"<<" ";
                        }
                        cout<<endl;
                    }else{
                        //player_use.dequeue();
                        highest_level = highest_level_finder(map, L , R);
                        //cout<<"The highest level is: "<<highest_level<<endl;
                        cout << "MINE LEVEL:" << L - highest_level << endl;

                        for (int j = 0; j < R; j++) {
                            //cout<< "The highest level index is:  " << highest_level<<"The j value   " <<j<<endl;
                            //cout<<"The map value:   ";
                            cout << map[highest_level][j] << " ";
                        }
                        cout << endl;
                    }
                }else{ //magnet
                    //cout<< "Going into magnet"<<endl;
                    //player_use.dequeue();
                    //moves_queue.printQueue();
                    for(int j = R-1 ; j>=0;j--){
                        //cout<< "The j starting value"<<to_string(j)<<endl;
                        moves_queue.addNewData(to_string(j));
                        //moves_queue.printQueue();
                    }
                    //moves_queue.printQueue();
                    //moves_queue.printQueue();
                    //highest_level = highest_level_finder(map, L , R);
                }
            }
            //Otherwise player Use is empty
        }
    }


    cout<<"FINAL BAG:"<<endl;
    if(!money_bag.stack_empty()){
        //cout<<"FINAL BAG:"<<endl;
        money_bag.printStack();
    }else{
        cout<<endl;
    }
    
    cout << "FINAL MAP:" << endl;
    int lowest_level_with_item = L;

    for (int i = 0; i < L; i++) {
        if (!checking_row_empty(map, L, R, i)) {
            lowest_level_with_item = i;
            break;
        }
    }
    for (int i = lowest_level_with_item; i < L; i++) {
        for (int j = 0; j < R; j++) {
            cout << map[i][j] << " ";
        }
        cout << endl;
    }
    //cout<<endl;

    //empty the space
    for (int i = 0; i < L; ++i) {
        delete[] map[i];
        //delete[] new_map[i]
    }
    delete[] map;
    //delete[] new_map;
    
    return 0;
}