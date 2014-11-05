#include "DirectoryTraverser.h"

//ListPtr directory_stack;
//int traversal_count;
//
//void push_to_directory_stack(char* dir);
//
//void init()
//{
//    directory_stack = createList(compareTo, toString, freeObject);
//    traversal_count = 0;
//}
//
void change_to_directory(char* dir)
{
    chdir(dir);
}

void change_to_home_directory()
{
    struct passwd *user_pass;
    errno = 0;
    char* dir_to_change;

    user_pass = getpwuid(getuid());
    if (errno == EIO || errno == EINTR || errno == EMFILE)
    {
        fprintf(stderr, "Could not get user home directory. 'cd' will navigate to root");
        dir_to_change="/";
    }
    else
    {
        dir_to_change=user_pass->pw_dir;
    }

    printf("%s\n", dir_to_change);
    change_to_directory(dir_to_change);
}

char* get_current_directory()
{
    char* current_directory = (char*)malloc(MAX_DIRECTORY_SIZE * sizeof(char));
    
    if (getcwd(current_directory, sizeof(current_directory)) != NULL)
        return current_directory;
    else
        err_sys("Could not get current working directory");
    return NULL;
}
//
//void push_to_directory_stack(char* dir)
//{
//    ObjectPtr directory = createObject(traversal_count, dir);
//    NodePtr node = createNode(directory);
//    addAtRear(directory_stack, node);
//}

//char* pop_from_directory_stack()
//{
//    return *((char*)(removeRear(directory_stack)->data));
//}

