/*INCLUDES ************************************************************/
#include "ses_timer.h"
#include "ses_scheduler.h"
#include "util/atomic.h"
#include "ses_led.h"

/* PRIVATE VARIABLES **************************************************/

static taskDescriptor *taskList = NULL; // list of scheduled tasks
volatile taskDescriptor *temp = NULL;
volatile taskDescriptor *previous = NULL;
static volatile taskDescriptor *current = NULL;
static volatile task_t *task_execute = NULL;
volatile bool is_Same; // flag used to check if same task is added twice?

/*FUNCTION DEFINITION *************************************************/

/** After every 1ms this function executes and marks the redied task(if any)*************************/
static void scheduler_update(void)
{

	temp = taskList; // iteration pointer used to iterate through whole list
	if (temp != NULL)
	{

		while (temp->task != NULL && temp->expire > 0) // iterates through valid tasks
		{

			ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
			{
				temp->expire--; // decrements decreasing the expiry time of all tasks by 1 ms
			}

			if (temp->expire == 0) // Checks if task is ready to execute
			{
				temp->execute = 1; // ready to execute

				if (temp->period > 0)
				{
					ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
					{
						temp->expire = temp->period; //expiration time of periodic tasks is reset to the period
					}
				}
			}

			if (temp->next == NULL) // if it's the last task then break
			{
				break;
			}
			else
			{
				while (temp->next != NULL) // else point to the next task
				{
					ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
					{
						temp = temp->next;
					}
				}
			}
		}
	}
}

/** Initialzes the Schedular ***************************************/
void scheduler_init()
{
	timer2_start();						  //  sets the timer2 of 1 ms
	timer2_setCallback(scheduler_update); //The callback for the Timer2 interrupt to update the scheduler every 1 ms
}

/*** Actual execution of readied tasks (if any) *************************/
void scheduler_run()
{

	while (1)
	{

		current = taskList;
		while (current != NULL) // if a task is valid
		{
			if (current->execute == 1 && current->period > 0) // checks wether a task is ready and is it periodic?
			{
				current->task(current->param);
				current->execute = 0;
			}

			if (current->execute == 1 && current->period == 0) // checks wether a task is ready and is it non-periodic?
			{
				current->task(current->param);
				current->execute = 0;
				scheduler_remove(current); // removes non-periodics tasks from list after first execution
			}
			ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
			{
				current = current->next;
			}
		}
	}
}

/**** Adds the task to the end of singly link list *************************/
bool scheduler_add(taskDescriptor *toAdd)
{
	is_Same = false;
	if (toAdd != NULL)
	{

		toAdd->next = NULL;

		if (taskList == NULL) // if taskList is empty than make toADD as a first task
		{
			ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
			{
				taskList = toAdd;
			}
		}
		else
		{
			temp = taskList;
			if (temp->task == toAdd->task) //checks if the first task is same then sets flag
			{
				is_Same = true;
				return false;
			}
			while (temp->next != NULL) // if list is not empty than add new task at the end of list?
			{
				if (temp->task == toAdd->task) // if the task is already in schedule then break from the loop
				{
					is_Same = true;
					return false;
					break;
				}
				ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
				{
					temp = temp->next;
				}
			}
			if (!is_Same) // if task is already in sceduler then don't add.
			{
				ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
				{
					temp->next = toAdd;
					return true;
				}
			}
		}
	}
	else
	{
		return false;
	}
}


/* it reomoves the task from singly list, ***************************/
void scheduler_remove(taskDescriptor *toRemove)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		temp = taskList;

		if (temp != NULL) // checks from the valid list
		{
			if (temp == toRemove) // if the task to remove is at the first node
			{
				taskList = taskList->next; // start the list from second node
				return;
			}
			else
			{
				while (temp != toRemove) // if the task to remove is not at the first node
				{
					previous = temp;   // points to previous node from a node to delete
					temp = temp->next; // points to one node ahead  from a previous node
				}
				previous->next = temp->next; // deletes the link of a valid task from a list
			}
		}
	}
}
