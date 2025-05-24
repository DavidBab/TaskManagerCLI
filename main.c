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

int main(int argc, char *argv[])
{
    char buffer[BUFSIZE];
    char argument[BUFSIZE - 32], command[32];
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
            add_task("tasks.txt", task);
            fprintf(stdout, "Added task: [%d] %s\n", task.id, task.name);
        }
        else if (strcmp(command, "list") == 0)
        {
            fprintf(stdout, "List of tasks:\n");
            list_task("tasks.txt");
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
        fprintf(stderr, "Error reading from %s", file_name);
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), f) != NULL)
        fprintf(stdout, "%s", buffer);

    fclose(f);
    return 0;
}