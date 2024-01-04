import java.util.Scanner;
import java.io.*;
public class Project0
{
    public static void main(String[] args){
        Scanner input = new Scanner(System.in);
        try{
            Scanner inFile1 = new Scanner(new FileReader(args[0]));
            Scanner inFile2 = new Scanner(new FileReader(args[1]));
            FileWriter outFile1 = new FileWriter(args[2]);
            FileWriter outFile2 = new FileWriter(args[3]);
            processInts(inFile1,outFile1);
            processWords(inFile2,outFile2);
            inFile1.close();
            inFile2.close();
            outFile1.close();
            outFile2.close();
        }
        catch(FileNotFoundException ex) {
            System.out.println("Unable to open a file");
        }
        catch(IOException ex) {
            System.out.println("Error reading a file");
        }
    }

    static void processInts(Scanner inFile1, FileWriter outFile1){
        try{
            BufferedWriter write1 = new BufferedWriter(outFile1);
            while(inFile1.hasNextInt()){
                for(int i = 0; i < 5; i++){
                    if (inFile1.hasNextInt()){
                        write1.write(inFile1.nextInt() + " ");
                    }
                    else{
                        break;
                    }
                }
                write1.newLine();
            }
            write1.close();
        }
        catch(FileNotFoundException ex) {
            System.out.println("Unable to open a file");
        }
        catch(IOException ex) {
            System.out.println("Error reading a file");
        }
    }

    static void processWords(Scanner inFile2, FileWriter outFile2){
        try{
            BufferedWriter write2 = new BufferedWriter(outFile2);
            while(inFile2.hasNext()){
                for(int i = 0; i < 5; i++){
                    if (inFile2.hasNext()){
                        write2.write(inFile2.next() + " ");
                    }
                    else{
                        break;
                    }
                }
                write2.newLine();
            }
            write2.close();
        }
        catch(FileNotFoundException ex) {
            System.out.println("Unable to open a file");
        }
        catch(IOException ex) {
            System.out.println("Error reading a file");
        }
    }
}