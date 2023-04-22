//header files that we gonna use are 
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //for string manipulation duh
#include <time.h> //for getting time string into the tasks log file
#include <unistd.h> //for ftruncate system call
#include <stdbool.h>

#define MAX_SIZE 300

typedef struct found{
    int position;
    char *p;
} Found;

typedef struct line_data{
    int *compl;
    int *not_compl;
    int count1;
    int count2;
} Line_Data;

typedef struct line_coordinates{
    int start;
    int end;
} Coordinates;

static void help_sec(void){
    printf("Tasky is a TODO list created in C by Prince Gautam. Commands available are - \n"
            "add  [task]        adds a new task\n"
            "organize       organizes the tasks according to their completion state\n"
            "show [task]        shows the information about a task created by the user\n\
            earlier before.\n"
            "done [task]        checkbox the non-completed task to completed.\n"
            "done all       checks all tasks as 'completed'\n"
            "show all       shows all tasks \n"
            "delete [task]      delete a specific task\n"
            "delete all         delete all tasks entered by the user\n"
            "-h or help     prints this help menu\n"
            "\n"
            "Support the Development - https://github.com/princega/tasky\n"
            "princegautam@pm.me\n");
}


static char* get_time(void){
	char* timee = (char*)malloc(13*sizeof(char));
	time_t current_time;
	time(&current_time);
	strftime(timee, 13, "%b %d %H:%M", localtime(&current_time));
	return timee;
}

static void add(char icon[], FILE *fp, char arg[]){
    fclose(fp);
    fp = fopen("tasks.log","a+");
    char* time = get_time();
    fprintf(fp, "%s   %s      %s\n",icon, arg, time);
    free(time);
}

static Found *find_line(FILE *fp, char arg[]){
    Found *q = (Found*)malloc(sizeof(Found));
    int count = 0;
    char* data = (char*)malloc(MAX_SIZE*sizeof(char));
    while (fgets(data, MAX_SIZE, fp)){
        ++count;
        if(strstr(data, arg) != NULL){
            q -> p = data;
            q -> position = count;
            return q;
        }
    }
    free(data);
    free(q);
    return NULL;
}

static Coordinates* get_coordinates(FILE *fp, int line_number){
    int start, end;
    int line_count = 0;
    char* line = (char*)malloc(MAX_SIZE*sizeof(char));
    Coordinates *point = (Coordinates*)malloc(sizeof(Coordinates));
    rewind(fp);
    while(fgets(line, MAX_SIZE, fp) != NULL){
        ++line_count;
        if (line_count == line_number){
            end = ftell(fp);
            start = end - strlen(line);
            break;
        }
    }
    point -> start = start;
    point -> end = end;
    free(line);
    return point;
}

static Coordinates* get_coordinates_by_buffer(char* string, int line_num){
    Coordinates *p = (Coordinates*)malloc(sizeof(Coordinates));
    bool state = true;
    char c = string[0];
    int index = 0;
    int line_count = 1, end = -1, start = 0;

    while (line_count <= line_num && c != '\0'){
        c = string[index];
        ++index;
        if (line_count == line_num)
            state = false;
        if (c == '\n'){
            ++line_count;
            if (state != false)
            start = end + 2;
        }
        ++end;
    }
    p->start = start;
    p->end = end;
    return p;
}

static long file_size(FILE *fp){
    rewind(fp);
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);
    return size;
}

static void remove_line(FILE *fp, Found *get, Coordinates* y){
    char* buffer = (char*)malloc(file_size(fp));
    fseek(fp, 0, SEEK_SET);
    fread(buffer, 1, file_size(fp), fp);
    memmove(buffer+y->start, buffer+y->end, file_size(fp) - y->end);
    fseek(fp, 0, SEEK_SET);
    fwrite(buffer, 1, file_size(fp), fp);
    ftruncate(fileno(fp), file_size(fp) - strlen(get->p));
    free(buffer);
}

static Line_Data* assign_nums(FILE *fp, char done[], char not_done[]){
    int line_count = 0;
    int count1 = 0, count2 = 0;
    int num1 = 2, num2 = 2;

    //allocating the struct
    Line_Data *p = (Line_Data*)malloc(sizeof(Line_Data));

    int* completed = (int*)calloc(num1, sizeof(int));
    int* not_completed = (int*)calloc(num2, sizeof(int));
    char* data = (char*)malloc(MAX_SIZE*sizeof(char));
    while (fgets(data, MAX_SIZE, fp)){
        ++line_count;
        if (strstr(data, done) != NULL){
            ++count1;
            int num = count1 - 1;
            if (count1 <= num1){
            completed[num]=line_count;
            }
            else {
                num1 *= 2;
                completed = realloc(completed, num1*sizeof(int));
                completed[num]=line_count;
            }
        }
        else if (strstr(data, not_done) != NULL){
            ++count2;
            int num = count2 - 1;
            if (count2 <= num2){
                not_completed[num]=line_count;
            }
            else {
                num2 *= 2;
                not_completed = realloc(not_completed, num2*sizeof(int));
                not_completed[num]=line_count;
            }
        }
    }

    p -> compl = completed;
    p -> not_compl = not_completed;
    p -> count1 = count1;
    p -> count2 = count2;
    free(data);
    return p;
}

static void organize(Line_Data *point, FILE *fp){
    int line_count = 0;
    char* data = (char*)malloc(MAX_SIZE*sizeof(char));

    //printing the completed tasks
    fseek(fp , 0, SEEK_SET);
    if (point->compl[0] != 0){
    printf("< COMPLETED TASKS > \n");}
    while(fgets(data, MAX_SIZE, fp)){
        ++line_count;
        for (int i=0;i<point->count1;i++){
            if (line_count == point->compl[i]){
                printf("%s",data);
            }
        }
    }
    line_count = 0;
    rewind(fp);
    //printing the non completed tasks
    if (point->not_compl[0] != 0){
    printf("< STILL NOT COMPLETED > \n");}
    while(fgets(data, MAX_SIZE, fp)){
        ++line_count;
        for (int i=0;i<point->count2;i++){
            if (line_count == point->not_compl[i]){
                printf("%s",data);
            }
        }
    }
    free(point);
    free(data);
}

static void mark_as_done(FILE *fp, char done[], char not_done[], Coordinates *p){
    char* buffer = (char*)malloc(file_size(fp));
    fseek(fp , 0, SEEK_SET);
    fread(buffer, 1, file_size(fp), fp);
    memmove(buffer+p->start, done, strlen(done));
    memmove(buffer+p->start+strlen(done), buffer+p->start+strlen(not_done), file_size(fp) - (p->start+strlen(not_done)));
    fseek(fp, 0, SEEK_SET);
    fwrite(buffer, 1, file_size(fp), fp);
    ftruncate(fileno(fp), file_size(fp) - (strlen(not_done) - strlen(done)));
    free(buffer);
}

static void mark_all(FILE *fp, char done[], char not_done[],  Line_Data* point){
    char* buffer = (char*)malloc(file_size(fp));
    fseek(fp, 0, SEEK_SET);
    fread(buffer, 1, file_size(fp), fp);
    for (int i=0;i<point->count2;i++){
    Coordinates *get = get_coordinates_by_buffer(buffer, point->not_compl[i]);
    memmove(buffer+get->start, done, strlen(done));
    memmove(buffer+get->start+strlen(done), buffer+get->start+strlen(not_done), file_size(fp) - (get->start+strlen(not_done)));    
    }
    fseek(fp, 0, SEEK_SET);
    fwrite(buffer, 1, file_size(fp), fp);
    ftruncate(fileno(fp), file_size(fp) - point->count2*(strlen(not_done) - strlen(done)));
    free(buffer);
}

int main(int argc, char **argv){
    char done[]="✅";
    char not_done[]="⚠️";

    if (argc == 2){
    if (strcmp(argv[1],"help") == 0 || strcmp(argv[1], "-h") == 0){
        help_sec();
    }
    else {
    FILE *fp = fopen("tasks.log","r+");
    if (fp == NULL){
        printf("Error 01: File cannot be opened.\n");
        return 1;
    }
    if (strcmp(argv[1], "organize") == 0){
        Line_Data *point = assign_nums(fp, done, not_done);
        organize(point, fp);
    }
    fclose(fp);
    }
    }

    else if (argc == 3){
    FILE *fp = fopen("tasks.log","r+");
    if (fp == NULL){
        printf("Error 01: File cannot be opened.\n");
        return 1;
    }

    if (strcmp(argv[1], "show") == 0 && strcmp(argv[2], "all") == 0){
        char c; 
        while ((c = fgetc(fp))!= EOF){
            putchar(c);
        }
        if (ferror(fp)){
            printf("File cannot be found.\n");
        }
    }
    else if (strcmp(argv[1], "add") == 0){
        Found* r = find_line(fp, argv[2]);
        if (r == NULL){
        add(not_done, fp, argv[2]);}
        else {
            printf("Task already added.\n");
            free(r);
        }
    }
    else if (strcmp(argv[1], "show") == 0){
        Found* w = find_line(fp, argv[2]);
        if (w != NULL){
            printf("%s\n",w->p);
            free(w);
        }
        else if (w == NULL){
            printf("Task cannot be found.\n");
        }
    }
    else if (strcmp(argv[1], "delete") == 0 && strcmp(argv[2], "all") == 0){
        fclose(fp);
        fp = fopen("tasks.log","w");
    }
    else if (strcmp(argv[1], "delete") == 0){
        Found *get = find_line(fp, argv[2]);
        if (get != NULL){
            Coordinates *i = get_coordinates(fp, get->position);
            remove_line(fp, get, i);
            free(i);
            free(get);
        }
        else {
            printf("Line cannot be found\n");
        }
    }
    else if (strcmp(argv[1], "done") == 0 && strcmp(argv[2], "all") == 0){
        Line_Data *p = assign_nums(fp, done, not_done);
        mark_all(fp, done, not_done, p);
        free(p);
    }
    else if (strcmp(argv[1], "done") == 0){
        Found* get = find_line(fp, argv[2]);
        if (get != NULL){
            if (strstr(get->p, done) == NULL){
            Coordinates *y = get_coordinates(fp, get->position);
            mark_as_done(fp, done, not_done, y);
            free(y);}
            else if (strstr(get->p, done) != NULL)
                printf("Task already marked as done.\n");
        free(get);
        }
        else {
            printf("Line cannot be found\n");
        }
    }
    fclose(fp);
    }
    else {
        printf("Invalid Command. Refer to -h for help.\n");
    }
    return 0;
}
