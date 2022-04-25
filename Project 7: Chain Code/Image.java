import java.io.BufferedWriter;
import java.io.IOException;
import java.util.Scanner;

public class Image {
	int numRows;
	int numCols;
	int minVal;
	int maxVal;

	public int[][] imageAry;
	public int[][] boundaryAry;
	public int[][] ccAry;

	public Image(int numImgRows, int numImgCols, int imgMin, int imgMax) {
		this.numRows = numImgRows;
		this.numCols = numImgCols;
		this.minVal = imgMin;
		this.maxVal = imgMax;

		this.imageAry = new int[numImgRows + 2][numImgCols + 2];
		this.boundaryAry = new int[numImgRows + 2][numImgCols + 2];
		this.ccAry = new int[numImgRows + 2][numImgCols + 2];
	}

	public void zeroFramed(int[][] ary) {
		for (int i = 0; i < ary.length; i++) {
			for (int j = 0; j < ary[i].length; j++) {
				ary[i][j] = 0;
			}
		}
	}

	public void loadImg(Scanner imgFile, int[][] ary) {
		for (int i = 1; i < ary.length - 1; i++) {
			for (int j = 1; j < ary[i].length - 1; j++) {
				ary[i][j] = imgFile.nextInt();
			}
		}
	}

	public void ary2File(int[][] ary, BufferedWriter outFile) throws IOException {
		outFile.write(numRows + " " + numCols + " " + minVal + " " + maxVal + "\n");
		for (int i = 1; i < ary.length - 1; i++) {
			for (int j = 1; j < ary[i].length - 1; j++) {
				if (ary[i][j] >= 1) {
					outFile.write(ary[i][j] + " ");
				} else {
					outFile.write(0 + " ");
				}
			}
			outFile.write("\n");
		}
	}

	public void printHeader(BufferedWriter outFile) throws IOException {
		outFile.write(numRows + " " + numCols + " " + minVal + " " + maxVal + "\n");
	}
}
