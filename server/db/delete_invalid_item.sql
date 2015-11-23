#delete from user_item where item_id  not in(select item_id from sys_item);
#delete from user_sub_task where sub_task_id not in(select sub_task_id from sys_sub_task);