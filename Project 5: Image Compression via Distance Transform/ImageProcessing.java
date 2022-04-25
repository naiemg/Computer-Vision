import java.io.BufferedWriter;
import java.io.IOException;
import java.util.Scanner;

public class ImageProcessing {
	int numRows;
	int numCols;
	int minVal;
	int maxVal;
	int newMinVal;
	int newMaxVal;

	public int[][] zeroFramedAry;
	public int[][] skeletonAry;

	public ImageProcessing(int numImgRows, int numImgCols, int imgMin, int imgMax) {
		this.numRows = numImgRows;
		this.numCols = numImgCols;
		this.minVal = imgMin;
		this.maxVal = imgMax;

		this.newMinVal = imgMin;
		this.newMaxVal = imgMax;

		this.zeroFramedAry = new int[numImgRows + 2][numImgCols + 2];
		this.skeletonAry = new int[numImgRows + 2][numImgCols + 2];
	}

	public void setZero(int[][] ary) {
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

	public void compute8Distance(int[][] ary, BufferedWriter outFile) throws IOException {
		outFile.write("1st Pass Distance Transform:\n");
		firstPass_8Distance(ary);
		prettyPrint(ary, outFile);

		outFile.write("\n2nd Pass Distance Transform:\n");
		secondPass_8Distance(ary);
		prettyPrint(ary, outFile);
	}

	public int findShortest(int[] ary) {
		int shortest = 99999;
		for (int i = 0; i < ary.length; i++) {
			if (ary[i] < shortest) {
				shortest = ary[i];
			}
		}
		return shortest;
	}

	public void firstPass_8Distance(int[][] ary) {
		for (int i = 1; i < ary.length - 1; i++) {
			for (int j = 1; j < ary[i].length - 1; j++) {
				int[] distances = new int[4];
				if (ary[i][j] > 0) {
					distances[0] = ary[i - 1][j - 1] + 1;
					distances[1] = ary[i - 1][j] + 1;
					distances[2] = ary[i - 1][j + 1] + 1;
					distances[3] = ary[i][j - 1] + 1;

					ary[i][j] = findShortest(distances);
				}
			}
		}
	}

	public void secondPass_8Distance(int[][] ary) {
		for (int i = ary.length - 1; i > 0; i--) {
			for (int j = ary[i].length - 1; j > 0; j--) {
				int[] distances = new int[5];
				if (ary[i][j] > 0) {
					distances[0] = ary[i][j + 1] + 1;
					distances[1] = ary[i + 1][j - 1] + 1;
					distances[2] = ary[i + 1][j] + 1;
					distances[3] = ary[i + 1][j + 1] + 1;
					distances[4] = ary[i][j];

					ary[i][j] = findShortest(distances);

					if (ary[i][j] < newMinVal)
						newMinVal = ary[i][j];
					if (ary[i][j] > newMaxVal)
						newMaxVal = ary[i][j];
				}
			}
		}
	}

	public void skeletonExtraction(int[][] zeroFramedAry, int[][] skeletonAry, BufferedWriter skeletonFile,
			BufferedWriter outFile) throws IOException {
		outFile.write("\nLocal Maxima:\n");
		computeLocalMaxima(zeroFramedAry, skeletonAry);
		prettyPrint(skeletonAry, outFile);

		extractLocalMaxima(skeletonAry, skeletonFile);
		skeletonFile.close();
	}

	public void computeLocalMaxima(int[][] zeroFramedAry, int[][] skeletonAry) {
		for (int i = 1; i < zeroFramedAry.length - 1; i++) {
			for (int j = 1; j < zeroFramedAry[i].length - 1; j++) {
				skeletonAry[i][j] = setLocalMaxima(i, j);
			}
		}
	}

	public int setLocalMaxima(int i, int j) {
		int center = zeroFramedAry[i][j];
		for (int r = i - 1; r <= i + 1; r++) {
			for (int c = j - 1; c <= j + 1; c++) {
				if (center < zeroFramedAry[r][c]) {
					return 0;
				}
			}
		}
		return center;
	}

	public void extractLocalMaxima(int[][] ary, BufferedWriter outFile) throws IOException {
		outFile.write(numRows + " " + numCols + " " + newMinVal + " " + newMaxVal + "\n");
		for (int i = 1; i < ary.length - 1; i++) {
			for (int j = 1; j < ary[i].length - 1; j++) {
				if (ary[i][j] > 0) {
					outFile.write(i + " " + j + " " + ary[i][j] + "\n");
				}
			}
		}
	}

	public void skeletonExpansion(int[][] ary, Scanner inFile, BufferedWriter outFile) throws IOException {
		setZero(ary);

		for (int i = 0; i < 4; i++) {
			inFile.nextInt();
		}

		while (inFile.hasNextInt()) {
			ary[inFile.nextInt()][inFile.nextInt()] = inFile.nextInt();
		}

		outFile.write("1st Pass Expansion:\n");
		firstPassExpansion(ary);
		prettyPrint(ary, outFile);

		outFile.write("\n2nd Pass Expansion:\n");
		secondPassExpansion(ary);
		prettyPrint(ary, outFile);
	}

	public int getMaxNeighbor(int i, int j, int n) {
		int max = -99999;
		for (int r = i - 1; r <= i + 1; r++) {
			for (int c = j - 1; c <= j + 1; c++) {
				if (max < zeroFramedAry[r][c] - n) {
					max = zeroFramedAry[r][c] - n;
				}
			}
		}
		return max;
	}

	public void firstPassExpansion(int[][] ary) {
		for (int i = 1; i < ary.length - 1; i++) {
			for (int j = 1; j < ary[i].length - 1; j++) {
				if (ary[i][j] == 0) {
					int max = getMaxNeighbor(i, j, 1);

					if (ary[i][j] < max) {
						ary[i][j] = max;
					}
				}
			}
		}
	}

	public void secondPassExpansion(int[][] ary) {
		for (int i = ary.length - 2; i > 0; i--) {
			for (int j = ary[i].length - 2; j > 0; j--) {
				int max = getMaxNeighbor(i, j, 0);

				if (ary[i][j] < max)
					ary[i][j] = max - 1;

				if (ary[i][j] < newMinVal)
					newMinVal = ary[i][j];
				if (ary[i][j] > newMaxVal)
					newMaxVal = ary[i][j];
			}
		}
	}

	public void ary2File(int[][] ary, BufferedWriter outFile) throws IOException {
		outFile.write(numRows + " " + numCols + " " + 0 + " " + 1 + "\n");
		for (int i = 1; i < ary.length - 1; i++) {
			for (int j = 1; j < ary[i].length - 1; j++) {
				if (ary[i][j] >= 1) {
					outFile.write(1 + " ");
				} else {
					outFile.write(0 + " ");
				}
			}
			outFile.write("\n");
		}
	}

	public void prettyPrint(int[][] ary, BufferedWriter outFile) throws IOException {
		outFile.write(numRows + " " + numCols + " " + newMinVal + " " + newMaxVal + "\n");
		for (int i = 1; i < ary.length - 1; i++) {
			for (int j = 1; j < ary[i].length - 1; j++) {
				if (ary[i][j] == 0) {
					outFile.write(". ");
				} else {
					outFile.write(ary[i][j] + " ");
				}
			}
			outFile.newLine();
		}
	}
}
