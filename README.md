# Tasky
Tasky is a TODO-List written in C. It's minimalistic, and can be operated by command line arguments.

## Features
1. The user can add the tasks he or she wants to add.
2. User can delete those tasks, either separately by giving the task name, or can
delete all of the tasks at once.
3. There is a timestamp in each of the tasks written by tasky so that user will get an idea that when
they created that task.
4. User can see which tasks are completed and which are not, by using organize command.
5. Cool icons, a tick mark, and an error icon is added to the tasks which are done, and to the tasks which
are not completed yet.

## Requirements
As tasky is a minimal program, it doesn't require much other than a compiler into your computer.
The GNU Compiler Collection, or Clang are recommended to compile my software.

## Initial Setup
1. Clone the git repository
```bash
git clone https://github.com/princega/tasky.git
```

2. Enter into the cloned git repository
```bash
cd tasky/
```

Now, you have to choose whether to use my own executable or create a new one.
If you want to run my own executable itself, then
```bash
cd bin/
./tasky.out
```
or
3. Compile the source code file
```bash
cd src/
gcc tasky.c -o executable_name.extension
```

4. Run the executable
```bash
./executable_name.extension -h
```
-h will print the help menu needed for the first time.





