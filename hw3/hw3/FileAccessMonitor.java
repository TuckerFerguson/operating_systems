	/**
 *
 * @author reubentanner
 */
public class FileAccessMonitor {
    
    int total;
    int max;
    
    public FileAccessMonitor(int max)
    {
        this.max = max;
        this.total = 0;
    }
    
    public synchronized void accessFile() throws InterruptedException
    {
        int pid = getPid();
        if (!(pid > max))
        {
            while (total + pid > max)
            {
                this.wait();
            }
            System.out.println(String.format("Thread %d accessing file.", pid));
            this.total += pid;
        }
        
    }
    
    public synchronized void releaseFile()
    {
        int pid = getPid();
        System.out.println(String.format("Thread %d releasing file.", pid));
        this.total -= pid;
        this.notifyAll();
    }
    
    public int getPid()
    {
        return Integer.parseInt(Thread.currentThread().getName());
    }
}
