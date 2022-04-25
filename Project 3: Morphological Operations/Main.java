import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

public class Main {
	public static void main(String[] args) throws IOException {
		if (args.length != 7) {
			System.out.println("Invalid number of arguments.");
			System.exit(0);
		}

		Scanner imgFile = new Scanner(new BufferedReader(new FileReader(args[0])));
		Scanner structFile = new Scanner(new BufferedReader(new FileReader(args[1])));
		BufferedWriter dilateOutFile = new BufferedWriter(new FileWriter(args[2]));
		BufferedWriter erodeOutFile = new BufferedWriter(new FileWriter(args[3]));
		BufferedWriter openingOutFile = new BufferedWriter(new FileWriter(args[4]));
		BufferedWriter closingOutFile = new BufferedWriter(new FileWriter(args[5]));
		BufferedWriter prettyPrintFile = new BufferedWriter(new FileWriter(args[6]));

		Morphology m = new Morphology(imgFile.nextInt(), imgFile.nextInt(), imgFile.nextInt(), imgFile.nextInt(),
				structFile.nextInt(), structFile.nextInt(), structFile.nextInt(), structFile.nextInt(),
				structFile.nextInt(), structFile.nextInt());

		m.zero2DAry(m.zeroFramedAry);

		m.loadImg(imgFile, m.zeroFramedAry);
		prettyPrintFile.write("Original Image: \n");
		m.prettyPrint(m.zeroFramedAry, prettyPrintFile);

		m.zero2DAry(m.structAry);
		m.loadStruct(structFile, m.structAry);
		prettyPrintFile.write("\nStructuring Element: \n");
		m.prettyPrintElement(m.structAry, prettyPrintFile);

		m.zero2DAry(m.morphAry);
		m.computeDilation(m.zeroFramedAry, m.morphAry);
		m.aryToFile(m.morphAry, dilateOutFile);
		prettyPrintFile.write("\nDilation: \n");
		m.prettyPrint(m.morphAry, prettyPrintFile);

		m.zero2DAry(m.morphAry);
		m.computeErosion(m.zeroFramedAry, m.morphAry);
		m.aryToFile(m.morphAry, erodeOutFile);
		prettyPrintFile.write("\nErosion: \n");
		m.prettyPrint(m.morphAry, prettyPrintFile);

		m.zero2DAry(m.morphAry);
		m.computeOpening(m.zeroFramedAry, m.morphAry, m.tempAry);
		m.aryToFile(m.morphAry, openingOutFile);
		prettyPrintFile.write("\nOpening: \n");
		m.prettyPrint(m.morphAry, prettyPrintFile);

		m.zero2DAry(m.morphAry);
		m.computeClosing(m.zeroFramedAry, m.morphAry, m.tempAry);
		m.aryToFile(m.morphAry, closingOutFile);
		prettyPrintFile.write("\nClosing: \n");
		m.prettyPrint(m.morphAry, prettyPrintFile);

		imgFile.close();
		structFile.close();
		dilateOutFile.close();
		erodeOutFile.close();
		openingOutFile.close();
		closingOutFile.close();
		prettyPrintFile.close();
	}
}
