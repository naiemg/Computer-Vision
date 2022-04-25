public class CCproperty {
	public int numCC;
	public int label;
	public int numPixels;
	public int minRow;
	public int minCol;
	public int maxRow;
	public int maxCol;

	public CCproperty(int numCC) {
		this.numCC = numCC;
	}

	public void clearCCAry(int[][] ary) {
		for (int i = 0; i < ary.length; i++) {
			for (int j = 0; j < ary[i].length; j++) {
				ary[i][j] = 0;
			}
		}
	}

	public void loadCCAry(int label, int[][] imageAry, int[][] CCAry) {
		for (int i = 1; i < imageAry.length - 1; i++) {
			for (int j = 1; j < imageAry[i].length - 1; j++) {
				if (imageAry[i][j] == label) {
					CCAry[i][j] = imageAry[i][j];
				} else {
					CCAry[i][j] = 0;
				}
			}
		}
	}
}
