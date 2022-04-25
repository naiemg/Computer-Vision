#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int connectCases[6][3][3] = {
	{{1,0,-1},{0,1,-1},{-1,-1,-1}},
	{{-1,0,1},{-1,1,0},{-1,-1,-1}},
	{{-1,-1,-1},{0,1,-1},{1,0,-1}},
	{{-1,-1,-1},{-1,1,0},{-1,0,1}},
	{{-2,-2,-2},{0,1,0},{-3,-3,-3}},
	{{-2,0,-3},{-2,1,-3},{-2,0,-3}}
};

class Thinning{
	public:
		int numRows;
		int numCols;
		int minVal;
		int maxVal;
		int changeFlag;
		int cycleCount;
		
		int** aryOne;
		int** aryTwo;

		Thinning(int numRows, int numCols, int minVal, int maxVal){
			this->numRows = numRows;
			this->numCols = numCols;
			this->minVal = minVal;
			this->maxVal = maxVal;

			this->aryOne = new int*[numRows + 2];
			this->aryTwo = new int*[numRows + 2];
			
			for(int i = 0; i < numRows + 2; i++){
				aryOne[i] = new int[numCols + 2];
				aryTwo[i] = new int[numCols + 2];
			}
		}
		
		void zero2D(int** Ary){
			for(int i = 0; i < numRows + 2; i++){
				for(int j = 0; j < numCols + 2; j++){
					Ary[i][j] = 0;
				}
			}
		}
		
		void loadImage(ifstream & input){
			int AryRow = 1;
			int AryCol = 1;
			
			int value;
			while(input >> value){
				aryOne[AryRow][AryCol] = value;
				aryTwo[AryRow][AryCol] = value;				
			
				AryCol++;

				if(AryCol == numCols+1){
					AryRow++;
					AryCol = 1;
				}
			}
		}
		
		void copyArys(){
			for(int i = 1; i < numRows+1; i++){
				for(int j = 1; j < numCols+1; j++){
					aryOne[i][j] = aryTwo[i][j];
				}
			}
		}
		
		void NorthThinning(){
			for(int i = 1; i < numRows+1; i++){
				for(int j = 1; j < numCols+1; j++){
					if(aryOne[i][j] > 0){
						if(isNeighborZero("North", i, j) && hasXobjectNeighbors(i, j, 4) && !isConnector(i,j)){
							aryTwo[i][j] = 0;
							changeFlag++;						
						}
					}
				}
			}
		}
		
		void SouthThinning(){
			for(int i = 1; i < numRows+1; i++){
				for(int j = 1; j < numCols+1; j++){
					if(aryOne[i][j] > 0){
						if(isNeighborZero("South", i, j) && hasXobjectNeighbors(i, j, 4) && !isConnector(i,j)){
							aryTwo[i][j] = 0;
							changeFlag++;
						}
					}
				}
			}
		}
		
		void EastThinning(){
			for(int i = 1; i < numRows+1; i++){
				for(int j = 1; j < numCols+1; j++){
					if(aryOne[i][j] > 0){
						if(isNeighborZero("East", i, j) && hasXobjectNeighbors(i, j, 3) && !isConnector(i,j)){
							aryTwo[i][j] = 0;
							changeFlag++;
						}
					}
				}
			}
		}
		
		void WestThinning(){
			for(int i = 1; i < numRows+1; i++){
				for(int j = 1; j < numCols+1; j++){
					if(aryOne[i][j] > 0){
						if(isNeighborZero("West", i, j) && hasXobjectNeighbors(i, j, 3) && !isConnector(i,j)){
							aryTwo[i][j] = 0;
							changeFlag++;
						}
					}
				}
			}
		}
		
		bool isNeighborZero(string direction, int i, int j){
			if(direction == "North"){
				if(aryOne[i-1][j] == 0) return true;
			}
			if(direction == "East"){
				if(aryOne[i][j+1] == 0) return true;
			}
			if(direction == "South"){
				if(aryOne[i+1][j] == 0) return true;
			}
			if(direction == "West"){
				if(aryOne[i][j-1] == 0) return true;
			}
			return false;	
		}
		
		int numObjectNeighbors(int r, int c){
			int ct = 0;

			for(int i = r-1; i <= r+1; i++){
				for(int j = c-1; j <= c+1; j++){
					if(aryOne[i][j] > 0){
						ct++;
					}
				}
			}
			return ct-1;
		}
		
		bool hasXobjectNeighbors(int r, int c, int X){
			int numNeighbors = numObjectNeighbors(r,c);
			if(numNeighbors >= X){
				return true;
			}
			else{
				return false;
			}
		}
		
		bool isConnector(int r, int c){ 
			for(int CCase = 0; CCase < 4; CCase++){
				if(matchesCase(CCase, r,c) == true){
					return true;
				}
			}
			for(int CCase = 4; CCase < 6; CCase++){
				if(matchesCase1And2(CCase, r,c) == true){
					return true;
				}
			}
			return false;
		}
		
		bool matchesCase(int CCase, int r, int c){
			int caseR, caseC;
			caseR = -1;
			for(int i = r-1; i <= r+1; i++){
				caseR++;
				caseC = 0;
				for(int j = c-1; j <= c+1; j++){
					if(connectCases[CCase][caseR][caseC] >= 0){
						if(connectCases[CCase][caseR][caseC] != aryOne[i][j]){
							return false;
						}
					}
					caseC++;
				}
			}
			return true;
		}
		
		bool matchesCase1And2(int CCase, int r, int c){
			int sideA = 0, sideB = 0;
			int caseR, caseC;
			caseR = -1;
			for(int i = r-1; i <= r+1; i++){
				caseR++;
				caseC = 0;
				for(int j = c-1; j <= c+1; j++){
					if(connectCases[CCase][caseR][caseC] == 0){
						if(connectCases[CCase][caseR][caseC] != aryOne[i][j]){
							return false;
						}
					}
					if(connectCases[CCase][caseR][caseC] == -2){
						if(aryOne[i][j] > 0){
							sideA++;
						}
					}
					if(connectCases[CCase][caseR][caseC] == -3){
						if(aryOne[i][j] > 0){
							sideB++;
						}
					}
					caseC++;
				}
			}
			if(sideA > 0 && sideB > 0){
				return true;
			}
			return false;
		}
		
		void reformatPrettyPrint(int** inAry, ofstream & outImg){
			outImg << numRows << " " << numCols << " " <<  minVal << " " <<  maxVal << endl;

			string str = to_string(maxVal);
			int width = str.length();

			int r = 1; 
			int c;
			
			while(r < (numRows+1)){
				c = 1;
				while(c < (numCols+1)){
					if(inAry[r][c] > 0){
						outImg << inAry[r][c];
					}
					else{
						outImg << ".";
					}
					
					str = to_string(inAry[r][c]);
					int WW = str.length();
				
					while(WW <= width){
						outImg << " ";
						WW++;
					}
					c++;
				}
				r++;
				outImg << endl;
			}
		}
};


/*
	g++ -std=c++11 main.cpp
	
	a image1.txt outFile1.txt outFile2.txt
	a image2.txt outFile1.txt outFile2.txt
*/

int main(int argc, char *argv[]){
	ifstream inFile(argv[1]);
	ofstream outFile1(argv[2]), outFile2(argv[3]);

	int numRows, numCols, minVal, maxVal;
	inFile >> numRows >> numCols >> minVal >> maxVal;
	Thinning img(numRows, numCols, minVal, maxVal);
	
	img.zero2D(img.aryOne);
	img.zero2D(img.aryTwo);
	
	img.loadImage(inFile);
	img.cycleCount = 0;
	
	outFile2 << "Input Image: " << endl;
	img.reformatPrettyPrint(img.aryOne, outFile2);
	
	img.changeFlag = 99999;

	while(img.changeFlag > 0){
		img.changeFlag = 0;

		img.NorthThinning();
		img.copyArys();

		img.SouthThinning();
		img.copyArys();

		img.WestThinning();
		img.copyArys();
	
		img.EastThinning();
		img.copyArys();

		img.cycleCount++;
		
		outFile2 << "Result of thinning: Cycle " << img.cycleCount << endl;
		img.reformatPrettyPrint(img.aryOne, outFile2);
	}
	
	img.reformatPrettyPrint(img.aryOne, outFile1);
	
	inFile.close();
	outFile1.close();
	outFile2.close();
}
