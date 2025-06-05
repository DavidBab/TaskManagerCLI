#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 512

struct Task
{
    int id;
    char name[BUFSIZE];
};

int add_task(char *file_name, struct Task task);
int list_task(char *file_name);
int get_last_id(char *file_name);
int remove_task(char *file_name, char *task_id_str);

int main(int argc, char *argv[])
{
    char buffer[BUFSIZE];
    char argument[BUFSIZE - 32], command[32];
    int id = 0;
    struct Task task;

    while (1)
    {
        fgets(buffer, BUFSIZE, stdin);
        sscanf(buffer, "%s \"%[^\"]\"", command, argument);

        if (strcmp(command, "exit") == 0)
        {
            break;
        }
        else if (strcmp(command, "add") == 0)
        {
            task.id = get_last_id("tasks.txt"); // Adding ID to task
            strcpy(task.name, argument);        // Setting task name
            if (add_task("tasks.txt", task) == 0)
            {
                fprintf(stdout, "Added task: [%d] %s\n", task.id, task.name);
            }
        }
        else if (strcmp(command, "list") == 0 || strcmp(command, "ls") == 0)
        {
            fprintf(stdout, "List of tasks:\n");
            list_task("tasks.txt");
        }
        else if (strcmp(command, "remove") == 0 || strcmp(command, "rm") == 0)
        {
            remove_task("tasks.txt", argument);
        }
        else if (strcmp(command, "help") == 0)
        {
            fprintf(stdout, "Available commands:\n");
            fprintf(stdout, "  add \"task name\"   - Add a new task\n");
            fprintf(stdout, "  list              - List all tasks\n");
            fprintf(stdout, "  remove \"id\"       - Remove a task by ID\n");
            fprintf(stdout, "  exit              - Exit the program\n");
        }
    }

    return 0;
}

int add_task(char *file_name, struct Task task)
{
    FILE *f;

    if ((f = fopen(file_name, "a")) == NULL)
    {
        fprintf(stderr, "Error writing to file %s", file_name);
        return 1;
    }

    fprintf(f, "[%d] %s\n", task.id, task.name);

    fclose(f);

    return 0;
}

int get_last_id(char *file_name)
{
    FILE *f;
    int id = 0, temp_id;
    char buffer[BUFSIZE];

    if ((f = fopen(file_name, "r")) == NULL)
    {
        fprintf(stderr, "Error getting last_id from file %s", file_name);
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), f) != NULL)
    {
        if (sscanf(buffer, "[%d]", &temp_id) == 1)
        {
            id = temp_id;
        }
    }

    fclose(f);
    return id + 1;
}

int list_task(char *file_name)
{
    FILE *f;
    char buffer[BUFSIZE * 2];

    if ((f = fopen(file_name, "r")) == NULL)
    {
        fprintf(stderr, "Error reading from %s\n", file_name);
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), f) != NULL)
        fprintf(stdout, "%s", buffer);

    fclose(f);
    return 0;
}

int remove_task(char *file_name, char *task_id_str)
{
    FILE *fw, *fr;
    char buffer[BUFSIZE], argument[BUFSIZE];
    int id, temp_id, new_id = 1;
    int found = 0;
    struct Task task;

    if ((fr = fopen(file_name, "r")) == NULL)
    {
        fprintf(stderr, "Error reading from %s\n", file_name);
        return 1;
    }

    if ((fw = fopen("temp", "w")) == NULL)
    {
        fprintf(stderr, "Error reading from %s\n", file_name);
        return 1;
    }

    sscanf(task_id_str, "%d", &id);

    while (fgets(buffer, sizeof(buffer), fr) != NULL)
    {
        if (sscanf(buffer, "[%d] %[^\n]", &temp_id, argument) == 2)
        {
            if (temp_id == id)
            {
                found = 1;
                continue;
            }
            fprintf(fw, "[%d] %s\n", new_id++, argument);
        }
        else
        {
            fprintf(fw, "%s", buffer);
        }
    }

    fclose(fr);
    fclose(fw);

    if (!found)
    {
        fprintf(stderr, "Task with ID %d not found.\n", id);
        remove("temp");
        return 1;
    }

    remove(file_name);
    rename("temp", file_name);

    fprintf(stdout, "Removed task with ID %d\n", id);
    return 0;
}
