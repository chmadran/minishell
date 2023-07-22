
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

```BUILD_DIR := ./build``` 
Defines the variable BUILD_DIR as the path to the build directory where all the objects will be stored (see below). The ./build path represents a directory named "build" in the current directory.

```OBJ_FILES	:= $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))```
OBJ_FILES is another variable that will store a list of object file paths. It uses the patsubst function to transform the source file paths (SRC_FILES) into corresponding object file paths within the BUILD_DIR directory. The patsubst function replaces the directory prefix $(SRC_DIR) with $(BUILD_DIR) and the file extension .c with .o for each file in SRC_FILES.



```TO THINK ABOUT```

- add dependencies to gain in efficiency (only compile the parts of the project that have changed)

<details>
<summary><h3>Some rules we could implement</h3></summary>
* only include libraries in ```minishell.h```
* therefore include ```minishell.h``` in each .c file
* all the error messages are declared in the ```exit.h``` file
* to be able to use the global variable add ```extern t_master	g_master;``` to .h files
* never push with leaks OR with norm errors
* open an issue if you see some commands that segfaults but is not urgent (or is a simple enhancement), using the format : current behaviour/expected behaviour
</details>

<h2>Step by Step </h2>

<details>
<summary><h3>Part 0: THE ENVIRONMENT</h3></summary>

Sometimes it is useful to communicate with a program in a semi-permanent way, so that you do not need to specify a command-line option every time you type the command to execute the program. One way to do this is to generate a configuration file, in which you can store data that will be used by the program every time it is run. This approach is typically useful if you have a large amount of data that you want to pass to a program every time it runs, or if you want the program itself to be able to change the data.

Environment variables are a small amount of data that needs to be passed to a program every time it runs but it is provided with a more lightweight approach to a configuration file. Environment variables, sometimes called shell variables, are usually set with the export command in the shell. Standard environment variables are used for information about your home directory, terminal type, and so on; you can define additional variables for other purposes. The set of all environment variables that have values is collectively known as the environment.

Environment variables are stored in a special array that can be read by your main function. Envp is an array of strings, just as argv is. It consists of a list of the environment variables of your shell, in the following format: NAME=value. Just as you can manually process command-line options from argv, so can you manually process environment variables from envp. However, the simplest way to access the value of an environment variable is with the getenv function, defined in the system header stdlib.h. It takes a single argument, a string containing the name of the variable whose value you wish to discover. It returns that value, or a null pointer if the variable is not defined.

Going back to how to get the environment variables if you're not allowed to use getenv like at 42, the main function can take zero to three parameters, the third of which is envp, an array of pointeurs to your machine's environment's variables that ends with a null pointer :  

 ```
int main( void )
int main( int argc, char *argv[] )
int main( int argc, char *argv[], char *envp[] )
 ```

After check, we realised the usage of a third argument like this ```int	main(int ac, char **av, char **envp)``` is not specified in the C standard or POSIX (see this stack overflow thread: https://stackoverflow.com/questions/10321435/is-char-envp-as-a-third-argument-to-main-portable). As a reminder, POSIX refers to a set of standardized functions, variables, and definitions that provide a consistent interface for developers to write software that can run on any POSIX-compliant operating system. A POSIX-compliant method to get all is the extern char **environ :

```
int	main(void)
{
	extern char	**environ; // this variable
	char		**env;

	env = environ;
	while (*env)
	{
		printf("%s\n", *env);
		env++;
	}

}
```
We need to get access and therefore store the environement variables in minishell because if the command inputted by the user is a builtin we have been asked to recreate, it will run our implementation of this function, otherwise it will look for the corresponding executable in the directories contained in the PATH environment variable. Therefore, at the beggining of our programm :  

- [ ] ```manage_environment``` is launched and calls the extern char **environ that is passed to a function that will fill a structure with its content
- [ ] in minishell.h file, you'll find the structure for the environment that has three values : char *name, char *value and a pointer to the next node s_env *next, this structure is itself accessible through the global master structure
- [ ] in env.c, the function ```manage_environment``` browses through the char **environ and sends each char * (e.g "USER=chmadran") to a node creator function ```create_add_env_node``` that takes name (USER) and value (chmadran) and the env_list we're filling as parameters.
- [ ] if the environment is empty when the program is launched, ```manage_empty_environment``` fills the shell level, working directory (PWD) and _ as these would be set automatically even if you tried to run bash without env variables e.g with ```env -i``` see https://unix.stackexchange.com/questions/48994/how-to-run-a-program-in-a-clean-environment-in-bash


Source : http://crasseux.com/books/ctutorial/Environment-variables.html
Source : https://github.com/mavileo/minishell-42
 </details>

<details>
<summary><h3>PART 1 : LEXER </h3></summary>
The goal of the lexer in minishell is to (i) exit if there is an incorrect input (e.g an open double quote or extra and/or invalid operators) otherwise it is (ii) to sort and store the input in a linked list so it can eventually be executed. This process is called the tokenisation.

<h4>1. GETTING A PROMPT</h4>

To collect the input, we use ```readline``` which takes the prompt as argument. For example, our minishell displays ```minishell: ``` when launched, ```readline``` then returns a char * we named ```input_text``` that is the user's input. ```input_text``` is passed as argument to the lexer ready for the tokenisation process. Readline is a function allowed by the subject but be careful as it leaks. From this point onwards, one needs to use the script ```valgrind.sh``` to actually test the program without encountering the readline leaks.  

At this stage, set up the signals so you can actually loop on the readline function until sending a CTRL+D to end the programme, or a CTRL+C to get a new clean prompt. As a reminder, SIGINT = CTRL +C and SIGQUIT = CTRL + D. We'll use the simple signal function here as we dont need any information that would be stored in the```sigaction``` structure we'd normally be expected to use. We only add a handler for `SIGINT` (CTRL+C) that clears the prompt, prints "^C\n" and opens a new prompt. For CTRL+D (`SIGQUIT`), we specify that the "handler" is `SIG_IGN` meaning signal ignore and we check whether the readline buffer is empty. If so, it means we sent a CTRL+D and we clean the memory before exiting the programme.

Things to note : 
* ```rl_catch_signals``` is used to control whether Readline should catch signals or let them be handled by the signal handlers defined in the programme. It is set to 0, which means that Readline is instructed not to catch signals. Instead, we have defined its signal handlers for specific signals, such as SIGQUIT and SIGINT.
* you *will* get a linker command failed error if you don't amend your Makefile at this stage to include the readline library, yes your Makefile. Simply add the following ```-lreadline``` to your include flags

</details>


<summary><h3>ERROR/EXIT HANDLING</h3></summary>

* we use the library errno.h to be able to use error codes

Using errno.h error codes provides a standardized and consistent way to represent and handle various errors that might occur during program execution. The errno.h header defines a set of macros that represent different error codes, and each code corresponds to a specific error condition. These error codes are typically returned by various library functions and system calls to indicate the nature of the error that occurred. For example, ENOMEM is an error code in Unix-based systems that indicates a failure to allocate memory (insufficient memory). It is defined in the <errno.h> header.

* the other strategy could be to use perror

Using perror provides a convenient and user-friendly way to handle and report errors in C programs. The perror function is part of the standard C library (stdio.h) and is used to print descriptive error messages to the standard error stream (stderr) based on the value of the global errno variable. For exmaple, the above code could be ```ft_error_exit("malloc (create_add_env_node)", ENOMEM)``` or more simply ```perror("malloc (create_add_env_node)")``` as the correct erno variable would be automatically found.


</details> 

