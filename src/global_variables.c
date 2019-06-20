/**
 * Definition of global variables.
 * 
 * To use them in a file, declare as:
 *  - extern <type> <variable_name>;
 * 
 * @author Guilherme Sartori
 * @author Marlize Ramos
 * @author Renan Kummer
 */

// Holds the current working directory path.
char working_dir_path[10000];

OpenFile openFiles[MAX_OPEN_FILES];		// vetor de estruturas do tipo struct t2fs_openfile
