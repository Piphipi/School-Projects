//import java.util.Random;
import java.math.BigInteger;
import java.util.Scanner;

public class HackRSA {
	@SuppressWarnings("resource")
	public static void main (String[] args) {
		Scanner input = new Scanner(System.in);
		System.out.println("please enter C:");
		String c = input.nextLine();
		BigInteger C = new BigInteger(c);  // encrypted meesage is given
		System.out.println("please enter N:");
		String n = input.nextLine();
		BigInteger N = new BigInteger(n);	// given public key
		System.out.println("please enter e:");
		String E = input.nextLine();
		BigInteger e = new BigInteger(E);	// other part of given public key
		System.out.println("please enter P:");
		String p = input.nextLine();
		BigInteger P = new BigInteger(p);	// use online GNFS
		System.out.println("please enter Q:");
		String q = input.nextLine();
		BigInteger Q = new BigInteger(q);	// use online GNFS
		/*BigInteger P = findP(N);			// decompose N to get one factor
		if (P == null) {
			System.out.println("Error! Can't find a valid P");
			return;
		}
		BigInteger Q = findQ(N,P);			// get the other factor
		if (Q == null) {
			System.out.println("Error! Can't find a valid Q");
			return;
		}*/
		BigInteger Z = getZ(P,Q);			// get Z
		BigInteger d = e.modInverse(Z);		// get d
		//System.out.println("d= " + d);
		BigInteger MPrime = decrypt(C,N,d);
		//System.out.print("The decoded message is:" + MPrime);
		System.out.println("Plain Text: " + MPrime + "\n");
		System.out.println("Private Key: " + d);
		
	}

	public static BigInteger findP(BigInteger n) {
		BigInteger primeDecomp = new BigInteger("2");
		System.out.println("entering findP");
		if (n.mod(primeDecomp) == BigInteger.ZERO) { // if primeDecomp is a factor of 2
			System.out.println("exiting findP");
			return primeDecomp;
		}
		primeDecomp.add(BigInteger.ONE); // since its probably NOT, start at 3
		System.out.println("searching " + n.sqrt() + " values for a factor.");
		while (primeDecomp.compareTo(n.sqrt()) < 0) { // while primeDecomp < sqrt(N)
			if (n.mod(primeDecomp) == BigInteger.ZERO) { // if primeDecomp is a factor of N
				System.out.println("exiting findP");
				return primeDecomp;
			}
			primeDecomp = primeDecomp.nextProbablePrime(); //gets next prime rather than search 1 by 1
			System.out.println("prime Decomp is now: " + primeDecomp);
		}
		System.out.println("Error! no factor of N within the sqaure root of N! N is Prime!");
		return null;
	}

	public static BigInteger findQ(BigInteger n, BigInteger p) {
		System.out.println("entering findQ");
		BigInteger[] test = n.divideAndRemainder(p); //get the quotient and modulo
		if (test[1].equals(BigInteger.ZERO)) { // if the modulo is zero it dived perfectly(should always happen)
			System.out.println("exiting findQ");
			return test[0];  //return the quotient
		}
		else { //something weird happened!
			System.out.println("Error! Can't get valid Q!");
		return null;
		}
	}

	public static BigInteger getZ(BigInteger A, BigInteger B) {
		BigInteger Z = (A.subtract(BigInteger.ONE)).multiply(B.subtract(BigInteger.ONE));  // Z = (P - 1) * (Q - 1)
		return Z;
	}

	public static BigInteger decrypt(BigInteger c, BigInteger n, BigInteger d) {
		return c.modPow(d, n);
	}
}