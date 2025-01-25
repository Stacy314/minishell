Minishell 🐚 \n
Minishell is a simple Unix shell written in C that supports command execution, 
redirections (<, >, >>), heredoc (<<) pipes (|), and built-in shell commands 
like cd, echo, pwd, env, export, and unset. It mimics the behavior of bash 
while following the 42 School project requirements.

🛠 Compilation
To compile the project, run:
bash$ make
#This will generate the executable ./minishell.

To clean object files:
bash$ make clean

To remove the executable and object files:
bash$ make fclean

To recompile everything:
bash$ make re

🚀 Running Minishell
Run the shell with:
bash$ ./minishell

You'll see a prompt like this:
minishell$  

Now you can enter commands just like in bash! 🎉

📌 Features
1️⃣ Command Execution
Minishell supports executing system commands such as:
🔹ls
🔹cat
🔹grep
🔹wc

2️⃣ Redirections (<, >, >>) and Heredoc (<<)
You can redirect input and output like this:
minishell$ echo "Hello" > file.txt       # Write to a file
minishell$ cat < file.txt                # Read from a file
minishell$ echo "More text" >> file.txt  # Append to a file
minishell$ cat << EOF                    # Enter multiple lines of input until 'EOF' is reached

3️⃣ Pipes (|)
You can use pipes to chain commands:
minishell$ ls -l | grep minishell | wc -l

4️⃣ Built-in Commands
Minishell implements the following built-in commands:

Command	Description
echo	- prints text to the terminal
cd - changes the current directory
pwd	- prints the current directory
export	- sets environment variables
unset	- removes environment variables
env	- prints the environment variables
exit	- exits the shell

Example:
minishell$ export MY_VAR="Hello"
minishell$ echo $MY_VAR          # Output: Hello
minishell$ unset MY_VAR

5️⃣ Signal Handling
Ctrl+C → Displays a new prompt
Ctrl+D → Exits the shell
Ctrl+\ → Does nothing
⚠️ Known Limitations
🔹 No support for wildcard expansion (*).
🔹 No support for advanced scripting features like for loops.
🔹 Limited error handling in edge cases.
🔹 No support ;, ', " and \.

👨‍💻 Developers
apechkov and mgallyam (42 Students)

📜 License
This project is part of the 42 School curriculum and is licensed under its academic policies.

Enjoy using Minishell! 🐚🚀
