#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

const int CPmasks[8][5][5] = {
			{{0,0,0,0,0},{0,0,0,0,0},{0,0,1,0,0},{0,1,1,1,0},{1,1,1,1,1}},
			{{1,0,0,0,0},{1,1,0,0,0},{1,1,1,0,0},{1,1,0,0,0},{1,0,0,0,0}},
			{{1,1,1,1,1},{0,1,1,1,0},{0,0,1,0,0},{0,0,0,0,0},{0,0,0,0,0}},
			{{0,0,0,0,1},{0,0,0,1,1},{0,0,1,1,1},{0,0,0,1,1},{0,0,0,0,1}},
			{{1,1,1,0,0},{1,1,1,0,0},{1,1,1,0,0},{0,0,0,0,0},{0,0,0,0,0}},
			{{0,0,1,1,1},{0,0,1,1,1},{0,0,1,1,1},{0,0,0,0,0},{0,0,0,0,0}},
			{{0,0,0,0,0},{0,0,0,0,0},{0,0,1,1,1},{0,0,1,1,1},{0,0,1,1,1}},
			{{0,0,0,0,0},{0,0,0,0,0},{1,1,1,0,0},{1,1,1,0,0},{1,1,1,0,0}}
		};
		
class imageProcessing{
	public:
		int numRows;
		int numCols;
		int minVal;
		int maxVal;
		int newMin;
		int newMax;
		int thrVal;

		int neighborAry[9];
		int CPneighbor5x5[5][5];
		
		int** mirror3by3Ary;
		int** mirror5by5Ary;
		int** avgAry;
		int** medianAry;
		int** CPAry;
		
		int** thrAry3;
		int** thrAry5;

		imageProcessing(int numRows, int numCols, int minVal, int maxVal, int thrVal){
			this->numRows = numRows;
			this->numCols = numCols;
			this->minVal = minVal;
			this->maxVal = maxVal;
			this->thrVal = thrVal;
			this->newMin = minVal;
			this->newMax = maxVal;

			mirror3by3Ary = new int*[numRows + 2];
			avgAry = new int*[numRows + 2];
			medianAry = new int*[numRows + 2];
			thrAry3 = new int*[numRows + 2];
			
			for(int i = 0; i < numRows + 2; i++){
				mirror3by3Ary[i] = new int[numCols + 2];
				avgAry[i] = new int[numCols + 2];
				medianAry[i] = new int[numCols + 2];
				thrAry3[i] = new int[numCols + 2];
			}
			
			mirror5by5Ary = new int*[numRows + 4];
			CPAry = new int*[numRows + 4];
			thrAry5 = new int*[numRows + 4];
			
			for(int i = 0; i < numRows + 4; i++){
				mirror5by5Ary[i] = new int[numCols + 4];
				CPAry[i] = new int[numCols + 4];
				thrAry5[i] = new int[numCols + 4];
			}
		}
		
		void loadImage(ifstream & input){
			int Ary3Row = 1;
			int Ary3Col = 1;
			int Ary5Row = 2;
			int Ary5Col = 2;
			
			int value;
			while(input >> value){
				mirror3by3Ary[Ary3Row][Ary3Col] = value;				
				mirror5by5Ary[Ary5Row][Ary5Col] = value;
				
				Ary3Col++;
				Ary5Col++;

				if(Ary3Col == numCols+1 && Ary5Col == numCols+2){
					Ary3Row++;
					Ary5Row++;
					
					Ary3Col = 1;
					Ary5Col = 2;
				}
			}
		}
		
		void mirrorFraming(int** Ary, int frameSize){
			int totalR = numRows + ( 2 * frameSize ) - 1;
			int totalC = numCols + ( 2 * frameSize ) - 1;
			
			for(int r = 0; r < totalR; r++){
				int currentCol = frameSize;
				for(int c = frameSize-1; c > -1; c--){
					Ary[r][c] = Ary[r][currentCol];
					currentCol++;
				}
				
				currentCol = frameSize + numCols-1;
				for(int c = frameSize + numCols; c <= totalC; c++){
					Ary[r][c] = Ary[r][currentCol];
					currentCol--;
				}
			}
			
			for(int c = 0; c <= totalC; c++){
				int currentRow = frameSize;
				for(int r = frameSize-1; r > -1; r--){
					Ary[r][c] = Ary[currentRow][c];
					currentRow++;
				}
				
				currentRow = frameSize + numRows-1;
				for(int r = frameSize + numRows; r <= totalR; r++){
					Ary[r][c] = Ary[currentRow][c];
					currentRow--;
				}
			}
		}
		
		void imgReformat(int** inAry, ofstream & outImg, int frameSize){
			outImg << numRows << " " << numCols << " " <<  newMin << " " <<  newMax << endl;

			string str = to_string(newMax);
			int width = str.length();

			int r = frameSize; 
			int c;
			
			while(r < (numRows+frameSize)){
				c = frameSize;
				while(c < (numCols+frameSize)){
					outImg << inAry[r][c];
					
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
		
		void computeAvg(){
			newMin = 9999;
			newMax = 0;
			
			int r = 1;
			int c, sum;
			while(r < numRows+1){
				c = 1;
				while(c < numCols+1){
					avgAry[r][c] = avg3x3(r,c);
					
					if(newMin > avgAry[r][c]) newMin = avgAry[r][c];
					if(newMax < avgAry[r][c]) newMax = avgAry[r][c];

					c++;
				}
				r++;
			}
		}
		
		int avg3x3(int r, int c){
			double sum = 0;
			for(int i = r-1; i <= r+1; i++){
				for(int j = c-1; j <= c+1; j++){
					sum += mirror3by3Ary[i][j];
				}
			}
			return round(sum/9);
		}
		
		void computeMedian(){
			newMin = 9999;
			newMax = 0;
			
			int r = 1;
			int c;
			while(r < numRows+1){
				c = 1;
				while(c < numCols+1){
					medianAry[r][c] = median3x3(r,c);
					
					if(newMin > medianAry[r][c]) newMin = medianAry[r][c];
					if(newMax < medianAry[r][c]) newMax = medianAry[r][c];
					c++;
				}
				r++;
			}	
		}
		
		int median3x3(int r, int c){
			double sum = 0;
			int n = 0;
			for(int i = r-1; i <= r+1; i++){
				for(int j = c-1; j <= c+1; j++){
					neighborAry[n] = mirror3by3Ary[i][j];
					n++;
				}
			}
			sort();
			return neighborAry[4];
		}
		
		void sort(){
			int temp;
			for(int i = 0; i < 9; i++){
				for(int j = 0; j < 9; j++){
					if(neighborAry[j] > neighborAry[i]){
						temp = neighborAry[j];
						neighborAry[j] = neighborAry[i];
						neighborAry[i] = temp;
					}
				}
			}
		}
		
		void computeCPfilter(){
			newMin = 9999;
			newMax = 0;
			
			int r = 2;
			int c;
			while(r < numRows+2){
				c = 2;
				while(c < numCols+2){
					CP5x5(r,c);
					CPAry[r][c] = convolution(r,c);

					if(newMin > CPAry[r][c]) newMin = CPAry[r][c];
					if(newMax < CPAry[r][c]) newMax = CPAry[r][c];
					c++;
				}
				r++;
			}
		}
		
		void CP5x5(int r, int c){
			int x,y;
			x = 0;
			for(int i = r-2; i <= r+2; i++){
				y = 0;
				for(int j = c-2; j <= c+2; j++){
					CPneighbor5x5[x][y] = mirror5by5Ary[i][j];
					y++;
				}
				x++;
			}
		}
		
		int convolution(int r, int c){
			float smallestD = 99999;
			double smallestAvg;
			double sum;
			float d;
			for(int i = 0; i < 8; i++){
				sum = 0;
				for(int j = 0; j < 5; j++){
					for(int k = 0; k < 5; k++){
						sum += CPmasks[i][j][k] * CPneighbor5x5[j][k];
					}
				}
				d = abs((sum/9) - mirror5by5Ary[r][c]);
				if(d < smallestD){
					smallestD = d;
					smallestAvg = round(sum/9);
				}
			}
			return smallestAvg;
		}
		
		void threshold(int** avgAry, int** thrAry, int frameSize){
			newMin = 0;
			newMax = 1;
			
			int r,c;
			r = frameSize;
			
			while(r < (numRows+frameSize)){
				c = frameSize;
				
				while(c < (numCols + frameSize)){
					if(avgAry[r][c] >= thrVal){
						thrAry[r][c] =  1;
					}
					else
						thrAry[r][c] =  0;
					c++;
				}
				r++;
			}
		}
		
		void aryToFile(int** ary, ostream & outFile, int frameSize){
			outFile << numRows << " " << numCols << " " <<  newMin << " " <<  newMax << endl;
			
			for(int i = frameSize; i < numRows+frameSize; i++){
				for(int j = frameSize; j < numCols+frameSize; j++){
					outFile << ary[i][j] << " ";
				}
				outFile << endl;
			}
		}
		
		void prettyPrint(int** ary, ostream & outFile, int frameSize){
			outFile << numRows << " " << numCols << " " <<  newMin << " " <<  newMax << endl;
			
			for(int i = frameSize; i < numRows+frameSize; i++){
				for(int j = frameSize; j < numCols+frameSize; j++){
					if(ary[i][j] == 0) outFile << ". ";
					else outFile << 1 << " ";
				}
				outFile << endl;
			}
		}
};


/*
	g++ -std=c++11 main.cpp
	
	a data1.txt 30 rfImg.txt AvgOutImg.txt AvgThrImg.txt AvgPrettyPrint.txt MedianOutImg.txt MedianThrImg.txt MedianPrettyPrint.txt CPOutImg.txt CPThrImg.txt CPPrettyPrint.txt
*/

int main(int argc, char *argv[]){
	ifstream inFile(argv[1]);
	
	ofstream rfImg(argv[3]), AvgOutImg(argv[4]), AvgThrImg(argv[5]), AvgPrettyPrint(argv[6]);
	ofstream MedianOutImg(argv[7]), MedianThrImg(argv[8]), MedianPrettyPrint(argv[9]);
	ofstream CPOutImg(argv[10]), CPThrImg(argv[11]), CPPrettyPrint(argv[12]);
	
	int thrVal = atoi(argv[2]);

	int numRows, numCols, minVal, maxVal;
	inFile >> numRows >> numCols >> minVal >> maxVal;
	imageProcessing img(numRows, numCols, minVal, maxVal, thrVal);
	
	img.loadImage(inFile);
	
	img.mirrorFraming(img.mirror3by3Ary, 1);
	img.imgReformat(img.mirror3by3Ary, rfImg, 1);
	
	img.computeAvg();
	img.imgReformat(img.avgAry, AvgOutImg, 1);
	img.threshold(img.avgAry, img.thrAry3, 1);
	img.aryToFile(img.thrAry3, AvgThrImg, 1);
	img.prettyPrint(img.thrAry3, AvgPrettyPrint, 1);
	
	img.computeMedian();
	img.imgReformat(img.medianAry, MedianOutImg, 1);
	img.threshold(img.medianAry, img.thrAry3, 1);
	img.aryToFile(img.thrAry3, MedianThrImg, 1);
	img.prettyPrint(img.thrAry3, MedianPrettyPrint, 1);
	
	img.mirrorFraming(img.mirror5by5Ary, 2);

	img.computeCPfilter();
	img.imgReformat(img.CPAry, CPOutImg, 2);
	img.threshold(img.CPAry, img.thrAry5, 2);
	img.aryToFile(img.thrAry5, CPThrImg, 2);
	img.prettyPrint(img.thrAry5, CPPrettyPrint, 2);
	
	inFile.close();
	rfImg.close();
	AvgOutImg.close();
	AvgThrImg.close();
	AvgPrettyPrint.close();
	MedianOutImg.close();
	MedianThrImg.close();
	MedianPrettyPrint.close();
	CPOutImg.close();
	CPThrImg.close();
	CPPrettyPrint.close();
}
