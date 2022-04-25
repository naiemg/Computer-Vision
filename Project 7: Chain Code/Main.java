import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

public class Main {
	public static void main(String[] args) throws IOException {
		if (args.length != 2) {
			System.out.println("Invalid number of arguments.");
			System.exit(0);
		}

		Scanner labelFile = new Scanner(new BufferedReader(new FileReader(args[0])));
		Scanner propFile = new Scanner(new BufferedReader(new FileReader(args[1])));

		Image img = new Image(labelFile.nextInt(), labelFile.nextInt(), labelFile.nextInt(), labelFile.nextInt());

		int garbage = 0;
		while (garbage < 4) {
			propFile.nextInt();
			garbage++;
		}

		CCproperty CC = new CCproperty(propFile.nextInt());
		chainCode cd = new chainCode();
		img.loadImg(labelFile, img.imageAry);

		String chainCodeFileName = args[0] + "_chainCode.txt";
		String boundaryFileName = args[1] + "_Boundary.txt";

		BufferedWriter chainCodeFile = new BufferedWriter(new FileWriter(chainCodeFileName));
		BufferedWriter boundaryFile = new BufferedWriter(new FileWriter(boundaryFileName));

		img.printHeader(chainCodeFile);
		chainCodeFile.write(CC.numCC + "\n");

		int ct = 0;

		while (ct < CC.numCC) {
			ct++;
			CC.label = propFile.nextInt();
			CC.numPixels = propFile.nextInt();
			CC.minRow = propFile.nextInt();
			CC.minCol = propFile.nextInt();
			CC.maxRow = propFile.nextInt();
			CC.maxCol = propFile.nextInt();

			CC.clearCCAry(img.ccAry);
			CC.loadCCAry(CC.label, img.imageAry, img.ccAry);
			cd.getChainCode(CC, img.ccAry, chainCodeFile);
			chainCodeFile.newLine();
		}

		chainCodeFile.close();
		Scanner ccFile = new Scanner(new BufferedReader(new FileReader(chainCodeFileName)));

		garbage = 0;
		while (garbage < 4) {
			ccFile.nextInt();
			garbage++;
		}

		cd.constructBoundary(img.boundaryAry, ccFile);
		img.ary2File(img.boundaryAry, boundaryFile);

		labelFile.close();
		propFile.close();
		chainCodeFile.close();
		boundaryFile.close();
		ccFile.close();
	}
}
