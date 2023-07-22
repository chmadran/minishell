
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

<details>
<summary><h3>Some rules we could implement</h3></summary>
* only include libraries in ```minishell.h```
* therefore include ```minishell.h``` in each .c file
* all the error messages are declared in the ```exit.h``` file
* to be able to use the global variable add ```extern t_master	g_master;``` to .h files
* never push with leaks OR with norm errors
</details>

<h2>Step by Step </h2>

<details>
<summary><h3>Part 0: THE ENVIRONMENT</h3></summary>



 <summary> <h3>PART 0 : ENVIRONMENT</h3></summary>


Chaque programme C possède une fonction principale qui doit être nommée main. La fonction main sert de point de départ pour l’exécution du programme. Elle contrôle généralement l'exécution du programme en dirigeant les appels à d'autres fonctions du programme. Il n’existe aucun prototype déclaré pour main et cette fonction peut être définie avec zéro, deux ou trois paramètres.  Le troisième paramètre, envp, est un tableau de pointeurs vers des variables d’environnement. Le tableau envp se termine par un pointeur null.

 ```
int main( void )
int main( int argc, char *argv[] )
int main( int argc, char *argv[], char *envp[] )
 ```

 Sometimes it is useful to communicate with a program in a semi-permanent way, so that you do not need to specify a command-line option every time you type the command to execute the program. One way to do this is to generate a configuration file, in which you can store data that will be used by the program every time it is run. This approach is typically useful if you have a large amount of data that you want to pass to a program every time it runs, or if you want the program itself to be able to change the data.

However, environment variables provide a more lightweight approach. Environment variables, sometimes called shell variables, are usually set with the export command in the shell. (This section assumes you are using the GNU Bash shell.) Standard environment variables are used for information about your home directory, terminal type, and so on; you can define additional variables for other purposes. The set of all environment variables that have values is collectively known as the environment.

Environment variables are stored in a special array that can be read by your main function. Envp is an array of strings, just as argv is. It consists of a list of the environment variables of your shell, in the following format: NAME=value.

Just as you can manually process command-line options from argv, so can you manually process environment variables from envp. However, the simplest way to access the value of an environment variable is with the getenv function, defined in the system header stdlib.h. It takes a single argument, a string containing the name of the variable whose value you wish to discover. It returns that value, or a null pointer if the variable is not defined.

We need to store this data in minishell because 1. if the command inputted by the user is a builtin we have been asked to recreate, it will run our implementation of this function, otherwise it will look for the corresponding executable in the directories contained in the PATH environment variable.

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

Therefore, at this stage :
- [ ] in manage_environment, call the extern char **environ and pass it to a function that will fill a structure with its content
- [ ] in minishell.h file, you'll find the structure for the environment that has three values : char *name, char *value and a pointer to the next node s_env *next, this structure is itself accessible through the global master structure
- [ ] in env.c, the function ```create_add_env_node``` browses through the char **environ and sends each char * to a node creator function named ```fill_environment```
- [ ] ```fill_envirnoment``` returns a new node for the env structure, with for example ```SHELL=/bin/zsh : char *name = "SHELL" char *value="=/bin/zsh"```


Source : http://crasseux.com/books/ctutorial/Environment-variables.html
Source : https://github.com/mavileo/minishell-42
 </details>



<!-- <details>
<summary><h3>ERROR HANDLING</h3></summary>
* to be completed
</details> -->

