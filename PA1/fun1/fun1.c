#include <linux/syscalls.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <linux/kallsyms.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <asm/unistd.h>

MODULE_LICENSE("GPL");

char username[128] = { 0x0, } ;
void ** sctable ;
int count = 0 ;

asmlinkage int (*orig_sys_open)(const char __user * filename, int flags, umode_t mode) ;


static 
int fun1_proc_open(struct inode *inode, struct file *file) {
	return 0 ;
}

static 
int fun1_proc_release(struct inode *inode, struct file *file) {
	return 0 ;
}

//read proc file and print out to kernel
static
ssize_t fun1_proc_read(struct file *file, char __user *ubuf, size_t size, loff_t *offset) 
{
	char buf[256] ;
	ssize_t toread ;
	//in here, by current->cred->uid.val, can find user id as an int
	
	sprintf(buf, "username : %s, userid : %d\n", username, current->cred->uid.val) ;


	toread = strlen(buf) >= *offset + size ? size : strlen(buf) - *offset ;

	if (copy_to_user(ubuf, buf + *offset, toread))
		return -EFAULT ;	

	*offset = *offset + toread ;

	return toread ;
}


//write to proc file
//write username
static 
ssize_t fun1_proc_write(struct file *file, const char __user *ubuf, size_t size, loff_t *offset) 
{
	char buf[128] ;

	if (*offset != 0 || size > 128)
		return -EFAULT ;

	if (copy_from_user(buf, ubuf, size))
		return -EFAULT ;

	sscanf(buf,"%s", username) ;
	printk("user: %s", username);		
	*offset = strlen(buf) ;

	return *offset ;
}

static const struct file_operations fun1_fops = {
	.owner = 	THIS_MODULE,
	.open = 	fun1_proc_open,
	.read = 	fun1_proc_read,
	.write = 	fun1_proc_write,
	.llseek = 	seq_lseek,
	.release = 	fun1_proc_release,
} ;

static 
int __init fun1_init(void) {
	unsigned int level ; 
	pte_t * pte ;
	printk("fun1 hello\n");
	proc_create("fun1", S_IRUGO | S_IWUGO, NULL, &fun1_fops) ;
	return 0;
}

static 
void __exit fun1_exit(void) {
	unsigned int level ;
	pte_t * pte ;
	printk("fun1 bye\n");
	remove_proc_entry("fun1", NULL) ;

}

module_init(fun1_init);
module_exit(fun1_exit);
