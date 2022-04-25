#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Property{
	public:
		int label;
		int numPixels;
		int minR;
		int minC;
		int maxR;
		int maxC;
};

class CClabel{
	public:
		int numRows;
		int numCols;
		int minVal;
		int maxVal;
		int newMin;
		int newMax;
		int newLabel;
		int trueNumCC;
		
		int** zeroFramedAry;
		int* EQAry;
		int neighborAry[5];
		Property* CCproperty;

		CClabel(int numRows, int numCols, int minVal, int maxVal){
			this->numRows = numRows;
			this->numCols = numCols;
			this->minVal = minVal;
			this->maxVal = maxVal;
			this->newMin = minVal;
			this->newMax = maxVal;	
			this->newLabel = 0;

			zeroFramedAry = new int*[numRows + 2];
			EQAry = new int[(numRows*numCols)/4];
			
			for(int i = 0; i < numRows + 2; i++){
				zeroFramedAry[i] = new int[numCols + 2];
			}
			
			for(int i = 0; i < (numRows*numCols)/4; i++){
				EQAry[i] = i;
			}
		}
		
		void zero2D(int** Ary){
			for(int i = 0; i < numRows + 2; i++){
				for(int j = 0; j < numCols + 2; j++){
					Ary[i][j] = 0;
				}
			}
		}
		
		void minus1D(int Ary[]){
			for(int i = 0; i < 5; i++){
				Ary[i] = -1;
			}
		}
		
		void loadImage(ifstream & input){
			int AryRow = 1;
			int AryCol = 1;
			
			int value;
			while(input >> value){
				zeroFramedAry[AryRow][AryCol] = value;				
				AryCol++;

				if(AryCol == numCols+1){
					AryRow++;
					AryCol = 1;
				}
			}
		}
		
		int labelCount(int numNeighbors){
			int max = -99999;
			for(int i = 0; i < numNeighbors; i++){
				if(neighborAry[i] > max){
					max = neighborAry[i];
				}
			}
			
			int freq[max+1] = {0};
			for(int i = 0; i < numNeighbors; i++){
				freq[neighborAry[i]]++;
			}
			
			int ct = 0;
			for(int i = 1; i < max+1; i++){
				if(freq[i] > 0){
					ct++;
				}
			}
			return ct;
		}
		
		int getSingleNeighbor(){
			int i = 0;
			while(neighborAry[i] == 0){
				i++;
			}
			return neighborAry[i];
		}
		
		int minLabel(int numNeighbors){
			int min = 99999;
			for(int i = 0; i < numNeighbors; i++){
				if(neighborAry[i] < min && neighborAry[i] != 0){
					min = neighborAry[i];
				}
			}
			return min;
		}
		
		void connect4Pass1(){
			int numDistictNeighbors;
			for(int i = 1; i < numRows+1; i++){
				for(int j = 1; j < numCols+1; j++){
					if(zeroFramedAry[i][j] > 0){					
						neighborAry[0] = zeroFramedAry[i-1][j];
						neighborAry[1] = zeroFramedAry[i][j-1];
						
						numDistictNeighbors = labelCount(2);
						
						if(numDistictNeighbors == 0){
							newLabel++;
							zeroFramedAry[i][j] = newLabel;
							EQAry[newLabel] = newLabel;
							newMax = newLabel;
							
							for(int k = 0; k < 2; k++){
								EQAry[neighborAry[k]] = zeroFramedAry[i][j];
							}
						}
						else if(numDistictNeighbors == 1){
							zeroFramedAry[i][j] = getSingleNeighbor();
						}else if(numDistictNeighbors > 1){
							zeroFramedAry[i][j] = minLabel(2);
							for(int k = 0; k < 2; k++){
								EQAry[neighborAry[k]] = zeroFramedAry[i][j];
							}
						}
					}
				}
			}
		}
		
		void connect4Pass2(){
			minus1D(neighborAry);
			int numDistictNeighbors;
			
			for(int i = numRows; i >= 1; i--){
				for(int j = numCols; j >= 1; j--){
					if(zeroFramedAry[i][j] > 0){
						neighborAry[0] = zeroFramedAry[i][j+1];
						neighborAry[1] = zeroFramedAry[i+1][j];
						neighborAry[2] = zeroFramedAry[i][j];
						
						numDistictNeighbors = labelCount(3);

						if(numDistictNeighbors > 1){
							int min = minLabel(3);						
							if(zeroFramedAry[i][j] > min){
								EQAry[zeroFramedAry[i][j]] = min;
								zeroFramedAry[i][j] = min;
							}
						}
					}
				}
			}
		}
		
		void connect8Pass1(){
			int numDistictNeighbors;
			for(int i = 1; i < numRows+1; i++){
				for(int j = 1; j < numCols+1; j++){
					if(zeroFramedAry[i][j] > 0){					
						neighborAry[0] = zeroFramedAry[i-1][j-1];
						neighborAry[1] = zeroFramedAry[i-1][j];
						neighborAry[2] = zeroFramedAry[i-1][j+1];
						neighborAry[3] = zeroFramedAry[i][j-1];

						numDistictNeighbors = labelCount(4);
						
						if(numDistictNeighbors == 0){
							newLabel++;
							zeroFramedAry[i][j] = newLabel;
							EQAry[newLabel] = newLabel;
							newMax = newLabel;
							
							for(int k = 0; k < 4; k++){
								EQAry[neighborAry[k]] = zeroFramedAry[i][j];
							}
						}
						else if(numDistictNeighbors == 1){
							zeroFramedAry[i][j] = getSingleNeighbor();
						}else if(numDistictNeighbors > 1){
							zeroFramedAry[i][j] = minLabel(4);
							for(int k = 0; k < 4; k++){
								EQAry[neighborAry[k]] = zeroFramedAry[i][j];
							}
						}
					}
				}
			}
		}
		
		void connect8Pass2(){
			minus1D(neighborAry);
			int numDistictNeighbors;

			for(int i = numRows; i >= 1; i--){
				for(int j = numCols; j >= 1; j--){
					if(zeroFramedAry[i][j] > 0){						
						neighborAry[0] = zeroFramedAry[i][j+1];
						neighborAry[1] = zeroFramedAry[i+1][j-1];
						neighborAry[2] = zeroFramedAry[i+1][j];
						neighborAry[3] = zeroFramedAry[i+1][j+1];
						neighborAry[4] = zeroFramedAry[i][j];

						numDistictNeighbors = labelCount(5);

						if(numDistictNeighbors > 1){
							int min = minLabel(5);						
							if(zeroFramedAry[i][j] > min){
								EQAry[zeroFramedAry[i][j]] = min;
								zeroFramedAry[i][j] = min;
							}
						}
					}
				}
			}
		}
		
		void connectPass3(){
			newMax = trueNumCC;
			CCproperty = new Property[trueNumCC];
			
			for(int i = 1; i < trueNumCC+1; i++){
				CCproperty[i].label = i;
				CCproperty[i].numPixels = 0;
				CCproperty[i].minR = 99999;
				CCproperty[i].minC = 99999;
				CCproperty[i].maxR = -99999;
				CCproperty[i].maxC = -99999;
			}
			
			for(int i = 1; i < numRows+1; i++){
				for(int j = 1; j < numCols+1; j++){
					if(zeroFramedAry[i][j] > 0){
						zeroFramedAry[i][j] = EQAry[zeroFramedAry[i][j]];
						CCproperty[zeroFramedAry[i][j]].numPixels++;
						
						if(CCproperty[zeroFramedAry[i][j]].minR > i){
							CCproperty[zeroFramedAry[i][j]].minR = i;
						}
						if(CCproperty[zeroFramedAry[i][j]].minC > j){
							CCproperty[zeroFramedAry[i][j]].minC = j;
						}
						if(CCproperty[zeroFramedAry[i][j]].maxR < i){
							CCproperty[zeroFramedAry[i][j]].maxR = i;
						}
						if(CCproperty[zeroFramedAry[i][j]].maxC < j){
							CCproperty[zeroFramedAry[i][j]].maxC = j;
						}
					}
				}
			}
		}
		
		void drawBoxes(){
			int index = 1;
			
			while(index < trueNumCC+1){
				int minRow = CCproperty[index].minR;
				int minCol = CCproperty[index].minC;
				int maxRow = CCproperty[index].maxR;
				int maxCol = CCproperty[index].maxC;
				int label = CCproperty[index].label;
				
				for(int i = minCol; i <= maxCol; i++){
					zeroFramedAry[minRow][i] = label;
					zeroFramedAry[maxRow][i] = label;
				}
				for(int i = minRow; i <= maxRow; i++){
					zeroFramedAry[i][minCol] = label;
					zeroFramedAry[i][maxCol] = label;
				}
				index++;
			}
		}
		
		int manageEQAry(){
			int readLabel = 0;
			int index = 1;
			
			while(index <= newLabel){
				if(index != EQAry[index]){
					EQAry[index] = EQAry[EQAry[index]];
				}
				else{
					readLabel++;
					EQAry[index] = readLabel;
				}
				index++;
			}
			return readLabel;
		}

		void printCCproperty(ofstream & outfile){
			outfile << numRows << " " << numCols << " " <<  newMin << " " <<  newMax << endl;
			outfile << trueNumCC << endl;
			
			for(int i = 1; i < trueNumCC+1; i++){
				outfile << CCproperty[i].label << endl;;
				outfile << CCproperty[i].numPixels << endl;
				outfile << CCproperty[i].minR << " " << CCproperty[i].minC << endl;
				outfile << CCproperty[i].maxR << " " << CCproperty[i].maxC << endl;
			}
		}
		
		void imgReformat(int** inAry, ofstream & outImg){
			outImg << numRows << " " << numCols << " " <<  newMin << " " <<  newMax << endl;

			string str = to_string(newMax);
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
		
		void printImg(int** inAry, ostream & outImg){
			outImg << numRows << " " << numCols << " " <<  newMin << " " <<  newMax << endl;

			string str = to_string(newMax);
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
						outImg << "0";
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
		
		void printEQary(ofstream & out){
			for(int i = 1; i <= newLabel; i++){
				out << i << " " << EQAry[i] << endl;
			}
		}
};


/*
	g++ -std=c++11 main.cpp
	
	a data1.txt 4 RFprettyPrintFile.txt labelFile.txt propertyFile.txt
	a data1.txt 8 RFprettyPrintFile.txt labelFile.txt propertyFile.txt

*/

int main(int argc, char *argv[]){
	ifstream inFile(argv[1]);
	ofstream RFprettyPrintFile(argv[3]), labelFile(argv[4]), propertyFile(argv[5]);

	int connectness = atoi(argv[2]);

	int numRows, numCols, minVal, maxVal;
	inFile >> numRows >> numCols >> minVal >> maxVal;
	CClabel img(numRows, numCols, minVal, maxVal);
	
	img.zero2D(img.zeroFramedAry);
	img.loadImage(inFile);
	
	if(connectness == 4){
		RFprettyPrintFile << "Pass 1:" << endl;
		img.connect4Pass1();
		img.imgReformat(img.zeroFramedAry, RFprettyPrintFile);
		
		RFprettyPrintFile << endl << "Equivalency Array after Pass 1: " << endl;
		img.printEQary(RFprettyPrintFile);
		
		RFprettyPrintFile << endl << "Pass 2: " << endl;
		img.connect4Pass2();
		img.imgReformat(img.zeroFramedAry, RFprettyPrintFile);
		
		RFprettyPrintFile << endl << "Equivalency Array after Pass 2: " << endl;
		img.printEQary(RFprettyPrintFile);
	}
	
	if(connectness == 8){
		RFprettyPrintFile << "Pass 1: " << endl;
		img.connect8Pass1();
		img.imgReformat(img.zeroFramedAry, RFprettyPrintFile);
		
		RFprettyPrintFile << endl << "Equivalency Array after Pass 1: " << endl;
		img.printEQary(RFprettyPrintFile);
		
		RFprettyPrintFile << endl << "Pass 2: " << endl;
		img.connect8Pass2();
		img.imgReformat(img.zeroFramedAry, RFprettyPrintFile);
		
		RFprettyPrintFile << endl << "Equivalency Array after Pass 2: " << endl;
		img.printEQary(RFprettyPrintFile);
	}
	
	img.trueNumCC = img.manageEQAry();
	RFprettyPrintFile << endl << "Equivalency Array after EQ Table Management: " << endl;
	img.printEQary(RFprettyPrintFile);
	
	RFprettyPrintFile << endl << "Pass 3: " << endl;
	img.connectPass3();
	img.imgReformat(img.zeroFramedAry, RFprettyPrintFile);
	
	RFprettyPrintFile << endl << "Equivalency Array after Pass 3: " << endl;
	img.printEQary(RFprettyPrintFile);
	
	img.printImg(img.zeroFramedAry, labelFile);
	img.printCCproperty(propertyFile);
	
	RFprettyPrintFile << endl << "Draw Boxes: " << endl;
	img.drawBoxes();
	img.imgReformat(img.zeroFramedAry, RFprettyPrintFile);
	
	RFprettyPrintFile << endl << "Number of Connected Components: " << img.trueNumCC << endl;

	
	inFile.close();
	RFprettyPrintFile.close();
	labelFile.close();
	propertyFile.close();
}
