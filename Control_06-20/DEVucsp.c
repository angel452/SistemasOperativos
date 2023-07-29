#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h> /* for put_user */

MODULE_LICENSE("GPL");

#define SUCCESS 0
#define DEVICE_NAME "UCSP"       // Dev name as it appears in /proc/devices
#define BUF_LEN 80               // Max length of the message from the device

// ----------------- Prototipos de funciones ----------------------
int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

// ----------------- Variables globales ----------------------
static int Major;                   // Major number assigned to our device driver
static int Device_Open = 0;         // Is device open?
                                    // Used to prevent multiple access to device
static char msg[BUF_LEN] = {0};     // The msg the device will give when asked
static char *msg_Ptr;

static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};

// ----------------- Funciones de modulo ----------------------
// --> Se llama cuando se carga el modulo
int init_module(void)
{
    Major = register_chrdev(0, DEVICE_NAME, &fops);

    if (Major < 0) {
        printk(KERN_ALERT "Registering char device failed with %d\n", Major);
        return Major;
    }

    printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
    printk(KERN_INFO "the driver, create a dev file with\n");
    printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
    printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
    printk(KERN_INFO "the device file.\n");
    printk(KERN_INFO "Remove the device file and module when done.\n");
    return SUCCESS;
}

// --> Se llama cuando se desinstala el modulo
void cleanup_module(void)
{
    //int ret = unregister_chrdev(Major, DEVICE_NAME);
    unregister_chrdev(Major, DEVICE_NAME);

    //if (ret < 0)
    //    printk(KERN_ALERT "Error in unregister_chrdev: %d\n", ret);
}
    
// ----------------- Metodos ------------ ----------------------
// --> Se llama cuando se abre el archivo como cat /dev/UCSP
static int device_open(struct inode *inode, struct file *file)
{
    static int counter = 0;
    //if (Device_Open)
    //    return -EBUSY;
    Device_Open++;
    //sprintf(msg, "I already told you %d times Hello world!\n", counter++);
    msg_Ptr = msg;
    try_module_get(THIS_MODULE);

    return SUCCESS;
}

// --> Se llama cuando se cierra el archivo
static int device_release(struct inode *inode, struct file *file)
{
    Device_Open--;     /* We're now ready for our next caller */

    /*
    * Decrement the usage count, or else once you opened the file, you'll
    * never get get rid of the module.
    */

    module_put(THIS_MODULE);
    return 0;
}

// --> Se llama cuando se lee el archivo como cat /dev/UCSP
static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t * offset)
{

    /*
    * Number of bytes actually written to the
    */
    int bytes_read = 0;

    /*
    * If we're at the end of the message,
    * return 0 signifying end of file
    */

    if (*msg_Ptr == 0)
        return 0;

    /*
    * Actually put the data into the buffer
    */

    while (length && *msg_Ptr) {
        /*
        * The buffer is in the user data segment, not the kernel
        * segment so "*" assignment won't work. We have to use
        * put_user which copies data from the kernel data segment to
        * the user data segment.
        */
        put_user(*(msg_Ptr++), buffer++);
        length--;
        bytes_read++;
    }

    /*
    * Most read functions return the number of bytes put into the buffer
    */

    return bytes_read;
}

// --> Se llama cuando se escribe en el archivo como: echo "hola" > /dev/UCSP
static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{
    int i;
    #ifdef DEBUG
        printk(KERN_INFO "device_write(%p,%s,%d)",filp,buff,len);
    #endif
    for(i=0;i<len && i< BUF_LEN; i++)
        get_user(msg[i], buff+i);
    msg_Ptr=msg;
    return i;

    /*
    int t = 0;
    t=copy_from_user(msg, buff, len);

    //sprintf(message, "%s(%zu letters)", buff, len); 
    printk(KERN_INFO "UCSP Char: Received %zu characters from the user %s\n", len, buff);
    return len;
    */
}