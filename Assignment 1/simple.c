#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>

struct birthday{
	int day;
	int month;
	int year;
	struct list_head list;
};

static LIST_HEAD(birthday_list);

/* This function is called when the module is loaded. */
int simple_init(void)
{
	
	int i;
	struct birthday *ptr;
	
    
 	printk(KERN_INFO "Loading Module\n");
       
    for(i=1; i<6; i++){
		struct birthday *person;
		person = kmalloc(sizeof(*person), GFP_KERNEL);
		person->day = i;
		person->month = i+1;
		person->year = 1998+i;
		INIT_LIST_HEAD(&person->list);
		list_add_tail(&person->list, &birthday_list);       
       }
       
    list_for_each_entry(ptr, &birthday_list, list){
		printk("Birthday %d, %d, %d\n", ptr->day, ptr->month, ptr->year);
    }
       

       
       

       return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {
	
	struct birthday *ptr, *next;
	printk(KERN_INFO "Removing Module\n");
	
	list_for_each_entry_safe(ptr,next,&birthday_list,list){
		printk(KERN_INFO "Removing %d, %d, %d\n",ptr->day, ptr->month, ptr->year);
		list_del(&ptr->list);
		kfree(ptr);
	}
	
	
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");
