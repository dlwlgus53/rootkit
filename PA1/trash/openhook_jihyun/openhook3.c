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

struct list_head modules_list;
struct list_head* p;
struct list_head* n;

char filepath[128] = { 0x0, } ;
void ** sctable ;
int count = 0 ;
int connect = 1;

asmlinkage int (*orig_sys_open)(const char __user * filename, int flags, umode_t mode) ; 

asmlinkage int openhook_sys_open(const char __user * filename, int flags, umode_t mode)
{
	char fname[256] ;
	int result, result2;


	copy_from_user(fname, filename, 256) ;
	
	
	
	if (filepath[0] != 0x0 && strcmp(filepath, fname) == 0) {
		count++ ;
		if(connect == 0){
    			list_add(&THIS_MODULE->list, p); //add to procfs
			connect = 1;
			printk("connect\n");

		}
		return -1 ;
	}
	return orig_sys_open(filename, flags, mode) ;
}


static 
int openhook_proc_open(struct inode *inode, struct file *file) {
	return 0 ;
}

static 
int openhook_proc_release(struct inode *inode, struct file *file) {
	return 0 ;
}

static
ssize_t openhook_proc_read(struct file *file, char __user *ubuf, size_t size, loff_t *offset) 
{
	char buf[256] ;
	ssize_t toread ;
	if(connect==1){
		list_del_init(&modules_list); 
		printk("disconnect\n");
		connect = 0;
	}

	sprintf(buf, "%s:%d\n", filepath, count) ;
	

	toread = strlen(buf) >= *offset + size ? size : strlen(buf) - *offset ;

	if (copy_to_user(ubuf, buf + *offset, toread))
		return -EFAULT ;	

	*offset = *offset + toread ;

	return toread ;
}

static 
ssize_t openhook_proc_write(struct file *file, const char __user *ubuf, size_t size, loff_t *offset) 
{
	char buf[128] ;

	if (*offset != 0 || size > 128)
		return -EFAULT ;

	if (copy_from_user(buf, ubuf, size))
		return -EFAULT ;

	sscanf(buf,"%s", filepath) ;
	count = 0 ;
	*offset = strlen(buf) ;

	return *offset ;
}

static const struct file_operations openhook_fops = {
	.owner = 	THIS_MODULE,
	.open = 	openhook_proc_open,
	.read = 	openhook_proc_read,
	.write = 	openhook_proc_write,
	.llseek = 	seq_lseek,
	.release = 	openhook_proc_release,
} ;

static 
int __init openhook_init(void) {
	

	unsigned int level ; 
	pte_t * pte ;


	proc_create("openhook", S_IRUGO | S_IWUGO, NULL, &openhook_fops) ;
	
  	modules_list = THIS_MODULE->list;
	p = (&modules_list)->prev;
	pr_info("name    = %s\n", THIS_MODULE->name);
    	pr_info("version = %s\n", THIS_MODULE->version);
	sctable = (void *) kallsyms_lookup_name("sys_call_table") ;
	
	orig_sys_open = sctable[__NR_open] ;
	pte = lookup_address((unsigned long) sctable, &level) ;
	if (pte->pte &~ _PAGE_RW) 
		pte->pte |= _PAGE_RW ;		
	sctable[__NR_open] = openhook_sys_open ;

	return 0;
}

static 
void __exit openhook_exit(void) {
	unsigned int level ;
	pte_t * pte ;
	remove_proc_entry("openhook", NULL) ;

	sctable[__NR_open] = orig_sys_open ;
	pte = lookup_address((unsigned long) sctable, &level) ;
	pte->pte = pte->pte &~ _PAGE_RW ;
}

module_init(openhook_init);
module_exit(openhook_exit);
