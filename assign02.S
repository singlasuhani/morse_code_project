#include "hardware/regs/addressmap.h"
#include "hardware/regs/io_bank0.h"
#include "hardware/regs/m0plus.h"
#include "hardware/regs/timer.h"

.syntax unified
.cpu    cortex-m0plus
.thumb
.global main_asm
.align  4

@taken from assign01.s
.equ    DFLT_STATE_STRT, 1            @ Specify the value to start flashing
.equ    DFLT_STATE_STOP, 0            @ Specify the value to stop flashing
.equ    DFLT_ALARM_TIME, 1000000      @ Specify the default alarm timeout
.equ    GPIO_BTN_EN_MSK, 0x00400000   @ Bit-22 for falling-edge event on GP21
.equ    GPIO_BTN_EN,     21           @ Specify pin for the "enter" button
.equ    GPIO_DIR_IN,     0            @ Specify input direction for a GPIO pin
.equ    GPIO_ISR_OFFSET, 0x74         @ GPIO is int #13 (vector table entry 29)
.equ    ALRM_ISR_OFFSET, 0x40         @ ALARM0 is int #0 (vector table entry 16)
.equ    ALRM_TIME,       3000000


@ Entry point to the ASM portion of the program
main_asm:
   bl   setup_buttons              @ subroutine to initialize the buttons
   bl   setup_alarmisr             @ subroutine to set up the alarm isr 
   bl   setup_gpioisr              @ subroutine to set up the gpio isr.

main_loop:
    bl  setup_alarm        @ subroutine to set up the alarm timer
    wfi                    @wfi
    b   main_loop          @ Branch back to the beginning of the main loop

setup_buttons:
    push    {lr}                        @ save the lr
    movs    r2, #GPIO_BTN_EN            @ Move GPIO_BTN_EN to r2
    mov     r0, r2                      @ Move value from r2 to r0 
    bl      asm_gpio_init               @ call function to set gpio
    
    mov     r0, r2                      @ Reuse the value for setting direction
    movs    r3, #GPIO_DIR_IN            @ Move GPIO_DIR_IN to r3
    mov     r1, r3                      @ Move value from r3 to r1
    bl      asm_gpio_set_dir            @ call function to set direction
    
    movs    r0, #GPIO_BTN_EN            @ Reuse the value for setting gpio interrupt
    bl      asm_gpio_set_irq            @ call function to set gpio interrupt
    pop     {pc}                        @ Restore the lr and return


setup_alarmisr:
    ldr     r2, =(PPB_BASE + M0PLUS_VTOR_OFFSET)   @ Load Offset address for the vector table to r2
    ldr     r1, [r2]                               @ Load the value of vector table
    ldr     r0, =alarm_isr                         @ Load address of alarm ISR
    adds    r1, r1, #ALRM_ISR_OFFSET               @ Add ALRM_ISR_OFFSET
    str     r0, [r1]                               @ Store address of alarm ISR
    movs    r2, #1                                 @ Move 1 to r2
    ldr     r1, =(PPB_BASE + M0PLUS_NVIC_ISER_OFFSET) @ Load the Offset address for the NVIC Interrupt Set Enable register
    str     r2, [r1]                               @ Enable NVIC interrupt for alarm
    movs    r2, #1                                 @ Move 1 to r2
    ldr     r1, =(PPB_BASE + M0PLUS_NVIC_ICPR_OFFSET) @ Load the Offset address for the NVIC Interrupt Clear Pending register
    str     r2, [r1]                               @ Clear any pending alarm interrupts
    bx      lr                                      @ Return

setup_gpioisr:
    ldr     r2, =(PPB_BASE + M0PLUS_VTOR_OFFSET)   @ Load Offset address for the vector table
    ldr     r1, [r2]                               @ Load the value of vector table
    ldr     r0, =gpio_isr                          @ Load address of GPIO ISR
    adds    r1, r1, #GPIO_ISR_OFFSET               @ Add #GPIO_ISR_OFFSET
    str     r0, [r1]                               @ Store address of GPIO ISR
    movs    r1, #1                                 @ Move 1 to r1
    lsls    r1, r1, #13                            @ Left shifting by 13 bits
    ldr     r2, =(PPB_BASE + M0PLUS_NVIC_ICPR_OFFSET) @ Load the Offset address for the NVIC Interrupt Clear Pending register
    str     r1, [r2]                               @ Clear any pending GPIO interrupts
    ldr     r2, =(PPB_BASE + M0PLUS_NVIC_ISER_OFFSET) @ Load the Offset address for the NVIC Interrupt Set Enable register
    str     r1, [r2]                               @ Enable NVIC interrupt for GPIO
    bx      lr                                      @ Return

setup_alarm:
    push    {lr}                            @ Save the lr

    ldr     r2, =ALRM_TIME                  @ Load address of alarm time
    ldr     r1, =(TIMER_BASE + TIMER_TIMELR_OFFSET) @ Load address of current timer value
    ldr     r0, [r1]                        @ Load value into r0 from the address in r1
    add     r0, r2                          @ Add  r2 in r0
    ldr     r1, =(TIMER_BASE + TIMER_ALARM0_OFFSET) @ Load address of the alarm0 
    str     r0, [r1]                        @ Store the alarm time in the alarm0 

    movs    r0, #1                          @ Move the value 1 into r0
    ldr     r1, =(TIMER_BASE + TIMER_INTE_OFFSET) @ Load address of timer interrupt in r1
    str     r0, [r1]                        @ store the alarm interruptin the timer interrupt

    pop     {pc}                            @ Restore the lr and return


.thumb_func
alarm_isr:
    push    {lr}                        @ Save the lr

    ldr     r1, =(TIMER_BASE + TIMER_INTR_OFFSET)  @ Timer interrupt register address
    movs    r0, #1                      @ Move the value 1 into r0
    str     r0, [r1]                    @ Clear timer interrupt

    ldr     r0, =char                   @ Address of char into r0
    movs    r1, #0                      @ set r1 to 0
    str     r1, [r0]                    @ Reset char by giving 0 to it

    movs    r0, #2                      @ timeout event
    bl      handle_Dot_Dash              @ call handle_Dot_Dash

    pop     {pc}                        @ Restore lr and return



.thumb_func
gpio_isr:
    push    {lr}                                        @ Save lr
    ldr     r1, =(IO_BANK0_BASE + IO_BANK0_INTR2_OFFSET) @ Load address of gpio interrupt 
    ldr     r2, [r1]                                    @ load address of r1 to r2
    str     r2, [r1]                                    @ store address of r2 to r1
    ldr     r1, =GPIO_BTN_EN_MSK                        @ Load GPIO_BTN_EN_MSK  to r1
    ands    r2, r1                                      @ Check if button interrupt occurred
    cmp     r1, r2                                      @ Compare r2 to r1
    beq     handle_rising                               @ If equal it's a rising edge(button interrupt occurred)

handle_falling:                                          
    ldr     r1, =(TIMER_BASE + TIMER_TIMELR_OFFSET)     @ Load address of timer lower 
    ldr     r0, [r1]                                    @ Read current timer value
    ldr     r1, =time                                   @ Load address of time 
    ldr     r2, [r1]                                    @ Load address of r1 to r2
    subs    r0, r2                                      @ subtract r2 from r0
    ldr     r1, =500000                                 @ load 500000 to r1
    cmp     r0, r1                                      @ Compare r1 to r0
    bge     process_dash                                @ If greater or equal - dash

process_dot:                                            @short press
    movs    r0, #0                                      @ Indicate dot with 0
    bl      handle_Dot_Dash                             @ Call function handle_Dot_Dash
    b       gpio_isr_end                                @ ISR end

process_dash:                                           @ Long press 
    movs    r0, #1                                      @ Indicate dash with 1
    bl      handle_Dot_Dash                             @ Call function handle_Dot_Dash

gpio_isr_end:                                           @ ISR end
    pop     {pc}                                        @ Restore lr and return

handle_rising:                                            @ Rising edge detected
    ldr     r1, =(TIMER_BASE + TIMER_TIMELR_OFFSET)     @ Load address of timer lower register
    ldr     r0, [r1]                                    @ Read current timer value
    ldr     r1, =time                                   @ Load address of time variable
    str     r0, [r1]                                    @ Store current time for next comparison
    b       gpio_isr_end                                @ Proceed to ISR end

@taken from assign01.s
.align 4
msg: .asciz "Hello World!\n"
.data
time: .word     0
char: .word     0



  
