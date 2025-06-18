# Minishell  
*As beautiful as a shell – a 42 school project*

## 📌 Summary

**Minishell** is a simple Unix shell implemented in C. This project is part of the curriculum at [School 42], designed to give students hands-on experience with process control, file descriptors, parsing, and command execution – all essential components of a modern shell.

## ⚙️ Features

- Custom shell prompt and command line interface  
- Command execution with support for:
  - Environment variable expansion (`$VAR`, `$?`)
  - Quoting (`'`, `"`) and parsing
  - Built-in commands:  
    - `echo [-n]`  
    - `cd`  
    - `pwd`  
    - `export`  
    - `unset`  
    - `env`  
    - `exit`
- Redirection operators:
  - `<`, `>`, `>>`, `<<` (heredoc)
- Piping with `|`
- Signal handling:
  - `Ctrl-C`, `Ctrl-D`, `Ctrl-\`
- Minimal use of global variables (only one allowed for signal handling)

## 🧠 Learning Objectives

- Process creation (`fork`, `execve`, `wait`)
- File descriptor duplication (`dup`, `dup2`)
- Signal management (`signal`, `kill`, `sigaction`)
- Memory management (no leaks allowed)
- Terminal capabilities (`readline`, `termcap`)
- Manual parsing and lexical analysis
- Makefile usage with standard rules

## 🛠️ Technologies

- **Language**: C  
- **Libraries**:  
  - [`readline`](https://tiswww.case.edu/php/chet/readline/rltop.html)  
  - Custom [`libft`](https://github.com/Stacy314/libft)  
- **Tools**:  
  - `Makefile`  
  - `GCC` with flags: `-Wall -Wextra -Werror`

## 🚀 Getting Started

### 1. Clone the repo
```bash
git clone https://github.com/your-username/minishell.git
cd minishell
```
### 2. Build the project
```bash
make
```

### 3. Run the shell
```bash
./minishell
```

### 4. Enjoy our shell 👾

✅ Status
✔️ Mandatory part fully implemented
🔧 Bonus features (if any) in progress
