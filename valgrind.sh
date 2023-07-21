#!/bin/bash
valgrind --suppressions=leaks_ignore.supp --leak-check=full --show-leak-kinds=all ./minishell
