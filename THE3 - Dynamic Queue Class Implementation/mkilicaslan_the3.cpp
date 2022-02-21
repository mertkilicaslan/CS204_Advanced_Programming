//
//  main.cpp
//  THE3
//
//  Created by Duygu Karaoglan Altop on 8.08.2021.
//  Copyright © 2021 Duygu Karaoglan Altop. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include "DynQueue.h"
using namespace std;

void openFile(ifstream &inFile){
    string fName;
    do{
        cout << "Please enter a filename: ";
        cin >> fName;
        inFile.clear();
        inFile.open(fName.c_str());
    }while(inFile.fail());
}

int main() {
    
    ifstream inFile;
    openFile(inFile);
    
    DynQueue<int> myQueue1;
    inFile >> myQueue1;
    cout << myQueue1 << endl;
    myQueue1 += 2;
    cout << myQueue1 << endl;
    
    DynQueue<int> myQueue2(myQueue1);
    cout << myQueue2 << endl;
    myQueue2 *= 2;
    cout << myQueue2 << endl;
    
    DynQueue<int> myQueue3;
    myQueue3 = myQueue1 + myQueue2;
    cout << myQueue3 << endl;
    
    cout << myQueue3 + myQueue3 << endl;
    inFile.close();
    
    openFile(inFile);
    DynQueue<string> myQueue4;
    inFile >> myQueue4;
    cout << myQueue4 << endl;
    myQueue4 += "2";
    cout << myQueue4 << endl;
    
    DynQueue<string> myQueue5(myQueue4);
    cout << myQueue5 << endl;
    myQueue5 *= 2;
    cout << myQueue5 << endl;
    
    DynQueue<string> myQueue6;
    myQueue6 = myQueue4 + myQueue5;
    cout << myQueue6 << endl;
    
    cout << myQueue6 + myQueue6 << endl;
    inFile.close();
    
    
    return 0;
}
