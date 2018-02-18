# 
#   Course: CS 3224 - Operating Systems
#   Name: David Zheng
#   Assignment #2: Guessing Game
#   Due Date: Feb 22, 2018
#

.code16 # use 16bit assembly
.globl start # indicate starting point for execution

# Our "main()"
# Should be in a loop 
# checks if user entered correct number if guess and number both are equal 
# Could SUB the two registers and see if result is 0
# jmp to start if not zero
# otherwise display correctMsg and go into infinite loop

# GRPs; ax,bx,cx,dx. (ax is mostly used for the arguments for interrupts)
# w: 16 bits, b: 8 bits, l: 32 bits, s: 16 bits/32bits FP #

# let %bl be the register to hold the random number
# let %bh be the top high to hold the guess
# let %cx,ch,cl be a temp register to hold values
start: # start will setup things I need
    # movb $0x02, %bl # move the number to guess into %bl
    jmp _getRandomNumber
    _backStart:
    movb $0x00, %ah # set video mode
    movb $0x03, %al # select the 80x25 text mode
    int $0x10 # calls BIOS interrupt #10, sets up the display
    jmp _gameLoop

# Reads the seconds on the CMOS RTC registers and 
# put a number in the range from 0 - 9 into %bl
# %bl will hold the random number
# No need to loop
_getRandomNumber:
# out: source, dest. source (only %al,ah,ax) sent to I/O at dest (anything)
# in: source, dest. source (anything) read data and sent to dest (%al,ah,ax)   
    movb $0x00, %al # only %al,ax,ah can be used as destination
    outb %al, $0x70 # selecting CMOS register 0x00 at I/O port 70
    inb $0x71, %al # only %al,ax,ah can be used as a destination
    movb %al,%bl

    # %bl holds 1 byte representing the secs
    # from the CMOS RTC clock
    # restricting range. assuming bl has 1 bytes
    andb $0x0F, %bl  # First, get rid of the leftmost byte
    # cmp: compares dest < source. not other way
    cmp $0x09,%bl # sub if %bl is greater than 9 in hex
    jl _underNine
    # by sub 6, we bring the number back to
    # range between 0 - 9. since largest is 0x0F (15)
    # after we zeroed the 4 leftmost bits
    sub $0x06, %bl 
    _underNine:
    jmp _backStart

# Keeps prompting the user if guess is not right
_gameLoop:
    movw $_userPromptMsg, %si # move offset for _userPromptMsg to %si
    call _printString
    
    # read user input ====================
    movb $0x00, %ah # sets up to read from keyboard
    int $0x16 # calls BIOS to read from keyboard and puts char in %al
    
    # print out what user pressed to give indication
    movb $0x0E, %ah # set mode to print
    int $0x10 # calls BIOS service to print char, in %al

    # store the user input before making newline
    # Note: No stack during the bootloader (Need to verify)
    movb %al,%cl # %cl would act as a temp register

    # newline ===================================
    # _printNewLine: will modify %ah, %al
    call _printNewLine

    # load back user's input from %cl
    # Note: %cl will act as a temp location that can be overwritten
    # David's convention
    movb %cl,%al

    # check guess
    # Note: '0' in hex is 0x30
    sub $0x30,%al # offset for integer hex representation
    cmp %al,%bl # note %al has hex represensation of the char
    jz _printRight # if correct, display correct and finish
    _printWrong: 
        movw $_wrongMsg, %si
        call _printString
        call _printNewLine
        jmp _gameLoop # prmpt the user again
    _printRight: # user got the guess
        movw $_correctMsg, %si
        call _printString
        call _printNewLine
        # jmp _endGame

    _endResult: # prints end game results
        movw $_endGameMsg, %si
        call _printString # prints the string at %si
        movb %bl, %al # the char to be printed
        # since %bl holds 0x00 to 0x09 for the guessing number
        add $0x30, %al # offset for ASCII value
        movb $0x0E, %ah # set mode for 0x10
        int $0x10 # BIOS interrupt to print
        call _printNewLine # moves cursor to new line

    _endGame: jmp _done

# Prints out the characters on the %si register and out to the display buffer
_printString:
    # think of %si as a char* to a string
    lodsb # load single byte from (%si) and put in %ai, increments %si
    testb %al, %al # check for null char
    jz _retToCaller # if null char, return to _gameLoop
    movb $0x0E, %ah # select teletype mode to write to display
    int $0x10 # call BIOS to print from $al
    jmp _printString # loop back if still has chars to print

_printNewLine:
    movb $0x0d, %al # *carriage return*, moves cursor to front of line
    movb $0x0E , %ah # set mode to write out (redundant but ensure)
    int $0x10 # calls to BIOS interrupt to print a char from %al

    movb $0x0a, %al # *line feed*, moves cursor down one row
    movb $0x0E, %ah # set mode to write out
    int $0x10 # calls BIOS to print char from %al
    ret # return to caller

# invokes a ret call, acting like a "return" for a function
# Used for wanting to return in the middle of the function
_retToCaller:
    ret 

# User can restart by pressing "R"
_done:
    movw $_restart, %si
    call _printString
    call _printNewLine
    # read user input ====================
    movb $0x00, %ah # sets up to read from keyboard
    int $0x16 # calls BIOS to read from keyboard and puts char in %al
    cmp $0x52, %al
    call _getRandomNumber
    jz _gameLoop
    jmp _done

# data / content:
_wrongMsg:
    .string "Wrong!"

_correctMsg:
    .string "Right! Congratulations."

_endGameMsg:
    .string "The number was: "

_userPromptMsg:
    .string "What number am I thinking of (0-9)? "

_restart:
    .string "Press R to restart"

.fill 510 - (. - start), 1, 0
# the magic bytes in the end
.byte 0x55
.byte 0xAA
