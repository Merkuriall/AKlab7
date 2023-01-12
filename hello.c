#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <asm-generic/bug.h>

MODULE_AUTHOR("Karutsa Denis");
MODULE_DESCRIPTION("Lab 7");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int repeat_num = 1;

module_param(repeat_num, uint, S_IRUGO);

MODULE_PARM_DESC(repeat_num, "Number of 'Hello, World!' string prints");

struct event_list {
  struct list_head list;
  ktime_t event_time;
};

static struct list_head event_list_head;

static LIST_HEAD(event_list_head);

void new_event(void);

static int __init hello_init(void)
{
  BUG_ON(repeat_num > 10);
  
  if (repeat_num == 0 || (repeat_num >= 5 && repeat_num <= 10)) {
    printk(KERN_WARNING "Parameter is 0 or between 5 and 10\n");  
  }

  int i;
  for (i = 0; i < repeat_num; i++) {
    printk(KERN_EMERG "Hello, World!\n");
    new_event();
    WARN(i == 2, "начебто kmalloc() повернув 0\n");
  }

  return 0;
}

static void __exit hello_exit(void) {
  struct event_list *md, *tmp;
  list_for_each_entry_safe(md, tmp, &event_list_head, list) {
    printk(KERN_EMERG "Time: %lld\n", md->event_time);
    list_del(&md->list);
    kfree(md);
  }
}

void new_event(void) {
  struct event_list *element = NULL;
  element = kmalloc(sizeof(struct event_list), GFP_KERNEL);
  element->event_time = ktime_get();
  list_add_tail(&element->list, &event_list_head);
}

module_init(hello_init);
module_exit(hello_exit);
