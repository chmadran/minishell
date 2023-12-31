
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
* calloc not malloc   
</details>

<h2>Step by Step </h2>

One thing to note, this is not the final version and all functions using malloc are not protected. You should protect any function that uses malloc. Also, this version has a bug on redirections infile "<" at position 0.

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
* Finally, `add_history` is a function that is in the stdio library and it will allow to use the up and down arroy to check previous (non-empty) inputs as it is asked in the subject to have a working history

<h4>2. CREATING A TOKEN_LIST</h4>

Tokenization is the process of breaking a string (input line) into individual meaningful units called tokens. Tokens can represent different elements of the input, such as commands, arguments, operators, and symbols. The lexer recognizes specific patterns in the input and creates a linked list of tokens, where each node contains a token string and a token type.

Now, in the global structure `g_master`, we have a variable called `t_token *token_list;`. This is a pointer to a the token list. A token is composed of (i) a type that is OPERATOR OR COMMAND, (ii) a data (the command or NULL if the token is of type OPERATOR), and a pointer to the previous and to the next tokens of the list. We now need to fill this token_list with the user input that has been retrieved by the readline function and stored in `g_master.line_read`. 

We will check at this stage whether there is an uneven number of single or double quotes since we can just exit directly if so. We do this with `unclosed_quotes` but the specificities here are 1. that if we have a backslash before the quote`\'` then it is not interpreted as an opening/closing one but as a simple character (basically as anything but a quote) and 2. that if we have a backslash before a backslash before the quote `\\'` the the first backslash cancels the second and the quote IS considered as a quote and not a simple character. This applies to both single and double quotes.

Then we enter in the `manage_token` function that will create each token node and fill out the token list. `start_operator` starts by checking whether the first character of `line_read` is an operator as this would be an error in input. If it is a PIPE, it prints the predefined macro  "minishell: syntax error: unexpected token '%c'\n" (Error String Unexpected or ESTR_UNEXP) else if it's an operator of any other type is prints the predefined macro "minishell: syntax error near unexpected token 'newline'\n" (Error String Operator Start or ESTR_OPSTART). Else if it's a builtin it starts the token_list.

The two main functions of `manage_token` are `check_token_type` and `trim_spaces`. `check_token_type` browses through the operators we need to handle's list and if one is found associates the type to the operator enum. If no operator is found, the type is COMMAND. To make enums clearer look at the minishell.h file and see how COMMAND = 0, PIPE = 1 etc. So type = 1 = T_PIPE :) `trim_spaces` on the other hand returns a char * data that will fill out the token, if the token is an operator that data is set to NULL.

The final checks at this stage are `is_heredoc_pipe` that checks whether there's a pipe followed by nothing which would open a heredoc pipe but in our program simply displays an error message and opens a new prompt; and `is_clean` that checks whether there are two different or similar operators following each other (so operator, space, operator) which is an invalid input.

If `launch_lexer` exits successfully, we move on to the execution.

Things to note : 
* use `print_token_list` to test the output of the token_list, maybe thats where your error lies
* the function `is_in_quotes` allows us to check whether an input is within quote so that its counted as part of a single "argument". For example, a pipe within a quoted argument is part of the string it belongs to, it doesnt create a new token.

</details>

<details>
<summary><h3>Part 2: THE EXECUTION</h3></summary>

<h4>1. PREPARING FOR EXECUTION</h4>

The exec launcher function called `launch_execution` starts by preparing the execution.

<h4>1.1 PREPARING FOR EXECUTION</h4>

`create_arguments` creates an argument under the format int argc, char **argv. To create the char **, we malloc by the number of spaces (that have been trimmed previously so no risk of extra ones) and then `split_args` fills out the argv from the token data. Then argc is just the number of char * in argv.

Things to note are that we must be careful of quoted/unquoted arguments, and we implemented special rules for backslash. The function `clean_arg` cleans the final argv before it is stored (even reallocating the right amount of memory). At this stage, handling backslashes makes the code much more complex, we might want to remove these parts. 

<h4>1.2 EXPANDING THE INPUT</h4>

At this stage, the function `launch_expansion` will start looking for `$` characters. If one is found then a substring of the corresponding argv is sent to an expansion processer, that's `process_expansion_replace`. If the character following the dollar sign is a question mark `?` then it means the expansion will replace argv[i] (`$?`) by the exit code found in the global structure master. If no question mark is found right after the dollar sign, then we are looking for an environment variable matching the string following the dollar sign, until the next space, for example "$USER fenkfnek" here we will look for the `USER` environment variable. If found, we replace `argv[i] = $USER fenkfnek` by `argv[i] = "chmadran fenkfnek` that is the environment variable found. Else we replace with nothing !

<h4>1.3 BUILTIN OR COMMAND ?</h4>

We start by look at argv[0] in `find_arg_type` and assigning it a type following an enum you'll find in`minishell.h`. It can be either one of the builtins we had to recreate ELSE it is a type OTHER (potentiallya command / builtin we didnt have to recreate) or an ERROR.  Then we `execute_command_or_builtin`, indeed if the type is ERROR we start by handling it by printing the correct error message and assigning the exit_code to 127. Now if the type is not ERROR and not OTHERS it means its a builtin we had to recreate. We send our exec arg to `execute_builtin` and then to the correct recreated builtin function (cd, echo, env, export, exit, pwd or unset). Finally, if the type is neither ERROR nor a known BUILTIN then it's OTHERS. The exec arg is sent to `execute_command` that will `search_path_command` check if the command sent as argv[0] is an existing pathname under the environment variable `PATH`. If not, the path is set to DEFAULT_PATH_1 (a standard set of directories where system executables are usually located) and (DEFAULT_PATH_2: This is an additional default directory path).

Else, it then splits the PATH string into an array of individual paths using ft_split, which splits the string based on the delimiter :. This will give an array (paths) containing individual directories where the command might be located. The function then iterates through each path in the paths array and appends the command name to the path to create a full pathname. It checks if the file with the generated pathname exists and is executable (X_OK indicates executable permission). If it finds an executable file, it returns the full pathname. If the command is not found in any of the directories in the PATH or default paths, it returns NULL, indicating that the command was not found.

Overall, this mechanism allows the code to search for the command in different directories specified in the PATH or fall back to the default directories if the PATH environment variable is not available or not set. This ensures that the shell can find and execute common commands, even when the PATH variable is not explicitly set in the environment.

Things to note :   
*  When we check if an entry is a directory, the correct type for representing a directory stream in C is DIR, and it is typically used in conjunction with the `opendir`, `readdir`, and `closedir` functions from the `<dirent.h>` header
*  An alternative to check if an entry is a directory is to use the `struct stat` is a C structure defined in the `<sys/stat.h> header (or <sys/types.h> and <sys/stat.h> headers on some systems).` It is commonly used to store information about files and file systems. This structure is filled with file system metadata retrieved by functions like stat, lstat, fstat, etc. In this case, wecould have used the S_ISDIR macro which is a conditional macro provided in `<sys/stat.h>` that checks whether the st_mode member of the struct stat indicates that the file is a directory. It evaluates to a non-zero value (true) if the file is a directory and 0 (false) otherwise. For readability, we decided to use a simple `is_directory` function.
* The `access` function in C is used to check the accessibility of a file with respect to the permissions of the calling process. It is declared in the `<unistd.h>` header. The second argument, mode, is an integer representing the mode of access you want to check. One of the possible values for the mode argument is X_OK, which is used to check if a file is executable (has the execute permission bit set) by the calling process.


<h4>1.4. CODING THE REQUIRED BUILTINS</h4>

The subject of minishell asks us to implement some builtins : `cd, echo, env, export, exit, pwd and unset`. Therefore, we must recode them. Very concisely...

<h5>CD</h5>

The CD builtins if sent without any argument simply searches for the environment variable "HOME" and returns to its path, so to the home directory. You can use `chdir()` which is a system call and a C library function used to change the current working directory of a process. It stands for "change directory." The current working directory is the base directory from which all relative pathnames are resolved. If the HOME directory is valid then the environment variable PWD ("Present Working Directory") is updated. If an argument is given to the `cd` buitlins then we check if its a valid directory and if so the PWD environment variable is updated to the value of that new directory and chdir changes it. 

<h5>ECHO</h5>

We are asked to recreate the `echo` builtins with a potential `-n` flag. So here we use a flag to check if `-n` is present in the input, ifso the flag is set to true, else it stays at false. Then we simply print the arguments of echo on the regular STDOUT, and with a space between each word. If there is no flag `-n` we add a newline after the word, else we dont.

<h5>ENV</h5>

Simply prints the environment list.

<h5>EXIT</h5>

Simply clean the programm and use `exit` to leave it. 

<h5>EXPORT</h5>

If only `export` is sent as argument, we print each variable of the environment list preceded with theword `export` in ALPHABETICAL ORDER (dont rearrange the environment variables altogether). If an argument is sent to export, we check if (1) its an existing environment variable, if so and its syntaxed a follows NAME=VALUE, we replace the current value of the environment variable found NAME by the new VALUE; (2) else we just add it to the environment variable lists, even if the value is empty.

<h5>PWD</h5>

We use `getcwd()` which is a C library function and system call used to get the current working directory of a process in a Unix-based operating system to print the working directory. Indeed, the name of the function stands for "get current working directory." The current working directory is the base directory from which all relative pathnames are resolved. It takes two parameters :
	* buf: A pointer to a buffer where the current working directory path will be stored.  
	* size: The size of the buffer provided (buf).  
 
<h5>UNSET</h5>

`Unset` is used to remove environment variables. So we browse through the environment variable and if found we remove it. 

</details>

<details> 
<summary><h3>Part 3: DEBUGGING :cowboy_hat_face: </h3></summary>

We used a [spreadsheet](https://docs.google.com/spreadsheets/d/1ejFaynqpGdesM2IBUPgswXOv1Vcysf9ilns25Ik5TIY/edit?usp=sharing) that is a cleaned version of @vietdu91's [minishell bible](https://docs.google.com/spreadsheets/d/1uJHQu0VPsjjBkR4hxOeCMEt3AOM1Hp_SmUzPFhAH-nA/edit#gid=0), in order to browse through all the tricky stuff and make sure our project covered them. Since the subject is not very explicit about the matter, i'd say that having the correct **error messages** is your choice but definitively make sure **exit codes** are set as expected.

<h4>THE FLAG -N</h4>

You probably will not have treated the flag `-n` correctly, here are additional rules to implement : 
*  The `-n` option must come before any string arguments, i.e., it must be the first argument after echo.  
*  Multiple `-n` options are allowed and treated as one, i.e., `-nn`, `-nnn`, etc., are valid and work the same as `-n`.  
*  There must not be any other character after the `-n` or `-nn`, `-nnn` etc. in the option string. If there is, it's treated as an argument to `echo` and not an option. So `-na` would not be treated as the `-n` option, but as an argument to print.  
*  The `-n` option, if used, prevents a trailing newline from being printed.  
*  All other arguments are treated as strings to be printed, separated by spaces.  
So based on this code, `echo -n -n hello` would output `hello` without a trailing newline, `echo -na hello would` output `-na hello` with a trailing newline, and `echo hello world` would output `hello world` with a trailing newline.  


<h4>FURTHER BUILTINS INPUT CHECK</h4>

Some checks you might not have included for the input of your builtins : 
*  `not a valid identifier` : If the input doesnt start with a letter or an underscore, or of the string contains a special character(`@`, `?` etc), applies to `export` and `unset` (basically to environment variable names) and exit code is 1. 
*  `invalid option` : if the input contains `-` at argv[0], applies to `cd` and `export` and `pwd`, the exit code is 2.
*  `event not found` : if the input contains `!`, applies to `export` and `unset`, the exit code is 1. 


<h4>INTERNAL VARIABLE UID</h4>
We noticed in a Minishell GSheet checker that there was a check of the return value of $UID. We decided NOT to include the $UID variable since the functions to retrieve it are not allowed. Here is why. The $UID variable in Unix-like systems (when you're using a shell like Bash or Zsh) is an internal shell variable, not an environment variable. This is why you do not see it when you print all the environment variables with env or accessing them through extern char **env in a C program. Internal shell variables are used by the shell to keep track of certain states or settings. They are not automatically exported to the environment of child processes, which is why they're not seen when you print the environment variables.

You can access the value of the $UID variable in a shell script or at the shell prompt, but it will not be visible to programs run from that shell, unless it's exported explicitly. However, $UID is typically not exported because it's set and managed by the shell, and applications can get the same information via system calls like getuid or geteuid in C or similar functions in other languages. If you want to see the user ID in a C program, you would use the getuid() or geteuid() function, which return the real and effective user ID, respectively. 


<h4>EXPANSION's TRICKS</h4>

	TO VERIFY/CHECK IN CODE: Quoting can affect variable expansion. Single quotes ' prevent any expansion, while double quotes " allow variable expansion but preserve whitespace. Also, the backslash `\` before the dollar sign prevents variable expansion. When the shell encounters a variable name followed by a special character like !, @, #, $, ^, &, *, (, ), -, =, +, [, {, etc, it treats the special character as a literal character and includes it in the output after expanding the variable. This behavior applies to any special character that is not part of a valid variable name character set (letters, digits, and underscores). So for example, `echo $:$= | cat -e` results in `$:$=$` because $: and $= are not replaced by an empty string but treated as literal strings, meaning the shell won't try to replace them with the value of a variable. The cat -e command is used to display the incoming data and will append $ at the end of each line. So the final command echo $:$= | cat -e in POSIX mode will output : $:$=$


<h4>EXPORT & UNSET SUBWORLDS</h4>

When you input `export VARIABLE` without assigning it a value, that variable does NOT become an environment variable BUT is in the **export list** when you print the environment variables using `export`. What's up?? See `add_back_envp_var` but basically we created a copy of the environment variables that is accessible through `g_master` called `char **export_envp`. this is where a variable added to the environment variables through export gets added if it doesnt have a value yet (cf line 199 gsheet).

	TODO:Update the export sub envronment char ** if anything is added to the real environment variables while the programme is ongoing. 

<h4> HANDLE BINARY FILES </h4>

You're supposed to be able to enter the pathname of a buitlin (i.e binary file) and it should compile, for example the output of `/bin/echo Hola Que Tal` should be `Hola Que Tal`. To handle binary files i.e inputs like `/bin/env`, `/bin/cd` etc, add a check in your pathname filer, if there is no pathname found but you have `access` to exec->argv[0] then check if its a valid directory and if so store it as the pathname see `is_directory` in `prepare_command`.

</details> 


<summary><h3>ERROR/EXIT HANDLING</h3></summary>

* we use the library errno.h to be able to use error codes

Using errno.h error codes provides a standardized and consistent way to represent and handle various errors that might occur during program execution. The errno.h header defines a set of macros that represent different error codes, and each code corresponds to a specific error condition. These error codes are typically returned by various library functions and system calls to indicate the nature of the error that occurred. For example, ENOMEM is an error code in Unix-based systems that indicates a failure to allocate memory (insufficient memory). It is defined in the <errno.h> header.

* the other strategy could be to use perror

Using perror provides a convenient and user-friendly way to handle and report errors in C programs. The perror function is part of the standard C library (stdio.h) and is used to print descriptive error messages to the standard error stream (stderr) based on the value of the global errno variable. For exmaple, the above code could be ```ft_error_exit("malloc (create_add_env_node)", ENOMEM)``` or more simply ```perror("malloc (create_add_env_node)")``` as the correct erno variable would be automatically found.

* returning EXIT_SUCCESS or EXIT_FAILURE
EXIT_SUCCESS and EXIT_FAILURE are symbolic constants defined in the C programming language, typically found in the stdlib.h header file. They are used to indicate the success or failure status of a program when it exits. These constants are commonly used with the exit() function to terminate a program and provide an exit status to the calling environment. EXIT_SUCCESS has a value of 0 while EXIT_FAILURE has a value of 1.

* using defined EXIT_messages
The syntax : `# define ESTR_QUOTE "minishell: syntax error: unmatched quote\n"` in the .h files are `preprocessor macros` defined using the C preprocessor directive #define. It is essentially a text substitution mechanism, where the macro ESTR_QUOTE is replaced with the corresponding string value "minishell: syntax error: unmatched quote\n" wherever it appears in the code. It can then be used as follows in the code : ` printf(ESTR_QUOTE);`

