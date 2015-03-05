// PASSWORD CHECKER - ASSESSED COURSEWORK - ALGORITHMS IN JAVA - LT 2015
// NAME: PUT YOUR NAME HERE

import java.io.BufferedReader;
import java.io.FileReader;

class PasswordChecker
{
         
  public static void main(String[] args)
  {

    // Our program expects two inputs, first the path to the file
    // containing the taboo words and then the password. It assumes 
    // that the list is lowercases only and also turns the password 
    // into lowercases.

    String path;
    String pwd;
 
    if(args.length != 2)
    {
      System.out.println("Usage: java PasswordChecker <path to file> <password>");
      return;
    }
    else
    {
      path = args[0];
      pwd = args[1].toLowerCase();
    }

    // Let's start by creating a large enough sorted array and feeding 
    // in the taboo words from the file. The method readFromFile is 
    // provided, but you will need to code up the add method that it 
    // uses as well as the getSize method used below.

    SortedArray a = new SortedArray(300000);
    System.out.print("Reading words from file " + path + "... ");
    a.readFromFile(path);
    System.out.println("done!");
    System.out.println("File contains " + a.getSize() + " words.");


    // Now let's actually sort the sorted array. For this you will need
    // to implement a sorting algorithm that works on arrays of strings
    // rather than on integers. Currently sort does nothing.

    System.out.print("Sorting ... ");
    a.sort();
    System.out.println(" done!");  

    // Use the following line to print the content of the sorted array
    // after the sorting to check whether your sorting algorithm works
    // correctly. In case you are having troubles you may find it useful
    // to also add prints in other places for debugging purposes. That's 
    // totally fine, but please remove them from the final program that
    // you hand in.

    // a.print();

    // Your are also asked to implement two different algorithms for 
    // finding a string in our data structure SortedArray. Let's see 
    // if they work properly.

    String testString = "algorithm";
    System.out.println("Searching for \'" + testString + "\'...");
    if(a.findLinearSearch(testString)) 
      System.out.println("Found with linear search.");
    else 
      System.out.println("Linear search failed.");
    if(a.findBinarySearch(testString)) 
      System.out.println("Found with binary search.");
    else
      System.out.println("Binary search failed.");
 
    // Last but not least, let's check whether the password that you typed in
    // is any good. For this you will have to complete the method checkPassword 
    // that you find below.  
    
    System.out.println("Checking if password \'" + pwd + "\' is good ...");
    System.out.println(checkPassword(a,pwd));

  }

  // Method checkPassword gets a SortedArray and a String and runs some
  // sanity checks on the password. It returns a String message that 
  // reflects whether the program deems the password and good. See the
  // description of the assessed coursework for what common mistakes you
  // should check for.

  public static String checkPassword(SortedArray a, String pwd)
  {      
    // YOUR CODE GOES HERE
    return "Not yet implemented";
  }

}

class SortedArray {

  // The data that we save in a SortedArray is an array of strings arr, 
  // an integer max denoting the maximum number of elements that can 
  // be stored in it, and an integer size that corresponds to the number 
  // of elements currently stored in arr.

  String[] arr;
  int max;
  int size;

  // The constructor "SortedArray" takes one argument max and creates an array
  // arr of length max and sets size to zero

  public SortedArray(int max)
  {
    this.heapSize = heapSize;
    this.size = size;
    this.value = new int[size + 1];
  }

  public void outHeap(String message)
  {
     System.out.println(message);
     for (int i=1; i <= heapSize; i++)
         System.out.print(value[i]; i++)
     System.out.println();
  }
  
  public void outArray(String message)
  {
      System.out.println(message);
      for (int i=1; i <= size; i++)
          System.out.print(value[i] + " ");
      System.out.println();
  }
  
  static boolen isSorted(int a[])
  { 
      boolean sorted = truel 
      int i=0;
      while (i < a.length-1 && sorted)
      {
          sorted = a[i] <= a[i+1];
          i++;
      }
      return sorted;
  }
  
  
  public void randomizeArray(int max)
  {
      for (int i=1; i <= size; i++)
          value[i] = (int)(max * Math.random());
  }
  
  
  // Method "readFromFile" takes path name of file containing list of taboo 
  // words and adds them to array arr. For example "test.txt" may be 
  // <file test.txt starts here>
  // lion
  // cat
  // dog
  // <file test.txt ends here>
  // Then a.readFromFile("test.txt") will create array arr=[lion,cat,dog].

  public void readFromFile(String pathToFile)
  {
    try(BufferedReader br = new BufferedReader(new FileReader(pathToFile))) 
    {
      String line = br.readLine();
      while (line != null) 
      { 
 // This tries to add the string line to arr. For this to work you 
        // will have to complete method add below.
        add(line);
 line = br.readLine();
      }
    }
    catch(Exception e)
    {
      e.printStackTrace();
    }
  }

  // Method "add" takes a String str and adds it at the end of the array arr
  // and updates the size

  public void add(String str)
  {
    // YOUR CODE GOES HERE
  }

  // Method "sort" sorts the array arr using any of the methods that we have 
  // seen in the lecture and/or classes, you can also use other sorting 
  // algorithms if you prefer to do so. Since the input will be large it is 
  // advisable to use an algorithm with whose running time is bounded by 
  // O(n log n), where n is the size of arr. Having it sort "in place" is 
  // a good idea, too.

  // You will probably find the method str1.compareTo(str2) useful for comparing
  // strings. Do not use <, ==, or >.
  
  public void heapSort()
  { 
      buildHeap();
      while (heapSize > 1)
      { 
          int temp = value[heapSize];
          value[heapSize] = value [1];
          value[1] = temp;
          heapSize--;
          siftdown(1);
      }
  }

  // Method "getSize" returns the size of the array arr

  public int getSize() 
  {
    // YOUR CODE GOES HERE
    return 0;
  }

  // Method "print" prints the contents of arr, one entry per line

  public void print() 
  {
    for(int i = 0; i < size; i++)
      System.out.println(arr[i]);
  }

  // Method "findLinearSearch" returns true/false depending on whether 
  // the string given as an argument is contained in arr or not. It looks 
  // for the string by scanning through the elements of arr from position 
  // 0 to size-1.

  public boolean findLinearSearch(String txt)
  {
      boolean sorted = true;
      int i=1
      for (i=0;i<length(array);i++): 
      if(array[i]==key):
          return i
         
    return false;
  }

  // Method "findBinarySearch" returns true/false depending on whether the 
  // string given as an argument is contained in arr or not. It looks for 
  // the string using binary search.

  public boolean findBinarySearch(String txt)
  {
    // YOUR CODE GOES HERE
    return false;
  }

  public boolean findBinarySearch(String txt, int l, int r)
  {
    int low = 0;
    int high = a.length - 1;
    while ()
    {
        int mid = (low + high)/2;
        if (int r = a[mid])
            return true;
        else if (target < a[mid] //string is lower than the middle 
            high = mid - 1; 
        else 
            low = mid + 1; 
     }
}
      
                     
            
        
        
    return false; 
  }


}
