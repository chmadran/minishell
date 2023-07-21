
# <h1 align = "center"> minishell </h1>

<p align="center"> This README documents the completion of the 42Paris minishell project.</p>

> The existence of shells is linked to the very existence of IT. At the time, all developers agreed that communicating with a computer using aligned 1/0 switches was seriously irritating.

> It was only logical that they came up with the idea of creating a software to communicate with a computer using interactive lines of commands in a language somewhat close to the human language.

> The minishell project is about creating our own shell based on bash. It implements redirections and pipes, as well as environment variable expansions and the cd, echo, env, exit, export, pwd and unset builtin commands.

<h2> Project Requirements </h2>

<p align = "center">
<img width="616" alt="Screenshot 2023-05-08 at 08 29 11" src="https://user-images.githubusercontent.com/113340699/236750719-1f037981-0d76-4674-9a91-bdae3deaae1a.png">
</p>

<details>
<summary><h3>Expected shell behavior and limitations <i>(+ click to expand)</i> </h3></summary>


* Display a **prompt** when waiting for a new command.
* Have a working **history**.
* Search and launch the right executable (based on the PATH variable or using a relative or an absolute path).
* Not use more than **one global variable**. Think about it. You will have to explain its purpose.
* Not interpret unclosed quotes or special characters which are not required by the subject such as \ (backslash) or ; (semicolon).
* Handle ’ (single quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence.
* Handle " (double quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence except for $ (dollar sign).

* Implement **redirections**:
	◦ < should redirect input.
	◦ > should redirect output.
	◦ << should be given a delimiter, then read the input until a line containing the delimiter is seen. However, it doesn’t have to update the history!
	◦ >> should redirect output in append mode.
* Implement **pipes** (| character). The output of each command in the pipeline is connected to the input of the next command via a pipe.
* Handle **environment variables** ($ followed by a sequence of characters) which should expand to their values.
* Handle $? which should expand to the exit status of the most recently executed foreground pipeline.
* Handle ctrl-C, ctrl-D and ctrl-\ which should behave like in bash.
* In interactive mode:
	◦ ctrl-C displays a new prompt on a new line.
	◦ ctrl-D exits the shell.
	◦ ctrl-\ does nothing.

* Your shell must implement the following **builtins**:
	◦ echo with option -n
	◦ cd with only a relative or absolute path.
	◦ pwd with no options
	◦ export with no options
	◦ unset with no options
	◦ env with no options or arguments
	◦ exit with no options

The readline() function can cause memory leaks. We don’t have to fix them.
</details>

<h3>Usage</h3>

* Clone the repository ```git@github.com:chmadran/minishell.git```  
* Compile with make ``` cd minishell && make```  
* Run the executable : ```./minishell```  
* OR if you want to check out leaks run ```./valgrind.sh```  

A prompt appears, now enter a command to be executed

<h3>Makefile</h3>

Some specificities about the makefile : 

```SRC_FILES	:= $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/**/*.c)```

SRC_FILES is a variable that will store a list of source file paths. It uses the wildcard function to expand patterns and find all the .c files within the SRC_DIR directory and its subdirectories. The ** notation is used to match files in subdirectories recursively. So, $(SRC_DIR)/**/*.c matches all .c files in SRC_DIR and its subdirectories.

```OBJ_FILES	:= $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))```
OBJ_FILES is another variable that will store a list of object file paths. It uses the patsubst function to transform the source file paths (SRC_FILES) into corresponding object file paths within the BUILD_DIR directory. The patsubst function replaces the directory prefix $(SRC_DIR) with $(BUILD_DIR) and the file extension .c with .o for each file in SRC_FILES.

```DEP_FILES	:= $(OBJ_FILES:.o=.d)```

DEP_FILES is a variable that will store a list of dependency file paths. It takes the OBJ_FILES list and replaces the file extension .o with .d. Dependency files (.d files) are used to track the dependencies between source files and headers to ensure that changes in headers trigger recompilation of dependent source files.

The use of .d files is common in C/C++ projects with Makefiles because they allow Make to automatically rebuild only the necessary parts of the project when a header file changes, saving compilation time.

```TO THINK ABOUT```

- add dependencies 
- add build folder 
