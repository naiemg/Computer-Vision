import java.io.BufferedWriter;
import java.io.IOException;
import java.util.Scanner;

public class Morphology {
	int numImgRows;
	int numImgCols;
	int imgMin;
	int imgMax;

	int numStructRows;
	int numStructCols;
	int structMin;
	int structMax;
	int rowOrigin;
	int colOrigin;

	int rowFrameSize;
	int colFrameSize;

	int extraRows;
	int extraCols;

	public int[][] zeroFramedAry;
	public int[][] morphAry;
	public int[][] tempAry;
	public int[][] structAry;

	public Morphology(int numImgRows, int numImgCols, int imgMin, int imgMax, int numStructRows, int numStructCols,
			int structMin, int structMax, int rowOrigin, int colOrigin) {
		this.numImgRows = numImgRows;
		this.numImgCols = numImgCols;
		this.imgMin = imgMin;
		this.imgMax = imgMax;
		this.numStructRows = numStructRows;
		this.numStructCols = numStructCols;
		this.structMin = structMin;
		this.structMax = structMax;
		this.rowOrigin = rowOrigin;
		this.colOrigin = colOrigin;

		this.rowFrameSize = numStructRows / 2;
		this.colFrameSize = numStructCols / 2;

		this.extraRows = rowFrameSize * 2;
		this.extraCols = colFrameSize * 2;

		this.zeroFramedAry = new int[numImgRows + extraRows][numImgCols + extraCols];
		this.morphAry = new int[numImgRows + extraRows][numImgCols + extraCols];
		this.tempAry = new int[numImgRows + extraRows][numImgCols + extraCols];
		this.structAry = new int[numStructRows][numStructCols];
	}

	public void zero2DAry(int[][] ary) {
		for (int i = 0; i < ary.length; i++) {
			for (int j = 0; j < ary[i].length; j++) {
				ary[i][j] = 0;
			}
		}
	}

	public void loadImg(Scanner imgFile, int[][] ary) {
		for (int i = rowOrigin; i < ary.length - rowOrigin; i++) {
			for (int j = colOrigin; j < ary[i].length - colOrigin; j++) {
				ary[i][j] = imgFile.nextInt();
			}
		}
	}

	public void loadStruct(Scanner structFile, int[][] ary) {
		for (int i = 0; i < numStructRows; i++) {
			for (int j = 0; j < numStructCols; j++) {
				ary[i][j] = structFile.nextInt();
			}
		}
	}

	public void aryToFile(int[][] ary, BufferedWriter outFile) throws IOException {
		outFile.write(numImgRows + " " + numImgCols + " " + imgMin + " " + imgMax + "\n");
		for (int i = rowOrigin; i < ary.length - rowOrigin; i++) {
			for (int j = colOrigin; j < ary[i].length - colOrigin; j++) {
				outFile.write(ary[i][j] + " ");
			}
			outFile.newLine();
		}
	}

	public void prettyPrint(int[][] ary, BufferedWriter outFile) throws IOException {
		outFile.write(numImgRows + " " + numImgCols + " " + imgMin + " " + imgMax + "\n");
		for (int i = rowOrigin; i < ary.length - rowOrigin; i++) {
			for (int j = colOrigin; j < ary[i].length - colOrigin; j++) {
				if (ary[i][j] == 0) {
					outFile.write(". ");
				} else {
					outFile.write(ary[i][j] + " ");
				}
			}
			outFile.newLine();
		}
	}

	public void prettyPrintElement(int[][] ary, BufferedWriter outFile) throws IOException {
		outFile.write(numStructRows + " " + numStructCols + " " + structMin + " " + structMax + "\n");
		outFile.write(rowOrigin + " " + colOrigin + "\n");
		for (int i = 0; i < ary.length; i++) {
			for (int j = 0; j < ary[i].length; j++) {
				if (ary[i][j] == 0) {
					outFile.write(". ");
				} else {
					outFile.write(ary[i][j] + " ");
				}
			}
			outFile.newLine();
		}
	}

	public void computeDilation(int[][] inAry, int[][] outAry) {
		int i = rowFrameSize;
		int j;

		while (i < numImgRows + rowFrameSize) {
			j = colFrameSize;
			while (j < numImgCols + colFrameSize) {
				if (inAry[i][j] > 0) {
					dilation(i, j, inAry, outAry);
				}
				j++;
			}
			i++;
		}
	}

	public void dilation(int i, int j, int[][] inAry, int[][] outAry) {
		int imgR = i - rowOrigin;
		int imgC;
		for (int structR = 0; structR < structAry.length; structR++) {
			imgC = j - colOrigin;
			for (int structC = 0; structC < structAry[structR].length; structC++) {
				if (structAry[structR][structC] == 1)
					outAry[imgR][imgC] = structAry[structR][structC];
				imgC++;
			}
			imgR++;
		}
	}

	public void computeErosion(int[][] inAry, int[][] outAry) {
		int i = rowFrameSize;
		int j;

		while (i < numImgRows + rowFrameSize) {
			j = colFrameSize;
			while (j < numImgCols + colFrameSize) {
				if (inAry[i][j] > 0) {
					erosion(i, j, inAry, outAry);
				}
				j++;
			}
			i++;
		}
	}

	public void erosion(int i, int j, int[][] inAry, int[][] outAry) {
		int imgR = i - rowOrigin;
		int imgC = j;
		for (int structR = 0; structR < structAry.length; structR++) {
			imgC = j - colOrigin;
			for (int structC = 0; structC < structAry[structR].length; structC++) {
				if (structAry[structR][structC] == 1) {
					if (inAry[imgR][imgC] != structAry[structR][structC]) {
						outAry[i][j] = 0;
						return;
					}
				}
				imgC++;
			}
			imgR++;
		}
		outAry[i][j] = 1;
	}

	public void computeOpening(int[][] zeroFramedAry, int[][] morphAry, int[][] tempAry) {
		computeErosion(zeroFramedAry, tempAry);
		computeDilation(tempAry, morphAry);
	}

	public void computeClosing(int[][] zeroFramedAry, int[][] morphAry, int[][] tempAry) {
		computeDilation(zeroFramedAry, tempAry);
		computeErosion(tempAry, morphAry);
	}
}
