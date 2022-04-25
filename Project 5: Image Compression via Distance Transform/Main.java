import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

public class Main {
	public static void main(String[] args) throws IOException {
		if (args.length != 1) {
			System.out.println("Invalid number of arguments.");
			System.exit(0);
		}

		Scanner inFile = new Scanner(new BufferedReader(new FileReader(args[0])));
		BufferedWriter skeletonFile = new BufferedWriter(new FileWriter(args[0] + "_skeleton.txt"));
		BufferedWriter decompressFile = new BufferedWriter(new FileWriter(args[0] + "decompressed.txt"));
		BufferedWriter outFile1 = new BufferedWriter(new FileWriter("outFile1.txt"));
		BufferedWriter outFile2 = new BufferedWriter(new FileWriter("outFile2.txt"));

		ImageProcessing m = new ImageProcessing(inFile.nextInt(), inFile.nextInt(), inFile.nextInt(), inFile.nextInt());

		m.setZero(m.zeroFramedAry);
		m.setZero(m.skeletonAry);

		m.loadImg(inFile, m.zeroFramedAry);

		m.compute8Distance(m.zeroFramedAry, outFile1);
		m.skeletonExtraction(m.zeroFramedAry, m.skeletonAry, skeletonFile, outFile1);

		Scanner skeletonReopen = new Scanner(new BufferedReader(new FileReader(args[0] + "_skeleton.txt")));
		m.skeletonExpansion(m.zeroFramedAry, skeletonReopen, outFile2);

		m.ary2File(m.zeroFramedAry, decompressFile);

		inFile.close();
		skeletonFile.close();
		decompressFile.close();
		outFile1.close();
		outFile2.close();
		skeletonReopen.close();
	}
}
