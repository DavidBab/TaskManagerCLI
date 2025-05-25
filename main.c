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
            add_task("tasks.txt", task);
        }
        else if (strcmp(command, "list") == 0)
        {
            fprintf(stdout, "List of tasks:\n");
            list_task("tasks.txt");
        }
        else if (strcmp(command, "remove") == 0)
        {
            remove_task("tasks.txt", argument);
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
    fprintf(stdout, "Added task: [%d] %s\n", task.id, task.name);

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
    FILE *f, *temp;
    char buffer[BUFSIZE];
    int current_id;
    char task_name[BUFSIZE];

    // Convert the string to an integer safely
    char *endptr;
    int task_id = strtol(task_id_str, &endptr, 10);
    if (*endptr != '\0')
    {
        fprintf(stderr, "Invalid task ID: %s\n", task_id_str);
        return 1;
    }

    // Open the original file for reading
    if ((f = fopen(file_name, "r")) == NULL)
    {
        fprintf(stderr, "Error opening file %s\n", file_name);
        return 1;
    }

    // Open a temporary file for writing
    if ((temp = fopen("temp.txt", "w")) == NULL)
    {
        fprintf(stderr, "Error opening temporary file\n");
        fclose(f);
        return 1;
    }

    // Process each line
    while (fgets(buffer, sizeof(buffer), f))
    {
        if (sscanf(buffer, "[%d] %[^\n]", &current_id, task_name) == 2)
        {
            if (current_id == task_id)
                continue; // Skip the task to remove

            // Write the remaining tasks with their original IDs
            fprintf(temp, "[%d] %s\n", current_id, task_name);
        }
    }

    fclose(f);
    fclose(temp);

    // Replace the original file with the updated file
    remove(file_name);
    rename("temp.txt", file_name);

    return 0;
}
