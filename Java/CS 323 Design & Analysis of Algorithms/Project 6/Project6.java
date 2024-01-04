import java.util.Scanner;
import java.io.*;
public class Project6
{
    public static void main(String[] args){
        Scanner input = new Scanner(System.in);
        int bucketSize;
        try{
            Scanner inFile = new Scanner(new FileReader(args[0]));
            FileWriter outFile1 = new FileWriter(args[1]);
            FileWriter outFile2 = new FileWriter(args[2]);
            DijktraSSS D = new DijktraSSS(inFile);
            D.loadCostMatrix(inFile);
            D.sourceNode = 1;
            int marked = 0;
            while(D.sourceNode <= D.numNodes){
                D.setBestCostAry();
                D.setFatherAry();
                D.setMarkedAry();
                while(marked != D.numNodes){
                    D.minNode = D.findMinNode();
                    D.markedAry[D.minNode] = 1;
                    marked++;
                    D.debugPrint(outFile2);
                    D.currentNode = 1;
                    while(D.currentNode <= D.numNodes){
                        if(D.markedAry[D.currentNode] == 0){
                            D.newCost = D.computeCost();
                            if(D.newCost < D.bestCostAry[D.currentNode]){
                                D.bestCostAry[D.currentNode] = D.newCost;
                                D.fatherAry[D.currentNode] = D.minNode;
                                D.debugPrint(outFile2);
                            }
                        }
                        D.currentNode++;
                    }
                }
                D.currentNode = 1;
                BufferedWriter writer = new BufferedWriter(outFile1);
                writer.write("Source node = " + D.sourceNode + "\n \n");
                writer.flush();
                while(D.currentNode <= D.numNodes){
                    D.printShortestPath(D.currentNode, D.sourceNode, outFile1);
                    D.currentNode++;
                }
                writer.write("===============================\n");
                writer.flush();
                D.sourceNode++;
                marked = 0;
            }
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
class DijktraSSS{
    int numNodes;
    int sourceNode;
    int minNode;
    int currentNode;
    int newCost;
    int [][] costMatrix;
    int [] fatherAry;
    int [] markedAry;
    int [] bestCostAry;
    public DijktraSSS(Scanner inFile){
        if(inFile.hasNextInt()){
            numNodes = inFile.nextInt();
            costMatrix = new int[numNodes + 1][numNodes + 1];
            for(int i = 1; i < costMatrix.length; i++){
                for(int j = 1; j < costMatrix[i].length; j++){
                    if(i == j){
                        costMatrix[i][j] = 0;
                    }
                    else{
                        costMatrix[i][j] = 9999;
                    }
                }
            }
            fatherAry = new int [numNodes + 1];
            markedAry = new int [numNodes + 1];
            for(int i = 1; i < markedAry.length; i++){
                markedAry[i] = 0;
            }
            bestCostAry = new int [numNodes + 1];
            for(int i = 1; i < bestCostAry.length; i++){
                bestCostAry[i] = 9999;
            }
        }
    }

    public void loadCostMatrix(Scanner inFile){
        int i;
        int j;
        while(inFile.hasNextInt()){
            i = inFile.nextInt();
            j = inFile.nextInt();
            costMatrix [i][j] = inFile.nextInt();
        }
    }

    public void setBestCostAry(){
        for(int i = 1; i < bestCostAry.length; i++){
            bestCostAry[i] = costMatrix[sourceNode][i];
        }
    }

    public void setFatherAry(){
        for(int i = 1; i < fatherAry.length; i++){
            fatherAry[i] = sourceNode;
        }
    }

    public void setMarkedAry(){
        for(int i = 1; i < markedAry.length; i++){
            if (i != sourceNode){
                markedAry[i] = 0;
            }
            else{
                markedAry[i] = 1;
            }
        }
    }

    public int findMinNode(){
        int minCost = 99999;
        int minNode = 0;
        for(int i = 1; i < bestCostAry.length; i++){
            if(markedAry[i] == 0){
                if(bestCostAry[i] < minCost){
                    minCost = bestCostAry[i];
                    minNode = i;
                }
            }
        }
        return minNode;
    }

    public int computeCost(){
        return (bestCostAry[minNode] + costMatrix[minNode][currentNode]);
    }

    public void debugPrint(FileWriter outFile){
        try{
            BufferedWriter writer = new BufferedWriter(outFile);
            writer.write("The source Node is: " + sourceNode + "\n");
            writer.write("The father array is: ");
            for(int i = 1; i < fatherAry.length; i++){
                writer.write("[" + fatherAry[i] + "], ");
            }
            writer.write("\nThe best cost array is: ");
            for(int i = 1; i < bestCostAry.length; i++){
                writer.write("[" + bestCostAry[i] + "], ");
            }
            writer.write("\nThe marked array is:");
            for(int i = 1; i < markedAry.length; i++){
                writer.write("[" + markedAry[i] + "], ");
            }
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

    public void printShortestPath(int current, int source, FileWriter outFile){
        try{
            BufferedWriter writer = new BufferedWriter(outFile);
            writer.write("The path from " + sourceNode + " to " + currentNode + ": " + currentNode);
            int temp = currentNode;
            while(fatherAry[temp] != sourceNode){
                writer.write(" <- " + fatherAry[temp]);
                temp = fatherAry[temp];
            }
            writer.write(" <- " + sourceNode + " : cost = " + bestCostAry[currentNode] + "\n");
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