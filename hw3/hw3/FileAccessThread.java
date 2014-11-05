/**
 *
 * @author reubentanner
 */
public class FileAccessThread extends Thread {
    
    private FileAccessMonitor monitor;

    public FileAccessThread(int pid, FileAccessMonitor monitor) 
    {
        this.monitor = monitor;
        Thread.currentThread().setName(pid + "");
    }

    @Override
    public void run() 
    {
        try 
        {
            monitor.accessFile();
            Thread.sleep(1000);
            monitor.releaseFile();
        } catch (InterruptedException ex) 
        {
            System.err.println("Bad things have happened.");
        }
    }
}
