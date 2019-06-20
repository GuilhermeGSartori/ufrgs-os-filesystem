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

// Array of sructures containing open files.
OpenFile openFiles[MAX_OPEN_FILES];

// Array of sructures containing open directories.
OpenFile openDirs[MAX_OPEN_DIRS];
