;/******************************************************************************
;*  Copyright 2017 - 2018, Opulinks Technology Ltd.
;*  ----------------------------------------------------------------------------
;*  Statement:
;*  ----------
;*  This software is protected by Copyright and the information contained
;*  herein is confidential. The software may not be copied and the information
;*  contained herein may not be used or disclosed except with the written
;*  permission of Opulinks Technology Ltd. (C) 2018
;******************************************************************************/


;/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
;*/


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00001000

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp

                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

                IMPORT  IPC0_IRQHandler
                IMPORT  IPC1_IRQHandler
                IMPORT  IPC2_IRQHandler
                IMPORT  IPC3_IRQHandler
                IMPORT  GPIO_IRQHandler
                IMPORT  SCRT_IRQHandler
                IMPORT  MSQ_IRQHandler
                IMPORT  UARTDBG_IRQHandler
                IMPORT  UART0_IRQHandler
                IMPORT  UART1_IRQHandler
                IMPORT  I2C_IRQHandler
                IMPORT  SPI0_IRQHandler
                IMPORT  SPI1_IRQHandler
                IMPORT  SPI2_IRQHandler
                IMPORT  TIM0_IRQHandler
                IMPORT  TIM1_IRQHandler
                IMPORT  WDT_IRQHandler
                IMPORT  JTAG_IRQHandler
                IMPORT  APS_DMA_IRQHandler
                IMPORT  I2S_IRQHandler
                IMPORT  SPI3_IRQHandler
                IMPORT  MSQ_DMA_IRQHandler
                IMPORT  DET_XTAL_IRQHandler
                IMPORT  PDM_IRQHandler
                IMPORT  AUX_ADC_IRQHandler

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemManage_Handler         ; MPU Fault Handler
                DCD     BusFault_Handler          ; Bus Fault Handler
                DCD     UsageFault_Handler        ; Usage Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     DebugMon_Handler          ; Debug Monitor Handler
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     IPC0_IRQHandler           ;  0: IPC0
                DCD     IPC1_IRQHandler           ;  1: IPC1
                DCD     IPC2_IRQHandler           ;  2: IPC2
                DCD     IPC3_IRQHandler           ;  3: IPC3
                DCD     GPIO_IRQHandler           ;  4: GPIO Int:  
                DCD     SCRT_IRQHandler           ;  5: SCRT:  
                DCD     MSQ_IRQHandler            ;  6: MSQ2APS
                DCD     UARTDBG_IRQHandler        ;  7: UARTDBG
                DCD     UART0_IRQHandler          ;  8: UART0 
                DCD     UART1_IRQHandler          ;  9: UART1 
                DCD     I2C_IRQHandler            ; 10: I2C
                DCD     SPI0_IRQHandler           ; 11: SPI0
                DCD     SPI1_IRQHandler           ; 12: SPI1
                DCD     SPI2_IRQHandler           ; 13: SPI2
                DCD     TIM0_IRQHandler           ; 14: Timer0
                DCD     TIM1_IRQHandler           ; 15: Timer1
                DCD     WDT_IRQHandler            ; 16: Watchdog
                DCD     JTAG_IRQHandler           ; 17: JTAG debug
                DCD     APS_DMA_IRQHandler        ; 18: APS_DMA
                DCD     I2S_IRQHandler            ; 19: I2S
                DCD     SPI3_IRQHandler           ; 20: SPI3
                DCD     MSQ_DMA_IRQHandler        ; 21: MSQ_DMA
                DCD     DET_XTAL_IRQHandler       ; 22: Xtal detection
                DCD     PDM_IRQHandler            ; 23: PDM
                DCD     AUX_ADC_IRQHandler        ; 24: AUX ADC
                DCD     0                         ; 25: Reserved
                DCD     0                         ; 26: Reserved
                DCD     0                         ; 27: Reserved
                DCD     0                         ; 28: Reserved - CPU FPGA CLCD
                DCD     0                         ; 29: Reserved - CPU FPGA
                DCD     0                         ; 30: Reserved
                DCD     0                         ; 31: Reserved
__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  Boot_CheckStrapEarly
                IMPORT  Boot_CheckWarmBoot
                IMPORT  SystemInit
                IMPORT  __main
                IMPORT  Boot_MainPatch
                
                LDR     R0, = 0x0
                LDR     R1, = 0x0
                LDR     R2, = 0x0
                LDR     R3, = 0x0
                LDR     R4, = 0x0
                LDR     R5, = 0x0
                LDR     R6, = 0x0
                LDR     R7, = 0x0

                MOV     R8, R0
                MOV     R9, R0
                MOV     R10, R0
                MOV     R11, R0
                MOV     R12, R0

                MOV     R14, R0

                LDR     R0, =Boot_CheckStrapEarly
                BLX     R0

                LDR     R0, =Boot_CheckWarmBoot
                BLX     R0
                CMP     R0, #1
                BEQ     WarmBoot

                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
WarmBoot
                LDR     R0, =Boot_MainPatch
                LDR     R0, [R0]
                BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler         [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler          [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler        [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler          [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC
                B       .

                ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp

                ELSE
                
                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap

__user_initial_stackheap PROC
                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR
                ENDP

                ALIGN

                ENDIF


                END
