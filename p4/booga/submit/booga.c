#include <linux/module.h>
#include <linux/version.h>
#include <linux/random.h>
#include <linux/kernel.h> /* printk() */
#include <linux/init.h>  /*  for module_init and module_cleanup */
#include <linux/slab.h>  /*  for kmalloc/kfree */
#include <linux/fs.h>     /* everything... */
#include <linux/errno.h>  /* error codes */
#include <linux/signal.h>  
#include <linux/sched.h>  
#include <linux/thread_info.h>  
#include <linux/types.h>  /* size_t */
#include <linux/proc_fs.h>	/* for the proc filesystem */
#include <linux/seq_file.h>
#include <linux/string.h>
#include "booga.h"        /* local definitions */

static int booga_major =   BOOGA_MAJOR;
static int booga_number_of_minors = BOOGA_NUMBER_OF_MINORS;    /* number of bare booga devices */
module_param(booga_major, int, 0);
module_param(booga_number_of_minors, int, 0);
MODULE_AUTHOR("Reuben Tanner");
MODULE_LICENSE("GPL v2");

static ssize_t booga_read (struct file *, char *, size_t , loff_t *);
static ssize_t booga_write (struct file *, const char *, size_t , loff_t *);
static int booga_open (struct inode *, struct file *);
static int booga_release (struct inode *, struct file *);

static int random(void);
static void update_phrase_usage(int phrase_index);
static void update_opens(int minor_number);
static void update_byte_throughput(int is_read, int number_of_bytes);

static struct file_operations booga_driver_operations = {
    .read =       booga_read,
    .write =      booga_write,
    .open =       booga_open,
    .release =    booga_release,
};

static booga_stats *booga_device_stats;
static struct proc_dir_entry* booga_proc_file;

static int booga_open (struct inode *inode, struct file *file_struct)
{
    int minor_number = NUM(inode->i_rdev);
    
    if (minor_number >= booga_number_of_minors) return -ENODEV;

    file_struct->f_op = &booga_driver_operations;
    
    if (down_interruptible(&booga_device_stats->lock))
       return (-ERESTARTSYS);
    update_opens(minor_number);
    up(&booga_device_stats->lock);
        
    try_module_get(THIS_MODULE);
    return 0;
}

static int booga_release (struct inode *inode, struct file *filp)
{
    module_put(THIS_MODULE);
    return (0);
}

static ssize_t booga_read (struct file *filp, char *user_space_buffer, size_t number_of_bytes_to_read, loff_t *f_pos)
{
    int bytes_read = 0;
    char* kernel_space_buffer = (char*)kmalloc(sizeof(char)*number_of_bytes_to_read, GFP_KERNEL);
    int size_of_phrase;
    int phrase_index;
    char* phrase;   

    printk("<1>booga_read invoked.\n");
    
    if (number_of_bytes_to_read > KMALLOC_MAX_SIZE)
    {
        printk("<1>attempted to read more than max size.\nSetting to default max %lu", KMALLOC_MAX_SIZE);
        number_of_bytes_to_read = KMALLOC_MAX_SIZE;
    }

    if (down_interruptible(&booga_device_stats->lock))
       return (-ERESTARTSYS);
    update_byte_throughput(READ_OPERATION, number_of_bytes_to_read);
    up(&booga_device_stats->lock);
        
    if (!kernel_space_buffer)
    {
        printk("<1>Memory allocation failed. Aborting read.\n");
        return -ENOMEM;
    }
    
    memset(kernel_space_buffer, '\0', number_of_bytes_to_read);
    while (number_of_bytes_to_read != 0)
    {
        phrase_index = random();
        phrase = phrases[phrase_index];

        if (down_interruptible(&booga_device_stats->lock))
            return (-ERESTARTSYS);
        update_phrase_usage(phrase_index);
        up(&booga_device_stats->lock);
                
        size_of_phrase = strlen(phrase);
        if (size_of_phrase <= number_of_bytes_to_read)
        {
            number_of_bytes_to_read -= size_of_phrase;
            bytes_read += size_of_phrase;
            strcat(user_space_buffer, phrase);
        }
        else if (number_of_bytes_to_read!=0)
        {
            char substring[number_of_bytes_to_read+1];
            memset(substring, '\0', sizeof(substring));
            strncpy(substring, phrase, number_of_bytes_to_read);
            strcat(user_space_buffer, substring);
            
            bytes_read += number_of_bytes_to_read;
            number_of_bytes_to_read = 0;
        }
    }
    copy_to_user(user_space_buffer, kernel_space_buffer, strlen(kernel_space_buffer));
    kfree(kernel_space_buffer);
    return 0;
}

static ssize_t booga_write (struct file *file_struct, const char *user_space_buffer, size_t number_of_bytes , loff_t *f_pos)
{
    struct inode* inode;
    int minor_number;
    int kill_status;
    
    printk("<1>booga_write invoked.\n");
    
    inode = file_struct->f_inode;
    minor_number = NUM(inode->i_rdev);
    
    if (minor_number == PROCESS_KILLER)
    {
        kill_status = send_sig_info(SIGTERM, SEND_SIG_NOINFO, current);
        if (kill_status)
        {
            printk("<1>Error killing process: %d.\n", kill_status);
        }
        return 0;
    }
    else
    {
        if (down_interruptible(&booga_device_stats->lock))
            return (-ERESTARTSYS);
        update_byte_throughput(WRITE_OPERATION, number_of_bytes);
        up(&booga_device_stats->lock);
        return number_of_bytes;     
    }
    
}

static void init_booga_device_stats(void)
{
    booga_device_stats->bytes_read=0;
    booga_device_stats->bytes_written=0;
    booga_device_stats->booga0_open=0;
    booga_device_stats->booga1_open=0;
    booga_device_stats->booga2_open=0;
    booga_device_stats->booga3_open=0;
    booga_device_stats->phrase0=0;
    booga_device_stats->phrase1=0;
    booga_device_stats->phrase2=0;
    booga_device_stats->phrase3=0;
    sema_init(&booga_device_stats->lock, 1);
}

static int booga_proc_show(struct seq_file *sequence_file, void *v)
{
    seq_printf(sequence_file, "bytes read = %lu\n", booga_device_stats->bytes_read);
    seq_printf(sequence_file, "bytes written = %lu\n", booga_device_stats->bytes_written);
    seq_printf(sequence_file, "number of opens:\n");
    seq_printf(sequence_file, "\t/dev/booga0 = %lu times\n", booga_device_stats->booga0_open);
    seq_printf(sequence_file, "\t/dev/booga1 = %lu times\n", booga_device_stats->booga1_open);
    seq_printf(sequence_file, "\t/dev/booga2 = %lu times\n", booga_device_stats->booga2_open);
    seq_printf(sequence_file, "\t/dev/booga3 = %lu times\n", booga_device_stats->booga3_open);
    seq_printf(sequence_file, "strings output:\n");
    seq_printf(sequence_file, "\t%s = %lu times\n", phrases[0], booga_device_stats->phrase0);
    seq_printf(sequence_file, "\t%s = %lu times\n", phrases[1], booga_device_stats->phrase1);
    seq_printf(sequence_file, "\t%s = %lu times\n", phrases[2], booga_device_stats->phrase2);
    seq_printf(sequence_file, "\t%s = %lu times\n", phrases[3], booga_device_stats->phrase3);
    return 0;
}

static int booga_proc_open(struct inode *inode, struct file *file)
{
    return single_open(file, booga_proc_show, NULL);
}

static const struct file_operations booga_proc_file_operations = {
     .owner	= THIS_MODULE,
     .open	= booga_proc_open,
     .read	= seq_read,
     .llseek	= seq_lseek,
     .release	= single_release,
};

static __init int booga_init(void)
{
    int dynamic_major_number;

    dynamic_major_number = register_chrdev(booga_major, "booga", &booga_driver_operations);
    
    if (dynamic_major_number < 0) {
        printk(KERN_WARNING "booga: can't get major %d\n",booga_major);
        return dynamic_major_number;
    }
    
    if (booga_major == 0) 
    {
        booga_major = dynamic_major_number;
    }
    printk("<1> booga device driver version 3: loaded at major number %d\n", booga_major);

    booga_device_stats = (booga_stats *) kmalloc(sizeof(booga_stats),GFP_KERNEL);
    if (!booga_device_stats) 
    {
      dynamic_major_number = -ENOMEM;
      goto fail_malloc;
    }
    init_booga_device_stats();

    proc_mkdir("driver", NULL);
    booga_proc_file = proc_create("driver/booga", 0, NULL, &booga_proc_file_operations); 
    if (!booga_proc_file)  
    {
      dynamic_major_number = -ENOMEM;
      goto fail_malloc;
    }

    return 0;

    fail_malloc:
        unregister_chrdev(booga_major, "booga");
        remove_proc_entry("driver/booga", NULL /* parent dir */);
        return  dynamic_major_number;
}


static __exit  void booga_cleanup(void)
{
    remove_proc_entry("driver/booga", NULL /* parent dir */);
    kfree(booga_device_stats);
    unregister_chrdev(booga_major, "booga");
    printk("<1> booga unloaded\n");
}

static int random()
{
    char randval;
    get_random_bytes(&randval, 1);
    return (randval & 0x7F) % 4; 
}

static void update_opens(int minor_number)
{
    switch (minor_number)
    {
        case 0:
            booga_device_stats->booga0_open++;
            break;
        case 1:
            booga_device_stats->booga1_open++;
            break;
        case 2:
            booga_device_stats->booga2_open++;
            break;
        case 3:
            booga_device_stats->booga3_open++;
            break;
    }
}

static void update_byte_throughput(int operation, int number_of_bytes)
{
    if (operation == READ_OPERATION)
    {
        booga_device_stats->bytes_read+=number_of_bytes;
    }
    else
    {
        printk("Wrote %d bytes\n", number_of_bytes);
        booga_device_stats->bytes_written+=number_of_bytes;
    }
}

static void update_phrase_usage(int phrase_index)
{
    switch (phrase_index)
    {
        case 0:
            booga_device_stats->phrase0++;
            break;
        case 1:
            booga_device_stats->phrase1++;
            break;
        case 2:
            booga_device_stats->phrase2++;
            break;
        case 3:
            booga_device_stats->phrase3++;
            break;
    }
}


module_init(booga_init);
module_exit(booga_cleanup);

