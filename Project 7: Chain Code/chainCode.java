import java.io.BufferedWriter;
import java.io.IOException;
import java.util.Scanner;

public class chainCode {
	Point[] neighborCoord;
	final int[] zeroTable = { 6, 0, 0, 2, 2, 4, 4, 6 };
	final int[][] neighborDist = { { 0, 1 }, { -1, 1 }, { -1, 0 }, { -1, -1 }, { 0, -1 }, { 1, -1 }, { 1, 0 },
			{ 1, 1 } };

	Point startP;
	Point currentP;
	Point nextP;

	int lastQ;
	int nextQ;
	int PchainDir;

	public chainCode() {
		neighborCoord = new Point[8];
	}

	public void findStart(CCproperty CC, int[][] ccAry, BufferedWriter outFile) throws IOException {
		for (int i = 1; i < ccAry.length - 1; i++) {
			for (int j = 1; j < ccAry[i].length - 1; j++) {
				if (ccAry[i][j] == CC.label) {
					outFile.write(CC.label + " " + i + " " + j + "\n");
					this.startP = new Point(i, j);
					this.currentP = this.startP;
					this.lastQ = 4;
					return;
				}
			}
		}
	}

	public void loadNeighborsCoord(Point currentP) {
		for (int neighbor = 0; neighbor < neighborDist.length; neighbor++) {
			this.neighborCoord[neighbor] = new Point(currentP.row + neighborDist[neighbor][0],
					currentP.col + neighborDist[neighbor][1]);
		}
	}

	public int findNextP(Point currentP, int nextQ, Point nextP, int[][] ccAry) {
		loadNeighborsCoord(currentP);
		int chainDir;

		int r = neighborCoord[this.nextQ].row;
		int c = neighborCoord[this.nextQ].col;

		while (ccAry[r][c] == 0) {
			this.nextQ = (this.nextQ + 1) % 8;
			r = neighborCoord[this.nextQ].row;
			c = neighborCoord[this.nextQ].col;
		}

		chainDir = this.nextQ;
		this.nextP = neighborCoord[chainDir];
		return chainDir;
	}

	public boolean isEqual(Point a, Point b) {
		if (a.row == b.row && a.col == b.col)
			return true;
		return false;
	}

	public void getChainCode(CCproperty CC, int[][] ccAry, BufferedWriter outFile) throws IOException {
		findStart(CC, ccAry, outFile);

		int ct = 0;
		while (!isEqual(this.currentP, this.startP) || ct == 0) {
			this.nextQ = (this.lastQ + 1) % 8;
			this.PchainDir = findNextP(this.currentP, this.nextQ, this.nextP, ccAry);
			ccAry[this.currentP.row][this.currentP.col] *= -1;
			outFile.write(this.PchainDir + " ");

			int index = this.PchainDir - 1;
			if (index < 0) {
				index = index + 8;
			}
			this.lastQ = zeroTable[index];
			this.currentP = this.nextP;
			ct++;
		}
	}

	public void constructBoundary(int[][] ary, Scanner inFile) {
		Scanner scanner;
		int currCC = 0;
		int numCC = inFile.nextInt();
		int value, r, c;

		inFile.nextLine();
		while (currCC < numCC) {
			while (inFile.hasNextLine()) {
				scanner = new Scanner(inFile.nextLine());
				value = scanner.nextInt();
				r = scanner.nextInt();
				c = scanner.nextInt();
				ary[r][c] = value;
				scanner = new Scanner(inFile.nextLine());
				while (scanner.hasNextInt()) {
					int neighbor = scanner.nextInt();
					r += neighborDist[neighbor][0];
					c += neighborDist[neighbor][1];
					ary[r][c] = value;
				}
			}
			currCC++;
		}
	}
}
