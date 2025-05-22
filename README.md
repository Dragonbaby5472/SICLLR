# SIC Linking Loader
A simple LLR for SIC.
## Program Functionality Overview

In this Linking Loader, I have designed it to operate concurrently in both **command line mode** and **interactive mode**.

### Command Line Mode

**Usage:**
```
SICLLR objf1 objf2 -o img_file -r rpt_file -l log_file -link
```

- `objf1, objf2, …`: All object code files to be linked.
- `-o img_file`: Sets the name of the output image file; if not used, the default output is `DEVF2`.
- `-r rpt_file`: Sets the name of the output report file; if not used, the default output is `report.txt`.
- `-l log_file`: Sets the name of the output log file; if not used, the default output is `SICLLR.log`.
- `-link`: If this option is provided, the program will immediately perform the linking process; if omitted, it will enter interactive mode waiting for further instructions.

### Interactive Mode

Regardless of whether commands are issued via the command line, the program will enter interactive mode to allow the user to issue additional commands. The program provides a `SICLLR_shell` for the user to execute the following commands:

- `help`               : Displays help information for commands.
- `exit`               : Terminates the program.
- `load filename...`   : Loads object files into the linking list.
- `remove filename...` : Removes an object file from the linking list.
- `clear`              : Clears all object files in the linking list.
- `list`               : Lists all object files in the linking list.
- `link`               : Links the object files in the linking list.
- `img filename`       : Sets the link image file name.
- `report filename`    : Generates a report with the specified filename.

All operations and messages in interactive mode are recorded in the log file.

---

## Program Error Reporting Function

This program includes a series of error messages, which are divided into **operational errors** and **linking errors**.

### Operational Errors

These errors occur during command execution or file reading and include the following:

**A.** Linking cannot be executed because no object code has been loaded.  
**B.** Unknown command, possibly due to a typo.  
**C.** No filename provided when using commands such as `load`, `remove`, `img`, or `report`.  
**D.** The filename provided for the `remove` command does not exist in the list of loaded files.  
**E.** The specified file cannot be read during the load operation.  
**F.** A file containing a `main` function is provided more than once.  

### Linking Errors

**A.** No object code has been loaded.  
**B.** No `main` function exists.  
**C.** No T record contains the address referenced by an M record.  
**D.** A symbol used by the program has not been defined or referenced.  
**E.** The operator used in an M record is not either `+` or `-`.  
**F.** The symbol used in an M record has not been defined.  
**G.** Duplicate program names.  
**H.** Duplicate symbol names.  
**I.** Unrecognizable record type.  
