// Mert Kilicaslan - THE1 Minesweeper Game
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream> 
using namespace std;

void vPrint(const vector<vector<char> > & v){
	for(unsigned int i = 0; i < v.size(); i++){
		for(unsigned int j = 0; j < v[0].size(); j++)
			cout << setw(4) << v[i][j];
		cout << endl;
	}
	cout << endl << endl;
}

bool isBomb(const vector<vector<int> > & bombVecIdx, const int & row, const int & col){
	for(unsigned int i = 0; i < bombVecIdx.size(); i++)
		if(bombVecIdx[i][0] == row && bombVecIdx[i][1] == col)
			return true;
	return false;
}

char neighbour(const vector<vector<int> > & bombVecIdx, const int & row, const int & col, const int & maxRow, const int & maxCol){
	int neighCount = 0;

	/* Right up, Right middle, Right down */
	if(col+1 <= maxCol && row-1 >= 0)
		if(isBomb(bombVecIdx, row-1, col+1))
			neighCount++;
	if(col+1 <= maxCol)
		if(isBomb(bombVecIdx, row, col+1))
			neighCount++;
	if(col+1 <= maxCol && row+1 <= maxRow)
		if(isBomb(bombVecIdx, row+1, col+1))
			neighCount++;

	/* Up, Down */
	if(row-1 >= 0)
		if(isBomb(bombVecIdx, row-1, col))
			neighCount++;
	if(row+1 <= maxRow)
		if(isBomb(bombVecIdx, row+1, col))
			neighCount++;

	/* Left up, Left middle, Left down */
	if(col-1 >= 0 && row-1 >= 0)
		if(isBomb(bombVecIdx, row-1, col-1))
			neighCount++;
	if(col-1 >= 0)
		if(isBomb(bombVecIdx, row, col-1))
			neighCount++;
	if(col-1 >= 0 && row+1 <= maxRow)
		if(isBomb(bombVecIdx, row+1, col-1))
			neighCount++;
	
	return neighCount + 48;

}

void zeroCellFound(vector<vector<char> > & gameVec, const vector<vector<int> > & bombVecIdx, const int & row, const int & col, const int & maxRow, const int & maxCol){
	/* Its own cell */
	gameVec[row][col] = '0';

	/* Right up, Right middle, Right down */
	if(col+1 <= maxCol && row-1 >= 0)
		gameVec[row-1][col+1] = neighbour(bombVecIdx,row-1,col+1,maxRow,maxCol);
	if(col+1 <= maxCol)
		gameVec[row][col+1] = neighbour(bombVecIdx,row,col+1,maxRow,maxCol);
	if(col+1 <= maxCol && row+1 <= maxRow)
		gameVec[row+1][col+1] = neighbour(bombVecIdx,row+1,col+1,maxRow,maxCol);

	/* Up, Down */
	if(row-1 >= 0)
		gameVec[row-1][col] = neighbour(bombVecIdx,row-1,col,maxRow,maxCol);
	if(row+1 <= maxRow)
		gameVec[row+1][col] = neighbour(bombVecIdx,row+1,col,maxRow,maxCol);

	/* Left up, Left middle, Left down */
	if(col-1 >= 0 && row-1 >= 0)
		gameVec[row-1][col-1] = neighbour(bombVecIdx,row-1,col-1,maxRow,maxCol);
	if(col-1 >= 0)
		gameVec[row][col-1] = neighbour(bombVecIdx,row,col-1,maxRow,maxCol);
	if(col-1 >= 0 && row+1 <= maxRow)
		gameVec[row+1][col-1] = neighbour(bombVecIdx,row+1,col-1,maxRow,maxCol);
} 

bool canHelp_check_1_2(const vector<vector<char> > & gameVec){
	for(unsigned int i = 0; i < gameVec.size(); i++)
		for(unsigned int j = 0; j < gameVec[0].size(); j++)
			if(gameVec[i][j] != '.' && gameVec[i][j] != '0' && gameVec[i][j] != 'B')
				return true;

	cout << "I can't help you." << endl;
	return false;
}

bool openHelp_check_3(vector<vector<char> > & gameVec, const vector<vector<int> > & bombVecIdx, const int & maxRow, const int & maxCol){
	
	unsigned int i = 0;
	while(i < bombVecIdx.size()){
		int row = bombVecIdx[i][0];
		int col = bombVecIdx[i][1];
		i++;

		if(gameVec[row][col] != 'B' && gameVec[row][col] != 'x'){

			/* Right up, Right middle, Right down */
			if(col+1 <= maxCol && row-1 >= 0)
				if(gameVec[row-1][col+1] != '.' && gameVec[row-1][col+1] != '0'){
					gameVec[row][col] = 'x';
					break;
				}
			if(col+1 <= maxCol)
				if(gameVec[row][col+1] != '.' && gameVec[row][col+1] != '0'){
					gameVec[row][col] = 'x';
					break;
				}
			if(col+1 <= maxCol && row+1 <= maxRow)
				if(gameVec[row+1][col+1] != '.' && gameVec[row+1][col+1] != '0'){
					gameVec[row][col] = 'x';
					break;
				}

			/* Up, Down */
			if(row-1 >= 0)
				if(gameVec[row-1][col] != '.' && gameVec[row-1][col] != '0'){
					gameVec[row][col] = 'x';
					break;
				}
			if(row+1 <= maxRow)
				if(gameVec[row+1][col] != '.' && gameVec[row+1][col] != '0'){
					gameVec[row][col] = 'x';
					break;
				}

			/* Left up, Left middle, Left down */
			if(col-1 >= 0 && row-1 >= 0)
				if(gameVec[row-1][col-1] != '.' && gameVec[row-1][col-1] != '0'){
					gameVec[row][col] = 'x';
					break;
				}
			if(col-1 >= 0)
				if(gameVec[row][col-1] != '.' && gameVec[row][col-1] != '0'){
					gameVec[row][col] = 'x';
					break;
				}
			if(col-1 >= 0 && row+1 <= maxRow)
				if(gameVec[row+1][col-1] != '.' && gameVec[row+1][col-1] != '0'){
					gameVec[row][col] = 'x';
					break;
				}
		}

		if(i == gameVec.size()){
			cout << "I can't help you."  << endl;
			return false;
		}
	}
	return true;
}

bool isFinish(const vector<vector<char> > & gameVec){
	for(unsigned int i = 0; i < gameVec.size(); i++)
		for(unsigned int j = 0; j < gameVec[0].size(); j++)
			if(gameVec[i][j] == '.')
				return false;
	return true;

}

bool doesWin(const vector<vector<char> > & gameVec, const vector<vector<int> > & bombVecIdx){
	/* Case1- checking positions of the bombs */
	for(unsigned int i = 0; i < bombVecIdx.size(); i++){
		int row = bombVecIdx[i][0];
		int col = bombVecIdx[i][1];

		if(gameVec[row][col] != 'B' && gameVec[row][col] != 'x')
			return false;
	}

	/* Case2- checking counts of the bombs */
	int counter = 0;
	for(unsigned int i = 0; i < gameVec.size(); i++)
		for(unsigned j = 0; j < gameVec[0].size(); j++)
			if(gameVec[i][j] == 'B' || gameVec[i][j] == 'x')
				counter++;

	if(counter != bombVecIdx.size())
		return false;

	return true;
}

int main(){

	string fileName;
	ifstream input;
	int col, row;

	do{
		cout << "Enter the input file name: ";
		cin >> fileName;
		input.open(fileName.c_str());

		if(input.fail())
			cout << "Problem occurred while reading the file!!!" << endl;
		else{
			cout << "Welcome to the Minesweeper Game!" << endl;
			cout << "You may choose a cell to open (-o), get help (-h) or mark/unmark bomb (-b)!!" << endl;
		}

	} while(input.fail());


	/* Vector initialization */
	input >> row >> col;
	vector<vector<char> > gameVec(row,vector<char>(col,'.'));
	vector<vector<int> > bombVecIdx;

	/* Getting bomb indexes from the file */
	char ch;
	int rowCount = 0, colCount = 0;

	while(input.get(ch)){
		if(ch == 'x'){
			vector<int> bombPair;
			bombPair.push_back(rowCount);
			bombPair.push_back(colCount);
			bombVecIdx.push_back(bombPair);
	}

		if(ch == '-' || ch == 'x'){
			colCount++;
			if(colCount == col){
				colCount = 0;
				rowCount++;
			}
		}
	}

	input.close();


	/* MINESWEEPER GAME */
	string choice;
	int helpCount = 0;
	const int rowMaxIdx = row-1, colMaxIdx = col-1;
	bool gameOver = false;

	vPrint(gameVec);
	while(!isFinish(gameVec)){
		cout << "Please enter your choice: ";
		cin >> choice;

		/* Open cell */
		if(choice == "-o"){
			int r,c;
			cin >> r >> c;

			if(r > rowMaxIdx || 0 > r || c > colMaxIdx || 0 > c){
				cout << "Please enter a valid coordinate! " << endl;
				continue;
			}
			else if(gameVec[r][c] == 'B' || gameVec[r][c] == 'x'){
				cout << "It seems like this cell is a bomb." << endl;
				continue;
			}
			else if(gameVec[r][c] != '.'){
				cout << "Already opened." << endl;
				continue;
			}
			else{
				if(isBomb(bombVecIdx,r,c)){
					cout << "You opened a mine! Game over:(" << endl;
					gameOver = true;
					break;
				}
				else if(neighbour(bombVecIdx,r,c,rowMaxIdx,colMaxIdx) >= '1') 
					gameVec[r][c] = neighbour(bombVecIdx,r,c,rowMaxIdx,colMaxIdx);
				else
					zeroCellFound(gameVec,bombVecIdx,r,c,rowMaxIdx,colMaxIdx);
			}
		}
	
		/* Mark/unmark cell */
		else if(choice == "-b"){
			int r,c;
			cin >> r >> c;

			if(r > rowMaxIdx || 0 > r || c > colMaxIdx || 0 > c){
				cout << "Please enter a valid coordinate! " << endl;
				continue;
			}
			else if(gameVec[r][c] == '.')
				gameVec[r][c] = 'B';

			else if(gameVec[r][c] == 'B')
				gameVec[r][c] = '.';

			else if(gameVec[r][c] != '.'){
				cout << "Can't mark that cell as a bomb." << endl;
				continue;
			}
		}

		/* Get help */
		else if(choice == "-h"){
			helpCount++;

			if(helpCount > 3){
				cout << "Your help chances are over. :(" << endl;
				continue;
			}
			else if(canHelp_check_1_2(gameVec)){
				if(!openHelp_check_3(gameVec,bombVecIdx,rowMaxIdx,colMaxIdx))
					continue;
			}
		}

		/* Invalid input */
		else
			cout << "Invalid choice!!" << endl;

		vPrint(gameVec);
	}

	/* Checking if the player wins the game or not */
	if(!gameOver){
		if(doesWin(gameVec,bombVecIdx))
			cout << "Congrats! You won!" << endl;
		else
			cout << "You put bombs in the wrong places! Game over:(" << endl;
	}

	return 0;
}
