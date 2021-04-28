/**
 * @file msg_type.h
 * @author Hikari Hashida
 * @brief Command enum type file.
 * @version 0.1
 * @date 2021-04-28
 * 
 * last visited: 2021-04-28
 */


/*
    Command has two major types: 
        1. Command with data structure.
        2. Command without data.
    More commonly command will contain data structure, however some commands
    e.g. emergency HALT command do not have data structure.
    CMD_NUM_DATA is there to keep count of number of command with data structures.
    Make sure CMD_NUM_DATA is placed AFTER the last command with data structure, and
    BEFORE the no-data commands. This is important, as it utilises enum property of starting
    from 0 and increments by 1. CMD_NUM_DATA is used to decide array size of storing data
    structure information for serial handler.
    ===============
    Commands with data struct
    ...
    --------
    CMD_NUM_DATA
    --------
    No data commands
    ...
    ================
 */

enum Command {
    // Command with data struct
    CMD_LED,
    // Number of command with data struct.
    CMD_NUM_DATA,
    // Command with no-data
    CMD_HALT,
    CMD_NULL
};
