import java.util.Scanner;
import java.io.*;
public class Project3
{
    public static void main(String[] args){
        Scanner input = new Scanner(System.in);
        int bucketSize;
        try{
            Scanner inFile = new Scanner(new FileReader(args[0]));
            System.out.println("please enter bucket size");
            bucketSize = input.nextInt();
            FileWriter outFile1 = new FileWriter(args[1]);
            FileWriter outFile2 = new FileWriter(args[2]);
            hashTable h = new hashTable(bucketSize);
            h.createHashTable();
            h.informationProcessing(inFile, outFile2);
            h.printHashTable(outFile1);
            inFile.close();
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
}
class listNode{
    String firstName;
    String lastName;
    listNode next;
    public listNode(String first, String last){
        this.firstName = first;
        this.lastName = last;
        this.next = null;
    }

    public static void printNode (listNode node, FileWriter outFile){
        try{
            BufferedWriter writer = new BufferedWriter(outFile);
            if(node.next == null){
                writer.write("(" + node.firstName + ", " + node.lastName + ", NULL) --> NULL");
            }
            else{
                writer.write("(" + node.firstName + ", " + node.lastName + ", " + node.next.firstName + ") --> ");
            }
            writer.flush();
        }
        catch(FileNotFoundException ex) {
            System.out.println("Unable to open a file");
        }
        catch(IOException ex) {
            System.out.println("Error reading a file");
        }
    }
}
class hashTable{
    char op;
    static int bucketSize;
    static listNode hashTable[];
    public hashTable(int size){
        this.bucketSize = size;
    }
    public static void createHashTable(){
        hashTable = new listNode[bucketSize];
        for(int i = 0; i < bucketSize; i++){
            listNode dummy = new listNode("dummyfirst", "dummylast");
            hashTable[i] = dummy;
        }
    }
    public static int doit(String lastName){
        int val = 1;
        for (int i = 0; i < lastName.length(); i++){
            val = val * 32 + (int) lastName.charAt(i);
        }
        return (Integer.remainderUnsigned(val, bucketSize));
    }
    public static void informationProcessing(Scanner inFile, FileWriter outFile2){
        try{
            BufferedWriter writer = new BufferedWriter(outFile2);
            writer.write("Enter informationProcessing method \n");
            writer.flush();
            String op;
            String firstName;
            String lastName;
            while(inFile.hasNext()){
                op = inFile.next();
                firstName = inFile.next();
                lastName = inFile.next();
                writer.write("The input triplet is: " + op + " " + firstName + " " + lastName + "\n");
                int index = doit(firstName);
                writer.write("The index of " + firstName + " is " + index + "\n");
                writer.flush();
                printList(index, outFile2);
                if (op.equals("+")){
                    hashInsert(index, firstName, lastName, outFile2);
                }
                else if (op.equals("-")){
                    hashDelete(index, firstName, lastName, outFile2);
                }
                else if (op.equals("?")){
                    hashRetrieval(index, firstName, lastName, outFile2);
                }
                else{
                    writer.write("op is an unrecognized operation \n");
                }
                writer.flush();
            }
        }
        catch(FileNotFoundException ex) {
            System.out.println("Unable to open a file");
        }
        catch(IOException ex) {
            System.out.println("Error reading a file");
        }
    }
    public static listNode findSpot(int index, String firstName, String lastName){
        listNode Spot = hashTable[index];
        while(Spot.next != null && (Spot.next.lastName.compareTo(lastName)) < 0){
            Spot = Spot.next;
        }
        while(Spot.next != null && (Spot.next.lastName.compareTo(lastName)) == 0 && (Spot.next.firstName.compareTo(firstName)) < 0){
            Spot = Spot.next;
        }
        return Spot;
    }
    public static void hashInsert(int index, String firstName, String lastName, FileWriter outFile2){
        try{
            BufferedWriter writer = new BufferedWriter(outFile2);
            writer.write("*** enter hashInsert method. Performing hashInsert on " + firstName + ", " + lastName + "\n");
            writer.flush();
            listNode Spot = findSpot(index, firstName, lastName);
            if(Spot.next != null && Spot.next.lastName.equals(lastName) && Spot.next.firstName.equals(firstName)){
                 writer.write("*** Warning, the record is already in the database! \n");
                 writer.flush();
            }
            else{
                listNode newNode = new listNode(firstName, lastName);
                newNode.next = Spot.next;
                Spot.next = newNode;
                writer.write("After hashInsert operation … ");
                writer.flush();
                printList(index, outFile2);
            }
        }
        catch(FileNotFoundException ex) {
            System.out.println("Unable to open a file");
        }
        catch(IOException ex) {
            System.out.println("Error reading a file");
        }
    }
    public static void hashDelete(int index, String firstName, String lastName, FileWriter outFile2){
        try{
            BufferedWriter writer = new BufferedWriter(outFile2);
            writer.write("** Inside hashDelete method. Performing hashDelete on " + firstName + ", " + lastName + "\n");
            writer.flush();
            listNode Spot = findSpot(index, firstName, lastName);
            if(Spot == null){
                 writer.write("*** Warning, the record is *not* in the database! \n");
                 writer.flush();
            }
            else if(Spot.next != null && Spot.next.lastName.equals(lastName) && Spot.next.firstName.equals(firstName)){
                listNode temp = Spot.next;
                Spot.next = temp.next;
                temp.next = null;
                writer.write("After hashDelete operation … ");
                writer.flush();
                printList(index, outFile2);
            }
            else{
                writer.write("Something is wrong with the findSpot method! \n");
                writer.flush();
            }
        }
        catch(FileNotFoundException ex) {
            System.out.println("Unable to open a file");
        }
        catch(IOException ex) {
            System.out.println("Error reading a file");
        }
    }
    public static void hashRetrieval(int index, String firstName, String lastName, FileWriter outFile2){
        try{
            BufferedWriter writer = new BufferedWriter(outFile2);
            writer.write("** Inside hashRetrieval. Performing hashRetrieval on " + firstName + ", " + lastName + "\n");
            writer.flush();
            listNode Spot = findSpot(index, firstName, lastName);
            if(Spot == null){
                 writer.write("*** Warning, the record is *not* in the database! \n");
            }
            else if(Spot.next != null && Spot.next.lastName.equals(lastName) && Spot.next.firstName.equals(firstName)){
                writer.write("Yes, the record is in the database! \n");
            }
            else{
                writer.write("Something is wrong with the findSpot method! \n");
            }
            writer.flush();
        }
        catch(FileNotFoundException ex) {
            System.out.println("Unable to open a file");
        }
        catch(IOException ex) {
            System.out.println("Error reading a file");
        }
    }
    public static void printList(int index, FileWriter outFile){
        try{
            BufferedWriter writer = new BufferedWriter(outFile);
            writer.write("HashTable[" + index + "]: ");
            writer.flush();
            listNode print = hashTable[index];
            while(print.next != null){
                print.printNode(print, outFile);
                print = print.next;
            }
            print.printNode(print, outFile);
            writer.write("\n");
            writer.flush();
        }
        catch(FileNotFoundException ex) {
            System.out.println("Unable to open a file");
        }
        catch(IOException ex) {
            System.out.println("Error reading a file");
        }
    }
    public static void printHashTable(FileWriter outFile){
        for(int i = 0; i < bucketSize; i++){
            printList(i, outFile);
        }
    }
}
