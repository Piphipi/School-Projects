import java.io.*;
import java.util.Random;
import java.math.BigInteger;
//import java.util.Scanner;

public class GenerateRSA {
	public static void main(String[] args)
	{
		int numBits = 6144;
		BigInteger P = getPrime(numBits);   // get P
		BigInteger Q = getPrime(numBits);   // get Q
		BigInteger N = P.multiply(Q);		// get N
		BigInteger Z = getZ(P,Q);			// get Z
		BigInteger e = getE(Z);				// get e
		BigInteger d = e.modInverse(Z);		// get d
		Random rnd = new Random();
		BigInteger M = new BigInteger(numBits, rnd);  // get random message
		BigInteger C = encrypt(M,N,e);
		BigInteger MPrime = decrypt(C,N,d);
		boolean isValid = M.equals(MPrime);
		if (isValid){ // if everything works print it
			System.out.println("P = " + P);
			System.out.println("Q = " + Q);
			System.out.println("N = " + N);
			System.out.println("Z = " + Z);
			System.out.println("e = " + e);
			System.out.println("d = " + d);
			System.out.println("M = " + M);
			System.out.println("C = " + C);
			System.out.println("M' = " + MPrime);
		}
		else { // otherwise print an error
			System.out.println("Error in using RSA, please try again or modify the code.");
		}
	}

	public static BigInteger getPrime(int bits) {
		Random rnd = new Random();
		BigInteger prime = BigInteger.probablePrime(bits, rnd); // get Prime of bit length bits
		return prime;
	}
	
	public static BigInteger getZ(BigInteger A, BigInteger B) {
		BigInteger Z = (A.subtract(BigInteger.ONE)).multiply(B.subtract(BigInteger.ONE));  // Z = (P - 1) * (Q - 1)
		return Z;
	}
	
	public static BigInteger getE(BigInteger Z) {
		Random rnd = new Random();
		int bits = Z.bitLength() - 1;
		BigInteger e = BigInteger.probablePrime(bits, rnd);
		do {
			e = BigInteger.probablePrime(bits, rnd);
			while(e.min(Z).equals(Z)) {
				e = BigInteger.probablePrime(bits, rnd);
			}
		} while(!gcd(e, Z).equals(BigInteger.ONE));
		return e;
	}

	private static BigInteger gcd(BigInteger a, BigInteger b) {
		if (b.equals(BigInteger.ZERO)) {
			return a;
		}
		else {
			return gcd(b, a.mod(b));
		}
	}

	public static BigInteger encrypt(BigInteger m, BigInteger n, BigInteger e) {
		return m.modPow(e, n);
	}
	
	public static BigInteger decrypt(BigInteger c, BigInteger n, BigInteger d) {
		return c.modPow(d, n);
	}
}
