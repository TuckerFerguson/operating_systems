/**
 *
 * @author reubentanner
 */
public class FileAccessThread extends Thread {
    
    private FileAccessMonitor monitor;
    private int pid;

    public FileAccessThread(int pid, FileAccessMonitor monitor) 
    {
        this.monitor = monitor;
        this.pid = pid;
    }

    @Override
    public void run() 
    {
        try 
        {
            Thread.currentThread().setName(pid + "");
            monitor.accessFile();
            Thread.sleep(1000);
            monitor.releaseFile();
        } catch (InterruptedException ex) 
        {
            System.err.println("Bad things have happened.");
        }
    }
}
