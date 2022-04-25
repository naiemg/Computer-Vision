#include <iostream>
#include <fstream>
using namespace std;

class image{
	public:
		int numRows;
		int numCols;
		int minVal;
		int maxVal;
		int* histAry = new int[maxVal+1];
		int thresholdValue;
		
		image(int numRows, int numCols, int minVal, int maxVal){
			this->numRows = numRows;
			this->numCols = numCols;
			this->minVal = minVal;
			this->maxVal = maxVal;
			
			for(int i = 0; i < maxVal+1; i++){
				histAry[i] = 0;
			}
		}
		
		void computeHist(ifstream & input){
			int value;
			while(input >> value){
				histAry[value]++;
			}
		}
		
		void printHist(ofstream & output){
			output << numRows << " " << numCols << " " << minVal << " " << maxVal << endl;
			
			for(int i = 0; i < maxVal+1; i++){
				output << i << " " << histAry[i] << endl;
			}
		}
		
		void displayHist(ofstream & output){
			output << numRows << " " << numCols << " " << minVal << " " << maxVal << endl;
			
			for(int i = 0; i < maxVal+1; i++){
				output << i << "\t" << "(" << histAry[i] << ")\t:";
				
				for(int j = 0; j < histAry[i]; j++){
					if(j == 70)
						break;
					else
						output << "+";
				}
				output << endl;
			}
		}
		
		void threshold(ifstream & inFile, ofstream & outFile3, ofstream & outFile4, int thrVal){
			this->minVal = 0;
			this->maxVal = 1;
			
			outFile3 << numRows << " " << numCols << " " << minVal << " " << maxVal << endl;
			outFile4 << numRows << " " << numCols << " " << minVal << " " << maxVal << endl;

			int pixelVal;
			
			for(int i = 0; i < numRows; i++){
				for(int j = 0; j < numCols; j++){
					inFile >> pixelVal;
					if(pixelVal >= thrVal){
						outFile3 << 1 << " ";
						outFile4 << 1 << " ";
					}
					else{
						outFile3 << 0 << " ";
						outFile4  << ". ";
					}
				}
				outFile3 << endl;
				outFile4 << endl;
			}
		}
};

int main(int argc, char *argv[]){
	ifstream inFile(argv[1]);
	ofstream outFile1(argv[3]);
	ofstream outFile2(argv[4]);
	ofstream outFile3(argv[5]);
	ofstream outFile4(argv[6]);
	
	int numRows, numCols, minVal, maxVal;
	inFile >> numRows >> numCols >> minVal >> maxVal;
	
	image img(numRows, numCols, minVal, maxVal);

	img.computeHist(inFile);
	img.printHist(outFile1);
	img.displayHist(outFile2);
	
	inFile.close();

	int thrVal = atoi(argv[2]);
	outFile3 << "The threshold value used is: " << thrVal << endl;
	outFile4 << "The threshold value used is: " << thrVal << endl;
	
	ifstream inFile2(argv[1]);
	
	int throwaway;
	for(int i = 0; i < 4; i++){
		inFile2 >> throwaway;
	}
	
	img.threshold(inFile2, outFile3, outFile4, thrVal);
	
	inFile.close();
	outFile1.close();
	outFile2.close();
	outFile3.close();
	outFile4.close();
}
