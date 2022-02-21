// Mert Kilicaslan - THE4 Childhood’s End
#include<sstream>
#include<thread>
#include<mutex>
#include<fstream>
#include<iomanip>
#include<string>
#include<iostream>
using namespace std;

/* Deletes dynamically allocated matrix and gives memory back to the heap */
void MatrixMutexDelete(mutex** matrix, const int & sizeRow){
	for(int i = 0; i < sizeRow; i++)
		delete[] matrix[i];
	delete matrix;
}

/* Displays the matrix content */
void displayMatrix(int** matrix, const int & sizeRow, const int & sizeCol){
	for(int i = 0; i < sizeRow; i++){
		for(int j = 0; j < sizeCol; j++)
			cout << setw(5) << matrix[i][j];
		cout << endl;
	}
}

/* Deletes dynamically allocated matrix and gives memory back to the heap */
void MatrixThreadDelete(thread** matrix, const int & sizeRow){
	for(int i = 0; i < sizeRow; i++)
		delete[] matrix[i];
	delete matrix;
}

/* Deletes dynamically allocated matrix and gives memory back to the heap */
void MatrixDelete(int** matrix, const int & sizeRow){
	for(int i = 0; i < sizeRow; i++)
		delete[] matrix[i];
	delete matrix;
}

/* Global values for 'TheFunction' */
bool isOkay = true;
mutex** DynamicMatrixMutex, mutexCout;

/* Heart of the program, it takes the i and j indexes and the row, col values for processing */
void TheFunction(const int & i, const int & j, int **& matrix, const int & row, const int & col){

	/* bool values: that helps the find wheter the result is true or false after trylock checking */
	bool boolA = false, boolB = false, boolC = false;
	bool boolD = false, boolE = false;			

	double baseCell = 0.0;							/* baseCell: the cell that has many neigbor the value of it is double because it should be divisible */
	int theMaximum = 0;						     	/* theMaximum: finds the highest number among their neigbours */
	int ValA = 0,ValB = 0,ValC = 0,ValD= 0;			/* int values: gets the neighbor values to the current cell */
	bool boolFlag = false;							/* boolFlag: helper bool value for conditions that are proccesed */ 

	while(!false){
	 /* Left above point */
	 if(j == 0 && i == 0){

		/* Finds the nearest neighbour cells unlocks them if they are available and not locked */ 
		boolA = DynamicMatrixMutex[0][1].try_lock();
		boolC = DynamicMatrixMutex[0][0].try_lock();
		boolB = DynamicMatrixMutex[1][0].try_lock();

		/* If all neighbor cells are unlocked then boolFlags = true */
		if(boolA == true && boolB == true && boolC == true) 
			boolFlag = true;

		if(boolFlag){
			baseCell = matrix[0][0];
			ValB = matrix[1][0];
			ValA = matrix[0][1];

			/* Finding the highest number in the neigbour cells, to get enough time for printing, it locks cout then base cell gets number from their nearest cells since its number and all nearest cell are unlocked */
			if(ValA >= ValB){	
				theMaximum = ValA;

				if(theMaximum >= 2*baseCell){
					isOkay = true;
					mutexCout.lock();
					cout << "Row-" << i << ",Col-" << j <<" (" << matrix[i][j] << ") is incremented by " << ceil(baseCell/3) << " by stealing from the cell to the right (" << matrix[0][1] << ")." << endl;
					mutexCout.unlock();
					matrix[0][0] += ceil(baseCell/3);
					matrix[0][1] -= ceil(baseCell/3);
				}

				DynamicMatrixMutex[1][0].unlock();
				DynamicMatrixMutex[0][1].unlock();
				DynamicMatrixMutex[0][0].unlock();
			}
			
			/* Finding the highest number in the neigbour cells, to get enough time for printing, it locks cout then base cell gets number from their nearest cells since its number and all nearest cell are unlocked */
				else{
					theMaximum = ValB;
					if(theMaximum >= 2*baseCell){

					isOkay = true;
					mutexCout.lock();
					cout << "Row-" << i << ",Col-" << j << " (" << matrix[i][j] << ") is incremented by " << ceil(baseCell/3) << " by stealing from the cell below (" << matrix[1][0] << ")." << endl;
					mutexCout.unlock();
					matrix[0][0] += ceil(baseCell/3);
					matrix[1][0] -= ceil(baseCell/3);
					}

					DynamicMatrixMutex[1][0].unlock();
					DynamicMatrixMutex[0][1].unlock();
					DynamicMatrixMutex[0][0].unlock();
				}
				break;
		}

		/* If there are any locked neigbour cells but others unlocked, it should be unlocked since at least one of them is locked */
		else if(boolFlag == false){
			if(boolA == true)
				DynamicMatrixMutex[0][1].unlock();
		
			if(boolB == true)
				DynamicMatrixMutex[1][0].unlock();
			
			if(boolC == true)
				DynamicMatrixMutex[0][0].unlock();

			this_thread::yield();
		}


	}
	/* Above edge processing edge without corner values */
	else if(j != 0 && i == 0 && j != col-1){

		/* Finds the nearest neighbour cells unlocks them if they are available and not locked */ 
		boolA = DynamicMatrixMutex[i][j-1].try_lock();
		boolB = DynamicMatrixMutex[i+1][j].try_lock();
		boolC = DynamicMatrixMutex[i][j+1].try_lock();
		boolD = DynamicMatrixMutex[i][j].try_lock();

		if(boolC==true && boolD == true && boolA == true && boolB== true)
			boolFlag = true;
	
		if(boolFlag){
			baseCell = matrix[i][j];
			ValA = matrix[i][j-1];
			ValC = matrix[i+1][j];
			ValB = matrix[i][j+1];

			if(ValA >= ValC && ValA >= ValB){
				theMaximum = ValA;

				if(theMaximum >= 2*baseCell){
					mutexCout.lock();
					cout << "Row-" << i << ",Col-" << j << " (" << matrix[i][j] << ") is incremented by " << ceil(baseCell/3) << " by stealing from the cell to the left (" << matrix[i][j-1] << ")." << endl;
					mutexCout.unlock();
					matrix[i][j] += ceil(baseCell/3);
					matrix[i][j-1] -= ceil(baseCell/3);
					isOkay = true;
				}

				DynamicMatrixMutex[i][j-1].unlock();
				DynamicMatrixMutex[i][j+1].unlock();
				DynamicMatrixMutex[i][j].unlock();
				DynamicMatrixMutex[i+1][j].unlock();
			}

			else if(ValB >= ValC && ValB >= ValA){
				theMaximum = ValB;

				/* Finding the highest number in the neigbour cells, to get enough time for printing, it locks cout then base cell gets number from their nearest cells since its number and all nearest cell are unlocked */
				if(theMaximum >= 2*baseCell){
					mutexCout.lock();
					cout << "Row-" << i << ",Col-" << j << " (" << matrix[i][j] << ") is incremented by " << ceil(baseCell/3) << " by stealing from the cell to the right (" << matrix[i][j+1] << ")." << endl;
					mutexCout.unlock();
					matrix[i][j] += ceil(baseCell/3);
					matrix[i][j+1] -= ceil(baseCell/3);
					isOkay = true;
				}

				DynamicMatrixMutex[i+1][j].unlock();
				DynamicMatrixMutex[i][j+1].unlock();
				DynamicMatrixMutex[i][j].unlock();
				DynamicMatrixMutex[i][j-1].unlock();
			}

			else if(ValC >= ValB && ValC >= ValA){
				theMaximum = ValC;

				/* Finding the highest number in the neigbour cells, to get enough time for printing, it locks cout then base cell gets number from their nearest cells since its number and all nearest cell are unlocked */
				if(theMaximum >= 2*baseCell){ 
					mutexCout.lock();
					cout << "Row-" << i << ",Col-" << j << " ("<<matrix[i][j] << ") is incremented by " << ceil(baseCell/3) << " by stealing from the cell below (" << matrix[i+1][j] << ")." << endl;
					mutexCout.unlock();
					matrix[i][j] += ceil(baseCell/3);
					matrix[i+1][j] -= ceil(baseCell/3);
					isOkay = true;
				}

				DynamicMatrixMutex[i][j+1].unlock();
				DynamicMatrixMutex[i][j-1].unlock();
				DynamicMatrixMutex[i+1][j].unlock();
				DynamicMatrixMutex[i][j].unlock();
			}
			break;
		}

		/* If there are any locked neigbour cells but others unlocked, it should be unlocked since at least one of them is locked */
		else if(boolFlag != true){
			if(boolC == true)
				DynamicMatrixMutex[i][j+1].unlock();

			if(boolA == true)
				DynamicMatrixMutex[i][j-1].unlock();
	
			if(boolB == true)			
				DynamicMatrixMutex[i+1][j].unlock();
			
			if(boolD == true)			
				DynamicMatrixMutex[i][j].unlock();
			
			this_thread::yield();
		}


	}

	/* Processesing of right upper */
	else if(j == col-1 && i == 0){

		/* Finds the nearest neighbour cells unlocks them if they are available and not locked */ 
		boolA = DynamicMatrixMutex[i+1][j].try_lock();
		boolC = DynamicMatrixMutex[i][j].try_lock();
		boolB = DynamicMatrixMutex[i][j-1].try_lock();

		if(boolB == true && boolC == true && boolA == true)
			boolFlag = true;
		
		if(boolFlag){
			baseCell = matrix[i][j];
			ValA = matrix[i+1][j];
			ValB = matrix[i][j-1];

			if(ValA >= ValB){
			/* Finding the highest number in the neigbour cells, to get enough time for printing, it locks cout then base cell gets number from their nearest cells since its number and all nearest cell are unlocked */
				theMaximum = ValA;

				if(theMaximum >= 2*baseCell){
					mutexCout.lock();
					cout << "Row-" << i << ",Col-" << j << " ("<<matrix[i][j] << ") is incremented by " << ceil(baseCell/3) << " by stealing from the cell below (" << matrix[i+1][j] << ")." << endl;
					mutexCout.unlock();
					matrix[i][j] += ceil(baseCell/3);
					matrix[i+1][j] -= ceil(baseCell/3);
					isOkay = true;					
				}

				DynamicMatrixMutex[i+1][j].unlock();
				DynamicMatrixMutex[i][j-1].unlock();	
				DynamicMatrixMutex[i][j].unlock();
			}

			else{
				theMaximum = ValB;

				/* The baseCell must smaller than theMaximum value */
				if(theMaximum >= 2*baseCell){

					mutexCout.lock();
					cout << "Row-" << i << ",Col-" << j << " (" << matrix[i][j] << ") is incremented by " << ceil(baseCell/3) << " by stealing from the cell to the left (" << matrix[i][j-1] << ")." << endl;
					mutexCout.unlock();
					matrix[i][j] += ceil(baseCell/3);
					matrix[i][j-1] -= ceil(baseCell/3);			
					isOkay = true;
				}

				DynamicMatrixMutex[i+1][j].unlock();
				DynamicMatrixMutex[i][j-1].unlock();
				DynamicMatrixMutex[i][j].unlock();
			}
			break;
		}

		/* If there are any locked neigbour cells but others unlocked, it should be unlocked since at least one of them is locked */
		else if(boolFlag != true){
			if(boolA == true)
				DynamicMatrixMutex[i+1][j].unlock();
			
			if(boolC == true)
				DynamicMatrixMutex[i][j].unlock();

			if(boolB == true)
				DynamicMatrixMutex[i][j-1].unlock();
			
			this_thread::yield();

		}
	}

	/* Processing of the left bottom corner */
	else if(j == 0 && i == row-1){
		/* Finds the nearest neighbour cells unlocks them if they are available and not locked */ 

		boolA = DynamicMatrixMutex[i-1][j].try_lock();
		boolC = DynamicMatrixMutex[i][j].try_lock();
		boolB = DynamicMatrixMutex[i][j+1].try_lock();

		if(boolB == true && boolA == true && boolC == true)
			boolFlag = true;
	
		if(boolFlag){

			ValA = matrix[i-1][j];
			baseCell = matrix[i][j];
			ValB = matrix[i][j+1];

			/* Finding the highest number in the neigbour cells, to get enough time for printing, it locks cout then base cell gets number from their nearest cells since its number and all nearest cell are unlocked */
			if(ValA >= ValB){
				theMaximum = ValA;

				if(theMaximum >= 2*baseCell){
					mutexCout.lock();
					cout << "Row-" << i << ",Col-" << j << " (" << matrix[i][j] << ") is incremented by " << ceil(baseCell/3) << " by stealing from the cell above (" << matrix[i-1][j] << ")." << endl;
					mutexCout.unlock();
					matrix[i][j] += ceil(baseCell/3);
					matrix[i-1][j] -= ceil(baseCell/3);
					isOkay = true;
				}	

				DynamicMatrixMutex[i][j+1].unlock();
				DynamicMatrixMutex[i-1][j].unlock();
				DynamicMatrixMutex[i][j].unlock();
			}

			else{
				theMaximum = ValB;

				/* Finding the highest number in the neigbour cells, to get enough time for printing, it locks cout then base cell gets number from their nearest cells since its number and all nearest cell are unlocked */
				if(theMaximum >= 2*baseCell){
					mutexCout.lock();
					cout << "Row-" << i << ",Col-" << j << " (" << matrix[i][j] << ") is incremented by " << ceil(baseCell/3) << " by stealing from the cell to the right (" << matrix[i][j+1] << ")." << endl;
					mutexCout.unlock();
					matrix[i][j] += ceil(baseCell/3);
					matrix[i][j+1] -= ceil(baseCell/3);
					isOkay = true;
				}	

				DynamicMatrixMutex[i][j+1].unlock();
				DynamicMatrixMutex[i][j].unlock();
				DynamicMatrixMutex[i-1][j].unlock();
			}
			break;
		}

		/* If there are locked neigbour cells but others unlocked, it should be unlocked since at least one of them is locked */
		else if(boolFlag != true){
			if(boolA == true)
				DynamicMatrixMutex[i-1][j].unlock();

			if(boolC == true)
				DynamicMatrixMutex[i][j].unlock();
		
			if(boolB == true)
				DynamicMatrixMutex[i][j+1].unlock();
			
			this_thread::yield();			
		}
	}


	/* Processing of the bottom */
	else if(i == row-1 && j != col-1 && j != 0){
		/* Finds the nearest neighbour cells unlocks them if they are available and not locked */ 
		boolB = DynamicMatrixMutex[i][j-1].try_lock();
		boolC = DynamicMatrixMutex[i][j+1].try_lock();
		boolA = DynamicMatrixMutex[i-1][j].try_lock();
		boolD = DynamicMatrixMutex[i][j].try_lock();

		/* If all neighbour values are unlocked than boolFlage --> true */
		if(boolC == true && boolA == true && boolB == true && boolD == true)
			boolFlag = true;
		
		if(boolFlag){

			baseCell = matrix[i][j];
			ValA = matrix[i-1][j];
			ValC = matrix[i][j+1];
			ValB = matrix[i][j-1];

			/* Finding the highest number in the neigbour cells, to get enough time for printing, it locks cout then base cell gets number from their nearest cells since its number and all nearest cell are unlocked */
			if(ValA >= ValC && ValA >= ValB){
				theMaximum = ValA;
				if(theMaximum >= 2*baseCell){
					mutexCout.lock();
					cout << "Row-" << i << ",Col-" << j << " (" << matrix[i][j] << ") is incremented by " << ceil(baseCell/3) << " by stealing from the cell above (" << matrix[i-1][j] << ")." << endl;
					mutexCout.unlock();
					matrix[i][j] += ceil(baseCell/3);
					matrix[i-1][j] -= ceil(baseCell/3);
					isOkay = true;
				}

				DynamicMatrixMutex[i-1][j].unlock();
				DynamicMatrixMutex[i][j-1].unlock();
				DynamicMatrixMutex[i][j+1].unlock();
				DynamicMatrixMutex[i][j].unlock();
			}


			/* Finding the highest number in the neigbour cells, to get enough time for printing, it locks cout then base cell gets number from their nearest cells since its number and all nearest cell are unlocked */
			else if(ValB >= ValC && ValB >= ValA){
				theMaximum = ValB;
				if(theMaximum >= 2*baseCell){
					mutexCout.lock();
					cout << "Row-" << i << ",Col-" << j << " (" << matrix[i][j] << ") is incremented by " << ceil(baseCell/3) << " by stealing from the cell to the left (" << matrix[i][j-1] << ")." << endl;
					mutexCout.unlock();
					matrix[i][j] += ceil(baseCell/3);
					matrix[i][j-1] -= ceil(baseCell/3);
					isOkay = true;
				}

				DynamicMatrixMutex[i-1][j].unlock();
				DynamicMatrixMutex[i][j-1].unlock();
				DynamicMatrixMutex[i][j+1].unlock();
				DynamicMatrixMutex[i][j].unlock();
			}


			/* Finding the highest number in the neigbour cells, to get enough time for printing, it locks cout then base cell gets number from their nearest cells since its number and all nearest cell are unlocked */
			else if(ValC >= ValB && ValC >= ValA){
				theMaximum = ValC;
				if(theMaximum >= 2*baseCell){
					mutexCout.lock();
					cout << "Row-" << i << ",Col-" << j << " (" << matrix[i][j] << ") is incremented by " << ceil(baseCell/3) << " by stealing from the cell to the right (" << matrix[i][j+1] << ")." << endl;
					mutexCout.unlock();
					matrix[i][j] += ceil(baseCell/3);
					matrix[i][j+1] -= ceil(baseCell/3);
					isOkay = true;				
				}

				DynamicMatrixMutex[i][j+1].unlock();
				DynamicMatrixMutex[i][j].unlock();
				DynamicMatrixMutex[i][j-1].unlock();
				DynamicMatrixMutex[i-1][j].unlock();
			}
			break;
		}

		/* If there are any locked neigbour cells but others unlocked, it should be unlocked since at least one of them is locked */
		else if(boolFlag != true){
			if(boolA == true)
				DynamicMatrixMutex[i-1][j].unlock();
	
			if(boolD == true)
				DynamicMatrixMutex[i][j].unlock();
			
			if(boolB == true)
				DynamicMatrixMutex[i][j-1].unlock();
	
			if(boolC == true)
				DynamicMatrixMutex[i][j+1].unlock();
			
			
			this_thread::yield();

		}
	}

	/* Processing of the rigtt bottom corner */
	else if(j == col-1 && i == row-1){

		boolA = DynamicMatrixMutex[i-1][j].try_lock();
		boolC = DynamicMatrixMutex[i][j].try_lock();
		boolB = DynamicMatrixMutex[i][j-1].try_lock();

		if(boolB == true && boolC == true && boolA == true)
			boolFlag = true;
		
		if(boolFlag){
			ValA = matrix[i-1][j];
			baseCell = matrix[i][j];
			ValB = matrix[i][j-1];			

			/* Finding the highest number in the neigbour cells, to get enough time for printing, it locks cout then base cell gets number from their nearest cells since its number and all nearest cell are unlocked */
			if(!(ValA < ValB)){
				theMaximum = ValA;

				if(theMaximum >= 2*baseCell){
					mutexCout.lock();
					cout << "Row-" << i << ",Col-" << j << " (" << matrix[i][j] << ") is incremented by " << ceil(baseCell/3) << " by stealing from the above (" << matrix[i-1][j] << ")." << endl;
					mutexCout.unlock();
					matrix[i][j] += ceil(baseCell/3);
					matrix[i-1][j] -= ceil(baseCell/3);
					isOkay = true;
				}	

				DynamicMatrixMutex[i][j-1].unlock();
				DynamicMatrixMutex[i-1][j].unlock();
				DynamicMatrixMutex[i][j].unlock();
			}

			/* Finding the highest number in the neigbour cells, to get enough time for printing, it locks cout then base cell gets number from their nearest cells since its number and all nearest cell are unlocked */
			else{
				theMaximum = ValB;
				if(theMaximum >= 2*baseCell){

					mutexCout.lock();
					cout << "Row-" << i << ",Col-" << j << " (" << matrix[i][j] << ") is incremented by " << ceil(baseCell/3) << " by stealing from the cell to the left (" << matrix[i][j-1] << ")." << endl;
					mutexCout.unlock();
					matrix[i][j] += ceil(baseCell/3);
					matrix[i][j-1] -= ceil(baseCell/3);
					isOkay = true;				
				}	

				DynamicMatrixMutex[i][j-1].unlock();
				DynamicMatrixMutex[i][j].unlock();
				DynamicMatrixMutex[i-1][j].unlock();
			}
			break;
		}


		/* If there are any locked neigbour cells but others unlocked, it should be unlocked since at least one of them is locked */
		else if(boolFlag != true){
			if(boolA == true)
				DynamicMatrixMutex[i-1][j].unlock();
			
			if(boolB == true)
				DynamicMatrixMutex[i][j-1].unlock();
			
			if(boolC == true)
				DynamicMatrixMutex[i][j].unlock();

			this_thread::yield();
		}
	}

	/* Processing of the left */
	else if(i != 0 && i != row-1 && j == 0){
		/* Finds the nearest neighbour cells unlocks them if they are available and not locked */ 
		boolA = DynamicMatrixMutex[i][j+1].try_lock();
		boolB = DynamicMatrixMutex[i+1][j].try_lock();
		boolC = DynamicMatrixMutex[i-1][j].try_lock();
		boolD = DynamicMatrixMutex[i][j].try_lock();

		/* If all neighbour values are unlocked than boolFlage --> true */
		if(boolA == true && boolB == true && boolC == true && boolD == true)
			boolFlag = true;
		
		if(boolFlag){
			baseCell = matrix[i][j];
			ValB = matrix[i+1][j];
			ValA = matrix[i][j+1];
			ValC = matrix[i-1][j];

			/* Finding the highest number in the neigbour cells, to get enough time for printing, it locks cout then base cell gets number from their nearest cells since its number and all nearest cell are unlocked */
			if(ValA >= ValC && ValA >= ValB){
				theMaximum = ValA;

				if(theMaximum >= 2*baseCell){
					mutexCout.lock();
					cout << "Row-" << i << ",Col-" << j << " (" << matrix[i][j] << ") is incremented by " << ceil(baseCell/3) << " by stealing from the cell to the right (" << matrix[i][j+1] << ")." << endl;
					mutexCout.unlock();
					matrix[i][j] += ceil(baseCell/3);
					matrix[i][j+1] -= ceil(baseCell/3);
					isOkay = true;
				}

				DynamicMatrixMutex[i][j+1].unlock();
				DynamicMatrixMutex[i-1][j].unlock();
				DynamicMatrixMutex[i][j].unlock();
				DynamicMatrixMutex[i+1][j].unlock();
			}

			/* Finding the highest number in the neigbour cells, to get enough time for printing, it locks cout then base cell gets number from their nearest cells since its number and all nearest cell are unlocked */
			else if(ValB >= ValC && ValB >= ValA){
				theMaximum = ValB;

				if(theMaximum >= 2*baseCell){
					mutexCout.lock();
					cout << "Row-" << i << ",Col-" << j << " (" << matrix[i][j] << ") is incremented by " << ceil(baseCell/3) << " by stealing from the cell below (" << matrix[i+1][j] << ")." << endl;
					mutexCout.unlock();
					matrix[i][j] += ceil(baseCell/3);
					matrix[i+1][j] -= ceil(baseCell/3);
					isOkay = true;			
				}

				DynamicMatrixMutex[i-1][j].unlock();
				DynamicMatrixMutex[i+1][j].unlock();
				DynamicMatrixMutex[i][j+1].unlock();
				DynamicMatrixMutex[i][j].unlock();
			}

			/* Finding the highest number in the neigbour cells, to get enough time for printing, it locks cout then base cell gets number from their nearest cells since its number and all nearest cell are unlocked */
			else if(ValC >= ValB && ValC >= ValA){
				theMaximum = ValC;

				if(theMaximum >= 2*baseCell){
					mutexCout.lock();
					cout << "Row-" << i << ",Col-" << j << " (" << matrix[i][j] << ") is incremented by " << ceil(baseCell/3) << " by stealing from the cell to the right (" << matrix[i-1][j] << ")." << endl;
					mutexCout.unlock();
					matrix[i][j] += ceil(baseCell/3);
					matrix[i-1][j] -= ceil(baseCell/3);
					isOkay = true;
				}

				DynamicMatrixMutex[i][j+1].unlock();
				DynamicMatrixMutex[i][j].unlock();
				DynamicMatrixMutex[i+1][j].unlock();
				DynamicMatrixMutex[i-1][j].unlock();
			}
			break;
		}

		/* If there are any llocked neigbour cells but others unlocked, it should be unlocked since at least one of them is locked */
		else if(boolFlag != true){
			if(boolA == true)
				DynamicMatrixMutex[i][j+1].unlock();
			
			if(boolB == true)		
				DynamicMatrixMutex[i+1][j].unlock();
			
			if(boolD == true)	
				DynamicMatrixMutex[i][j].unlock();

			if(boolC == true)			
				DynamicMatrixMutex[i-1][j].unlock();
			
			this_thread::yield();

		}
	}

	/* Processing of the right */
	else if(i != row-1 && i != 0 && j == col-1){

		/* Finds the nearest neighbour cells unlocks them if they are available and not locked */ 
		boolC = DynamicMatrixMutex[i][j-1].try_lock();
		boolD = DynamicMatrixMutex[i][j].try_lock();
		boolA = DynamicMatrixMutex[i-1][j].try_lock();
		boolB = DynamicMatrixMutex[i+1][j].try_lock();

		/* If all neighbour values are unlocked than boolFlage --> true */
		if(boolA == true && boolC == true && boolB == true && boolD == true)
			boolFlag=true;

		if(boolFlag){
			baseCell = matrix[i][j];
			ValB = matrix[i+1][j];
			ValC = matrix[i][j-1];
			ValA = matrix[i-1][j];


			/* Finding the highest number in the neigbour cells, to get enough time for printing, it locks cout then base cell gets number from their nearest cells since its number and all nearest cell are unlocked */
			if(ValA >= ValC && ValA >= ValB){
				theMaximum = ValA;

				if(theMaximum >= 2*baseCell){
					mutexCout.lock();
					cout << "Row-" << i << ",Col-" << j << " (" << matrix[i][j] << ") is incremented by " << ceil(baseCell/3) << " by stealing from the cell above (" << matrix[i-1][j] << ")." << endl;
					mutexCout.unlock();
					matrix[i][j] += ceil(baseCell/3);
					matrix[i-1][j] -= ceil(baseCell/3);
					isOkay = true;
				}

				DynamicMatrixMutex[i][j].unlock();
				DynamicMatrixMutex[i-1][j].unlock();
				DynamicMatrixMutex[i+1][j].unlock();
				DynamicMatrixMutex[i][j-1].unlock();
			}

			/* Finding the highest number in the neigbour cells, to get enough time for printing, it locks cout then base cell gets number from their nearest cells since its number and all nearest cell are unlocked */
			else if(ValB >= ValC && ValB >= ValA){
				theMaximum = ValB;

				if(theMaximum >= 2*baseCell){
					mutexCout.lock();
					cout << "Row-" << i << ",Col-" << j << " (" << matrix[i][j] << ") is incremented by " << ceil(baseCell/3) << " by stealing from the cell below (" << matrix[i+1][j] << ")." << endl;
					mutexCout.unlock();
					matrix[i][j] += ceil(baseCell/3);
					matrix[i+1][j] -= ceil(baseCell/3);
					isOkay = true;				
				}

				DynamicMatrixMutex[i+1][j].unlock();
				DynamicMatrixMutex[i-1][j].unlock();
				DynamicMatrixMutex[i][j-1].unlock();
				DynamicMatrixMutex[i][j].unlock();
			}

			/* Finding the highest number in the neigbour cells, to get enough time for printing, it locks cout then base cell gets number from their nearest cells since its number and all nearest cell are unlocked */
			else if(ValC >= ValB && ValC >= ValA){
				theMaximum = ValC;

				if(theMaximum >= 2*baseCell){
					mutexCout.lock();
					cout << "Row-" << i << ",Col-" << j << " (" << matrix[i][j] << ") is incremented by " << ceil(baseCell/3) << " by stealing from the cell to the left (" << matrix[i][j-1] << ")." << endl;
					mutexCout.unlock();
					matrix[i][j] += ceil(baseCell/3);
					matrix[i][j-1] -= ceil(baseCell/3);
					isOkay = true;	
				}

				DynamicMatrixMutex[i+1][j].unlock();
				DynamicMatrixMutex[i][j-1].unlock();
				DynamicMatrixMutex[i-1][j].unlock();
				DynamicMatrixMutex[i][j].unlock();
			}

			break;
		}

		/* If there are any locked neigbour cells but others unlocked, it should be unlocked since at least one of them is locked */
		else if(boolFlag != true){
			if(boolA == true)
				DynamicMatrixMutex[i-1][j].unlock();
						
			if(boolC == true)
				DynamicMatrixMutex[i][j-1].unlock();
			
			if(boolB == true)
				DynamicMatrixMutex[i+1][j].unlock();
			
			if(boolD == true)
				DynamicMatrixMutex[i][j].unlock();

			this_thread::yield();
		}
	}


	/* Processing of the all other parts of the matrix that are not edge and corner */
	else if(j > 0  && j < col-1 && i > 0 && i < row-1){
		/* Finds the nearest neighbour cells unlocks them if they are available and not locked */ 
		boolA = DynamicMatrixMutex[i+1][j].try_lock();
		boolB = DynamicMatrixMutex[i-1][j].try_lock();
		boolE = DynamicMatrixMutex[i][j].try_lock();
		boolC = DynamicMatrixMutex[i][j-1].try_lock();
		boolD = DynamicMatrixMutex[i][j+1].try_lock();

		if(boolD == true && boolE == true && boolA == true && boolB == true && boolC == true)
			boolFlag = true;

		if(boolFlag){
			ValA = matrix[i-1][j];
			ValB = matrix[i][j-1];
			ValC = matrix[i][j+1];
			baseCell = matrix[i][j];
			ValD = matrix[i+1][j];

			/* Finding the highest number in the neigbour cells, to get enough time for printing, it locks cout then base cell gets number from their nearest cells since its number and all nearest cell are unlocked */
			if(ValA >= ValD && ValA >= ValB && ValA >= ValC){
				theMaximum = ValA;

				if(theMaximum >= 2*baseCell){
					mutexCout.lock();
					cout << "Row-" << i << ",Col-" << j << " (" << matrix[i][j] << ") is incremented by " << ceil(baseCell/3) << " by stealing from the cell above (" << matrix[i-1][j] << ")." << endl;
					mutexCout.unlock();
					matrix[i][j] += ceil(baseCell/3);
					matrix[i-1][j] -= ceil(baseCell/3);
					isOkay = true;			
				}

				DynamicMatrixMutex[i+1][j].unlock();
				DynamicMatrixMutex[i][j-1].unlock();
				DynamicMatrixMutex[i][j+1].unlock();
				DynamicMatrixMutex[i][j].unlock();
				DynamicMatrixMutex[i-1][j].unlock();
			}


			/* Finding the highest number in the neigbour cells, to get enough time for printing, it locks cout then base cell gets number from their nearest cells since its number and all nearest cell are unlocked */
			else if(ValB >= ValA && ValB >= ValC && ValB >= ValD){
				theMaximum = ValB;

				if(theMaximum >= 2*baseCell){
					mutexCout.lock();
					cout << "Row-" << i << ",Col-" << j << " (" << matrix[i][j] << ") is incremented by " << ceil(baseCell/3) << " by stealing from the cell to the left (" << matrix[i][j-1] << ")." << endl;
					mutexCout.unlock();
					matrix[i][j] += ceil(baseCell/3);
					matrix[i][j-1] -= ceil(baseCell/3);
					isOkay = true;
				}	

				DynamicMatrixMutex[i][j].unlock();
				DynamicMatrixMutex[i][j-1].unlock();
				DynamicMatrixMutex[i-1][j].unlock();
				DynamicMatrixMutex[i+1][j].unlock();
				DynamicMatrixMutex[i][j+1].unlock();
			}

			/* Finding the highest number in the neigbour cells, to get enough time for printing, it locks cout then base cell gets number from their nearest cells since its number and all nearest cell are unlocked */
			else if(ValC >= ValA && ValC >= ValD && ValC >= ValB){
				theMaximum = ValC;

				if(theMaximum>=2*baseCell){
					mutexCout.lock();
					cout << "Row-" << i << ",Col-" << j << " (" << matrix[i][j] << ") is incremented by " << ceil(baseCell/3) << " by stealing from the cell to the right (" << matrix[i][j+1] << ")." << endl;
					mutexCout.unlock();
					matrix[i][j] += ceil(baseCell/3);
					matrix[i][j+1] -= ceil(baseCell/3);
					isOkay=true;
				}

				DynamicMatrixMutex[i][j].unlock();
				DynamicMatrixMutex[i][j-1].unlock();
				DynamicMatrixMutex[i-1][j].unlock();
				DynamicMatrixMutex[i+1][j].unlock();
				DynamicMatrixMutex[i][j+1].unlock();
			}

			/* Finding the highest number in the neigbour cells, to get enough time for printing, it locks cout then base cell gets number from their nearest cells since its number and all nearest cell are unlocked */
			else if(ValD >= ValC && ValD >= ValA && ValD >= ValB){
				theMaximum = ValD;

				if(theMaximum >= 2*baseCell){
					mutexCout.lock();
					cout << "Row-" << i << ",Col-" << j << " (" << matrix[i][j] << ") is incremented by " << ceil(baseCell/3) << " by stealing from the cell below (" << matrix[i+1][j] << ")." << endl;
					mutexCout.unlock();
					matrix[i][j] += ceil(baseCell/3);
					matrix[i+1][j] -= ceil(baseCell/3);
					isOkay = true;
				}

				DynamicMatrixMutex[i][j-1].unlock();
				DynamicMatrixMutex[i-1][j].unlock();
				DynamicMatrixMutex[i+1][j].unlock();
				DynamicMatrixMutex[i][j+1].unlock();
				DynamicMatrixMutex[i][j].unlock();
			}
			break;
		}

		/* If there are any locked neigbour cells but others unlocked, it should be unlocked since at least one of them is locked */
		else if(boolFlag != true){
			if(boolA == true)
				DynamicMatrixMutex[i+1][j].unlock();

			if(boolE == true)
				DynamicMatrixMutex[i][j].unlock();

			if(boolB == true)
				DynamicMatrixMutex[i-1][j].unlock();

			if(boolC == true)
				DynamicMatrixMutex[i][j-1].unlock();

			if(boolD == true)
				DynamicMatrixMutex[i][j+1].unlock();

			this_thread::yield();

			}
		}
	}
}


int main(){

	int number = 0, row = 0, col = 0;
	ifstream f;
	string fName = "", theLine = "";

	cout << "Welcome to the last homework :(" << endl;
	cout << "Please enter a file name for the matrix: ";

	cin >> fName;
	cout << "-----" << endl;

	f.open(fName.c_str());

	/* File reading fails -> terminates the program */
	if(f.fail()){
		cout << "File could not be opened!! Exiting..." << endl;
		return 1;
	}
	
	/* Creation of dynamically allocated matrix with the help of pointers */ 
	getline(f,theLine);
	istringstream ss(theLine);

	ss >> number;
	row = number;

	ss >> number;
	col = number;

	/* 2D dynamically allocated matrix */
	int** matrix = new int* [row];
	for(int i = 0; i < row; i++)
		matrix[i] = new int[col];
	
	int numbers = 0;
	int i = 0, j = 0;
	while(getline(f,theLine)){

		istringstream ss(theLine);
		j = 0;

		while(ss >> number){
			matrix[i][j] = number;
			j++;
		}
		i++;
	}

	cout << "Printing the original matrix:" << endl;
	displayMatrix(matrix,row,col);

	cout << "------" << endl;
	cout << "A new round starts" << endl;

	thread** threadmatrix = new thread* [row];

	/* Declaration of dynamically allocated thread matrix */
	for(int i = 0; i< row; i++)
		threadmatrix[i] = new thread[col];

	DynamicMatrixMutex = new mutex*[row];	
	for(int i = 0; i < row; i++)
		DynamicMatrixMutex[i] = new mutex[col];

	/* While loop makes the process continunty */
	while(isOkay){
		isOkay = false;
		for(int i = 0; i < row; i++){
			for(int j = 0; j < col; j++)
				/* Declaration of matrix that has threads in it */
				threadmatrix[i][j] = thread(&TheFunction,i,j,ref(matrix),row,col);
		}
	
		/* Thread matix joins for the precessor in the gpu */ 
		for(int i = 0; i < row; i++){
			for(int j = 0; j < col; j++)
				threadmatrix[i][j].join();
		}

		/* Means that there is a change in matrix, updates it */
		if(isOkay){
			cout << "The round ends with updates" << endl;
			cout << "Printing the matrix after the updates:" << endl;

			displayMatrix(matrix,row,col);

			cout << "----" << endl;
			cout << "A new round starts" << endl;
		}
	}

	cout << "The round ends without updates." << endl;
	cout << "------" << endl;
	cout << "Program is exiting..." << endl;	

	/* Deletion of three matrixes that were created dynamically from heap memory */
	MatrixMutexDelete(DynamicMatrixMutex,row);
	MatrixThreadDelete(threadmatrix,row);
	MatrixDelete(matrix,row);

	return 0;
}