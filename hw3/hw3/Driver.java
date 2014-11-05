import java.util.ArrayList;
import java.util.List;
import java.util.Random;

/**
 *
 * @author reubentanner
 */
public class Driver {

    private static FileAccessMonitor monitor;
    private static final int MAX_THREADS = 16;
    
    
    public static void main(String[] args) throws InterruptedException 
    {
        if (args.length != 1)
        {
            System.err.println("Usage: java Driver <n> ");
            System.exit(1);
        }
        
        int max = Integer.parseInt(args[0]);
        monitor = new FileAccessMonitor(max);
        Random rand = new Random(max);
        
        List<Thread> threads = new ArrayList<>();
        for (int i = 0; i < MAX_THREADS; ++i)
        {
           Thread fileAccessThread = new FileAccessThread(rand.nextInt(max), monitor);
           threads.add(fileAccessThread);
           fileAccessThread.start();
        }
        
        for (Thread thread : threads)
        {
            thread.join();
        }
    }
}
