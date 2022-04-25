#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

class kCurvature{
	class image{
		public:
			int numRows;
			int numCols;
			int minVal;
			int maxVal;
			
			int** imgAry;
			
			image(int numRows, int numCols, int minVal, int maxVal){
				this->numRows = numRows;
				this->numCols = numCols;
				this->minVal = minVal;
				this->maxVal = maxVal;
	
				this->imgAry = new int*[numRows];
				
				for(int i = 0; i < numRows; i++){
					imgAry[i] = new int[numCols];
				}
				
				zero2D();
			}
			
			image(){}
			
			void zero2D(){
				for(int i = 0; i < numRows; i++){
					for(int j = 0; j < numCols; j++){
						this->imgAry[i][j] = 0;
					}
				}
			}
			
			void reformatPrettyPrint(ofstream & outFile){
				for(int i = 0; i < numRows; i++){
					for(int j = 0; j < numCols; j++){
						if(this->imgAry[i][j] == 0)
							outFile << "." << " ";
						else
							outFile << this->imgAry[i][j] << " ";
					}
					outFile << endl;
				}
			}
	};
	
	class boundaryPt{
		public:
			int x;
			int y;
			double curvature;
			int localMax;
			int corner;
			
			void printPt(ofstream & outFile){
				outFile << "[";
				outFile << this->x << ", ";
				outFile << this->y << ", ";
				outFile << this->curvature << ", ";
				outFile << this->localMax << ", ";
				outFile << this->corner;
				outFile << "]," << endl;
			}
	};
	
	public:
		int K;
		int numPts;
		int label;
		int Q;
		int P;
		int R;
		
		boundaryPt* ptAry;
		image img;
		
		kCurvature(int numRows, int numCols, int minVal, int maxVal, int K, int label){
			img = image(numRows, numCols, minVal, maxVal);
			this->K = K;
			this->label = label;
		}
		
		void zeroPtAry(){
			for(int i = 0; i < this->numPts; i++){
				this->ptAry[i].x = 0;
				this->ptAry[i].y = 0;
				this->ptAry[i].curvature = 0;
				this->ptAry[i].localMax = 0;
				this->ptAry[i].corner = 0;
			}
		}
		
		void initialization(ifstream & inFile, string FileName){
			this->numPts = countPts(inFile);
			this->ptAry = new boundaryPt[this->numPts];
			zeroPtAry();
			
			inFile.close();
			inFile.clear();
			inFile.open(FileName);
						
			int throwaway = 0, garbage;
			while(throwaway < 5){
				inFile >> garbage;
				throwaway++;
			}
			
			int x, y, index = 0;
			
			while(inFile >> x >> y){
				storePt(x, y, index);
				index++;
			}
		}
		
		int countPts(ifstream & inFile){
			int count = 0;
			int row, col;
			
			while(inFile >> row >> col){
				count++;
			}
			return count;
		}
		
		void storePt(int x, int y, int index){
			ptAry[index].x = x;
			ptAry[index].y = y;
		}
		
		void printPtAry(ofstream & outFile){
			outFile << "Pt Ary:" << endl << endl;
			for(int i = 0; i < this->numPts; i++){
				this->ptAry[i].printPt(outFile);
			}
			outFile << endl << endl;
		}
		
		void cornerDetection(ofstream & outFile){
			this->Q = 0;
			this->P = this->K;
			this->R = 2 * this->K;
			
			int index, ct = 0;
			
			while(this->P != this->K || ct < this->numPts){
				index = this->P;
				double curvature = computeCurvature(this->Q, this->P, this->R, outFile);
				this->ptAry[index].curvature = curvature;
				
				outFile << "Q: " << this->Q << endl;
				outFile << "P: " << this->P << endl;
				outFile << "R: " << this->R << endl;
				outFile << "Index: " << index << endl;
				outFile << "x: " << this->ptAry[index].x << endl;
				outFile << "y: " << this->ptAry[index].y << endl;
				outFile << "curvature: " << this->ptAry[index].curvature << endl << endl;
				
				this->Q = (this->Q+1) % this->numPts;
				this->P = (this->P+1) % this->numPts;
				this->R = (this->R+1) % this->numPts;
				ct++;
			}
		}
		
		double computeCurvature(int Q, int P, int R, ofstream & outFile){
			double slope1, slope2, curvature;
			int r1, c1, r2, c2, r3, c3;
			
			r1 = this->ptAry[Q].x;
			c1 = this->ptAry[Q].y;
			
			r2 = this->ptAry[P].x;
			c2 = this->ptAry[P].y;
			
			r3 = this->ptAry[R].x;
			c3 = this->ptAry[R].y;
			
			double A = r1-r2;
			double B = r2-r3;

			if(A == 0){
				if(c1-c2 > 0)
					A = -0.5;
				else
					A = 0.5;
			}
			
			if(B == 0){
				if(c2-c3 > 0)
					B = 0.5;
				else
					B = -0.5;
			}
			
			slope1 = (c1-c2) / A;
			slope2 = (c2-c3) / B;

			curvature = abs(slope1 - slope2);
			return curvature;
		}
		
		void computeLocalMaxima() {
			for(int i = 0; i < this->numPts; i++){
				int j, visited = 0, ct = 0;
				j = (i - 2 + this->numPts) % this->numPts;
				while(visited <  5 && this->ptAry[i].curvature){
					if(this->ptAry[i].curvature >= this->ptAry[j].curvature){
						if(i != j){
							ct++;
						}
					}
					
					j = (j+1) % this->numPts;
					visited++;
				}
				if(ct == 4){
					this->ptAry[i].localMax = 1;
				}
			}
		}
		
		void markCorner(){
			for(int i = 0; i < this->numPts; i++){
				if(this->ptAry[i].localMax == 1 && (this->ptAry[(i - 2 + this->numPts) % this->numPts].localMax == 0 && this->ptAry[(i+2) % this->numPts].localMax == 0)){
					this->ptAry[i].corner = 9;
				}
				else{
					this->ptAry[i].corner = 1;
				}
			}
		}
		
		void display(ofstream & outFile){
			int x, y;
			for(int i = 0; i < this->numPts; i++){
				x = this->ptAry[i].x;
				y = this->ptAry[i].y;
				this->img.imgAry[x][y] = this->ptAry[i].corner;
			}
			outFile << img.numRows << " " << img.numCols << " " << img.minVal << " " << img.maxVal << endl;
			this->img.reformatPrettyPrint(outFile);
		}
		
		void printBoundary(ofstream & outFile){
			outFile << img.numRows << " " << img.numCols << " " << img.minVal << " " << img.maxVal << endl;
			outFile << this->label << endl;
			outFile << this->numPts << endl;
			
			for(int i = 0; i < this->numPts; i++){
				outFile << this->ptAry[i].x;
				outFile << " ";
				outFile << this->ptAry[i].y;
				outFile << " ";
				outFile << this->ptAry[i].corner << endl;;
			}
		}
};


/*
	cd Desktop/381/Project 8
	
	g++ -std=c++11 main.cpp
	
	a DataA.txt outFile1.txt outFile2.txt outFile3.txt
	a DataB.txt outFile1.txt outFile2.txt outFile3.txt
	a DataC.txt outFile1.txt outFile2.txt outFile3.txt
	a DataD.txt outFile1.txt outFile2.txt outFile3.txt
*/

int main(int argc, char *argv[]){
	ifstream inFile(argv[1]);
	ofstream outFile1(argv[2]), outFile2(argv[3]), outFile3(argv[4]);

	int numRows, numCols, minVal, maxVal, label;
	inFile >> numRows >> numCols >> minVal >> maxVal >> label;
	
	int K;
	cout << "Choose value for K: ";
	cin >> K;
	
	kCurvature KC(numRows, numCols, minVal, maxVal, K, label);
			
	KC.initialization(inFile, argv[1]);
	outFile3 << "After Initialization" << endl;
	KC.printPtAry(outFile3);
	
	outFile3 << "After Corner Detection: " << endl << endl;
	KC.cornerDetection(outFile3);
	
	outFile3 << "After Calculating the Curvature" << endl;
	KC.printPtAry(outFile3);
	
	KC.computeLocalMaxima();
	outFile3 << "After Computing the Local Maxima" << endl;
	KC.printPtAry(outFile3);

	KC.markCorner();
	outFile3 << "After Marking the Corners" << endl;
	KC.printPtAry(outFile3);

	KC.printBoundary(outFile1);
	KC.display(outFile2);
	
	inFile.close();
	outFile1.close();
	outFile2.close();
	outFile3.close();
}
