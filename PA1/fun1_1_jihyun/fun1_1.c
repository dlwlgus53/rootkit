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
void ** sctable ;
char filepath[10][512];
int top =0;
int count=0;
int uid=0;

int getlogin_r(char *buf,size_t bufsize);

asmlinkage int (*orig_sys_open)(const char __user * filename, int flags, umode_t mode) ; 

asmlinkage int openhook_sys_open(const char __user * filename, int flags, umode_t mode)
{
	char fname[512] ;
	copy_from_user(fname, filename,512) ;

	
		top++;
		if(top>=10)	top = top%10;
		filepath[top][0] = '\0';
		strcpy(&filepath[top][0],fname);
		printk("uid : %d file : %s mode : %d\n", current->cred->uid.val, &filepath[top][0], mode) ;
	
	

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

	//sprintf(buf, "%s:%d\n", filepath, count) ;


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
	sctable[__NR_open] = openhook_sys_open ;

	//sscanf(buf,"%s", filepath) ;
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
	char buf[128];

	pte_t * pte ;

	
	printk("name :  %s \n",buf);


	proc_create("uid", S_IRUGO | S_IWUGO, NULL, &openhook_fops);
	printk("user id %d", current->cred->uid.val) ;
	///printk("user name %s", current->cred->name);

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
	remove_proc_entry("uid", NULL) ;

	sctable[__NR_open] = orig_sys_open ;
	pte = lookup_address((unsigned long) sctable, &level) ;
	pte->pte = pte->pte &~ _PAGE_RW ;
}

module_init(openhook_init);
module_exit(openhook_exit);

