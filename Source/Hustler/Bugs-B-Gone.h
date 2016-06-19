// Bugs-B-Gone for C++ and BeOS
//
// Version 1.2 (Sunday, 14 March 1999)
//
// Created by Matthijs Hollemans 
// mahlzeit@bigfoot.com
// http://home.concepts.nl/~hollies/
//
// Bugs-B-Gone is public domain. No warranties expressed or implied.
//
// This package offers a few simple macros that allow you to write debugging 
// output to either stdout or the serial port. Nothing special, but it works 
// for me. For more "internal debugging" power, you may wish to look at Fred
// Fish's DBUG package.
//
// Typically, you would put something like this at the top of your main 
// source (or include) file:
// 
//   #define  BUGSBGONE
//   #define  BBG_PREFIX "MyApp"
//   #undef   BBG_SERIAL
//   #include "Bugs-B-Gone.h"
//
// The various symbols have the following meaning:
//
//   BUGSBGONE            -- Enables or disables the Bugs-B-Gone package.
//
//   BBG_PREFIX           -- Defines the text that is prepended to all
//                           debugging output, typically the name of your
//                           application. If you don't define BBG_PREFIX 
//                           yourself, it defaults to "Bugs-B-Gone".
//
//   BBG_SERIAL           -- Enables or disables serial debugging. If serial
//                           debugging is turned on, then all debugging output
//                           is sent to the serial port. See the BeBook and Be
//                           Newsletters for more details on serial debugging.
//
// Make sure that the "Bugs-B-Gone.h" file is included _before_ any other
// header files, so it is the first to load Be's Debug.h include. Otherwise,
// the Bugs-B-Gone macros won't work. You have been warned :-)
//
// Meet your slaves, oh master:
//
//   BBG_WRITE((Args))    -- Outputs the message given by Args. The argument
//                           takes the variable argument form of a printf()
//                           call and must be wrapped inside a second set of
//                           parenthesis; for example:
//                           BBG_WRITE(("Hi %s", "there!"))
//
//   BBG_ASSERT(Args)     -- Outputs the name and line of the current source
//                           code file if the condition given by Args (which
//                           can be any normal C or C++ expression) evaluates 
//                           to FALSE, but doesn't halt execution of the
//                           program.
//
//   BBG_CHECKPOINT       -- Outputs the name and line of the source code 
//                           file that contains this checkpoint.
//
//   BBG_ENTER(FuncName)  -- Outputs a string that indicates that the code 
//                           has just entered a particular function.
//
//   BBG_LEAVE(FuncName)  -- Outputs a string that indicates that the code 
//                           is about to leave a particular function.
//
// You don't need to separate these macros from other statements with a
// semicolon.

#ifndef BUGSBGONE_H
#define BUGSBGONE_H

#ifndef BBG_PREFIX
#define BBG_PREFIX "Bugs-B-Gone"
#endif

#ifdef BUGSBGONE

// Turn on BeOS debugging.
#define DEBUG 1
#include <Debug.h>

#ifdef BBG_SERIAL
#define BBG_WRITE(Args) {SERIAL_PRINT(("%s ",BBG_PREFIX));SERIAL_PRINT(Args); SERIAL_PRINT(("\n"));}
#else // BBG_SERIAL
#define BBG_WRITE(Args) {PRINT(("%s ",BBG_PREFIX));PRINT(Args); PRINT(("\n"));}
#endif // BBG_SERIAL

#define BBG_ASSERT(Args) {if (!(Args)) {BBG_WRITE(("Assertion of %s failed at line %d of %s",#Args,__LINE__,__FILE__));} }
#define BBG_CHECKPOINT {BBG_WRITE(("Checkpoint at line %d of %s",__LINE__,__FILE__));}
#define BBG_ENTER(Args) {BBG_WRITE(("%s Entering...",Args));}
#define BBG_LEAVE(Args) {BBG_WRITE(("%s Leaving...",Args));}

#else // BUGSBGONE

#define BBG_WRITE(Args)
#define BBG_ASSERT(Args)  
#define BBG_CHECKPOINT  
#define BBG_ENTER(Args)
#define BBG_LEAVE(Args)

#endif // BUGSBGONE

#endif // BUGSBGONE_H
