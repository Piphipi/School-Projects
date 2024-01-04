import java.util.Scanner;
import java.io.*;
public class Project1
{
    public static void main (String[] args){
        Scanner input = new Scanner(System.in);
        try{
            Scanner inFile = new Scanner(new FileReader(args[0]));
            FileWriter stackOutFile = new FileWriter(args[1]);
            FileWriter queueOutFile = new FileWriter(args[2]);
            FileWriter listOutFile = new FileWriter(args[3]);
            LLStack.buildStack(inFile,stackOutFile);
            inFile.close();
            inFile = new Scanner(new FileReader(args[0]));
            LLQueue.buildQueue(inFile,queueOutFile);
            inFile.close();
            inFile = new Scanner(new FileReader(args[0]));
            LList.buildList(inFile,listOutFile);
            inFile.close();
            stackOutFile.close();
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
    int data;
    listNode next;
    public listNode(int input){
        this.data = input;
        this.next = null;
    }

    public static void printNode (listNode node, FileWriter outFile){
        try{
            BufferedWriter writer = new BufferedWriter(outFile);
            if(node.next == null){
                writer.write("(" + node.data + ", NULL) --> ");
            }
            else{
                writer.write("(" + node.data + ", " + node.next.data + ") --> ");
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
class LLStack{
    static listNode top;
    public LLStack(){
        listNode dummy = new listNode(-9999);
        top = dummy;
    }

    public static void buildStack(Scanner inFile, FileWriter stackOutFile){
        try{
            BufferedWriter writer = new BufferedWriter(stackOutFile);
            writer.write("** Inside buildStack ** \n");
            writer.flush();
            LLStack S = new LLStack();
            int data;
            String op;
            while(inFile.hasNext()){
                op = inFile.next();
                data = inFile.nextInt();
                writer.write("The input pair are: " + op + " " + data + "\n");
                if (op.equals("+")){
                    listNode newNode = new listNode(data);
                    push(newNode);
                }
                else if (op.equals("-")){
                    if (isEmpty()){
                        writer.write("Stack is empty \n");
                    }
                    else{
                        listNode output = pop();
                        writer.write("the deleted top of stack is: ");
                        writer.write(output.data + "\n");
                    }
                }
                writer.flush();
                S.print(stackOutFile);
            }
            writer.close();
        }
        catch(FileNotFoundException ex) {
            System.out.println("Unable to open a file");
        }
        catch(IOException ex) {
            System.out.println("Error reading a file");
        }
    }

    public static void push(listNode newNode){
        listNode newest = top;
        while(newest.next != null){
            newest = newest.next;
        }
        newest.next = newNode;
    }

    public static listNode pop(){
        if(isEmpty()){
            return null;
        }
        else{
            listNode del = top.next;
            listNode preDel = top;
            while(del.next != null){
                preDel = del;
                del = del.next;
            }
            preDel.next = null;
            return del;
        }
    }

    public static boolean isEmpty(){
        if(top.next == null){
            return true;
        }
        else{
            return false;
        }
    }

    public static void print(FileWriter stackOutFile){
        try{
            BufferedWriter writer = new BufferedWriter(stackOutFile);
            writer.write("Top --> ");
            writer.flush();
            if (isEmpty()){
                top.printNode(top, stackOutFile);
            }
            else{
                listNode newest = top;
                while(newest.next != null){
                    newest.printNode(newest, stackOutFile);
                    newest = newest.next;
                }
                newest.printNode(newest, stackOutFile);
            }
            writer.write("NULL \n");
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
class LLQueue{
    static listNode head;
    static listNode tail = new listNode(0);
    public LLQueue(){
        listNode dummy = new listNode(-9999);
        head = dummy;
        tail.next = dummy;
    }

    public static void buildQueue(Scanner inFile, FileWriter queueOutFile){
        try{
            BufferedWriter writer = new BufferedWriter(queueOutFile);
            writer.write("** Inside buildQueue ** \n");
            writer.flush();
            LLQueue Q = new LLQueue();
            int data;
            String op;
            while(inFile.hasNext()){
                op = inFile.next();
                data = inFile.nextInt();
                writer.write("The input pair are: " + op + " " + data + "\n");
                if (op.equals("+")){
                    listNode newNode = new listNode(data);
                    insert(newNode);
                }
                else if (op.equals("-")){
                    if (isEmpty()){
                        writer.write("Queue is empty \n");
                    }
                    else{
                        listNode output = delete();
                        writer.write("the deleted head of Q is: ");
                        writer.write(output.data + "\n");
                    }
                }
                writer.flush();
                Q.print(queueOutFile);
            }
            writer.close();
        }
        catch(FileNotFoundException ex) {
            System.out.println("Unable to open a file");
        }
        catch(IOException ex) {
            System.out.println("Error reading a file");
        }
    }

    public static void insert(listNode newNode){
        (tail.next).next = newNode;
        tail.next = newNode;
    }

    public static listNode delete(){
        if(isEmpty()){
            return null;
        }
        else{
            listNode ret;
            ret = head.next;
            head.next = (head.next).next;
            return ret;
        }
    }

    public static boolean isEmpty(){
        if(tail.data == -9999){
            return true;
        }
        else{
            return false;
        }
    }

    public static void print(FileWriter queueOutFile){
        try{
            BufferedWriter writer = new BufferedWriter(queueOutFile);
            writer.write("Front --> ");
            writer.flush();
            if (isEmpty()){
                head.printNode(head, queueOutFile);
            }
            else{
                listNode newest = head;
                while(newest.next != null){
                    newest.printNode(newest, queueOutFile);
                    newest = newest.next;
                }
                newest.printNode(newest, queueOutFile);
            }
            writer.write("NULL \n");
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
class LList{
    static listNode head;
    public LList(){
        listNode dummy = new listNode(-9999);
        head = dummy;
    }

    public static void buildList(Scanner inFile, FileWriter listOutFile){
        try{
            BufferedWriter writer = new BufferedWriter(listOutFile);
            writer.write("** Inside build ** \n");
            writer.flush();
            LList list = new LList();
            int data;
            String op;
            while(inFile.hasNext()){
                op = inFile.next();
                data = inFile.nextInt();
                writer.write("The input pair are: " + op + " " + data + "\n");
                writer.flush();
                if (op.equals("+")){
                    listNode newNode = new listNode(data);
                    insert(newNode);
                }
                else if (op.equals("-")){
                    if (isEmpty()){
                        writer.write("List is empty! \n");
                    }
                    else{
                        listNode output = deleteOneNode(data);
                        if(output == null){
                            writer.write("data is not in the list\n");
                        }
                        else{
                            writer.write("the data been deleted is: ");
                            writer.write(output.data + "\n");
                        }
                    }
                }
                writer.flush();
                list.print(listOutFile);
            }
        }
        catch(FileNotFoundException ex) {
            System.out.println("Unable to open a file");
        }
        catch(IOException ex) {
            System.out.println("Error reading a file");
        }
    }

    public static void insert(listNode newNode){
        listNode newest = head;
        while(newest.next != null && newest.next.data < newNode.data){
            newest = newest.next;
        }
        newNode.next = newest.next;
        newest.next = newNode;
    }

    public static listNode deleteOneNode(int data){
        if(isEmpty()){ 
            return null;
        }
        else{
            listNode current = head.next;
            listNode prev = head;
            if(current.data == data){
                head.next = current.next;
                return current;
            }
            while(current.next != null){
                prev = current;
                current = current.next;
                if(current.data == data){ //mach found
                    prev.next = current.next;
                    return current;
                }
            }
            return null;
        }
    }

    public static boolean isEmpty(){
        if(head.next == null){
            return true;
        }
        else{
            return false;
        }
    }

    public static void print(FileWriter listOutFile){
        try{
            BufferedWriter writer = new BufferedWriter(listOutFile);
            writer.write("listHead --> ");
            writer.flush();
            if (isEmpty()){
                head.printNode(head, listOutFile);
            }
            else{
                listNode newest = head;
                while(newest.next != null){
                    newest.printNode(newest, listOutFile);
                    newest = newest.next;
                }
                newest.printNode(newest, listOutFile);
            }
            writer.write("NULL \n");
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