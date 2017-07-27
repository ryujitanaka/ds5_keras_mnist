;==================================================================
; Copyright ARM Ltd 2009-2012. All rights reserved.
;
; Basic Secure Monitor
;==================================================================

  PRESERVE8

  AREA  Monitor_Code, CODE, ALIGN=5, READONLY

  ; Defines used in the code
Mode_MON            EQU   0x16
Mode_SVC            EQU   0x13
NS_BIT              EQU   0x1

  ENTRY

; ------------------------------------------------------------
; Monitor mode vector table
; ------------------------------------------------------------

  EXPORT monitor
monitor

  B .     ; Reset      - not used by Monitor
  B .     ; Undef      - not used by Monitor
  B       SMC_Handler
  B .     ; Prefetch   - can by used by Monitor
  B .     ; Data abort - can by used by Monitor
  B .     ; RESERVED
  B .     ; IRQ        - can by used by Monitor
  B .     ; FIQ        - can by used by Monitor

; ------------------------------------------------------------
; SMC Handler
;
; - Detect which world executed SMC
; - Saves state to appropriate stack
; - Restores other worlds state
; - Switches world
; - Performs exception return
; ------------------------------------------------------------

  EXPORT SMC_Handler
SMC_Handler PROC {r4-r12}
  PUSH   {r0-r3}                       ; r0-r3 contain args to be passed between worlds
                                       ; Temporarily stack, so can be used as scratch regs

  ; Which world have we come from
  ; ------------------------------
  MRC     p15, 0, r0, c1, c1, 0        ; Read Secure Configuration Register data
  TST     r0, #NS_BIT                  ; Is the NS bit set?
  PUSH    {r0}                         ; Save a copy of the SCR on entry
  BIC     r0, r0, #NS_BIT              ; Make sure the SCR.NS bit is now clear
  MCR     p15, 0, r0, c1, c1, 0        ; Write Secure Configuration Register data
  ISB

  ; Load save to pointer
  ; ---------------------
  LDREQ   r0, =S_STACK_SP             ; If NS bit set, was in Normal world.  So restore Secure state
  LDRNE   r0, =NS_STACK_SP
  LDR     r2, [r0]

  ; Load restore from pointer
  ; --------------------------
  LDREQ   r1, =NS_STACK_SP
  LDRNE   r1, =S_STACK_SP
  LDR     r3, [r1]

  ; r2  <-- save to
  ; r3  <-- restore from

  ; Save general purpose registers, SPSR, LR and SP
  ; ------------------------------------------------
  STMFD   r2!, {r4-r12}               ; Save r4 to r12
  MRS     r4, spsr                    ; Also get a copy of the SPSR_mon
  STMFD   r2!, {r4, lr}               ; Save original SPSR_mon and LR_mon

  CPS     #Mode_SVC                   ; Switch into Supervisor mode temporarily
  MOV     r4, sp                      ; Temp for SP_svc
  STMFD   r2!, {r4, lr}               ; Save SP_svc and LR_svc

  STR     r2, [r0]                    ; Save updated pointer back, r0 and r2 now free

  ; Restore other world's registers, SPSR and LR
  ; ---------------------------------------------
  LDMFD   r3!, {r4, lr}               ; Restore SP_svc and LR_svc
  MOV     sp, r4                      ; Temp for SP_svc
  CPS     #Mode_MON                   ; Switch back into Monitor mode

  LDMFD   r3!, {r0, lr}               ; Get SPSR_mon and LR_mon (return address)
  MSR     spsr_cxsf, r0               ; Restore SPSR_mon
  LDMFD   r3!, {r4-r12}               ; Restore registers r4 to r12

  STR     r3, [r1]                    ; Save updated pointer back, r1 and r3 now free

  ; Clear local monitor
  ; --------------------
  CLREX                               ; Not strictly required in this example, as not using LDREX/STREX
                                      ; However, architecturally should execute CLREX on a context switch

  ; Set SCR.NS bit correctly for world we're entering
  ; --------------------------------------------------
  POP     {r0}
  EOR     r0, r0, #NS_BIT              ; Toggle NS bit
  MCR     p15, 0, r0, c1, c1, 0        ; Write Secure Configuration Register data


  ; Now restore args (r0-r3)
  ; -------------------------
  POP     {r0-r3}


  ; Perform exception return
  ; -------------------------
  MOVS    pc, lr

  ENDP


; ------------------------------------------------------------
; Monitor Initialization
;
; This function is called by the Secure world code at start-up
; to initialize the Secure Monitor.  The function returns back
; to the Secure world.  Secure world must execute a SMC to
; switch to the Normal world
; ------------------------------------------------------------

  EXPORT monitorInit
monitorInit PROC

  ; Install Secure Monitor
  ; -----------------------
  LDR     r0, =monitor                 ; Get address of Monitor's vector table
  MCR     p15, 0, r0, c12, c0, 1       ; Write Monitor Vector Base Address Register
  
  ; Initialize the Monitor mode stack pointer
  IMPORT  ||Image$$MON_STACK$$ZI$$Limit||
  CPS    #Mode_MON
  LDR    sp, =||Image$$MON_STACK$$ZI$$Limit||
  CPS    #Mode_SVC

  ; Create and save a "dummy" Normal world state
  ; This will be used for the first entry to the Normal world
  LDR     r1, =NS_STACK_LIMIT          ; Get top of Normal state stack (assuming FD model)

  MOV     r2,  #0
  MOV     r3,  #0

  STMFD   r1!, {r2-r3}                 ; Save off "dummy" values for r11-r12
  STMFD   r1!, {r2-r3}                 ; Save off "dummy" values for r9-r10
  STMFD   r1!, {r2-r3}                 ; Save off "dummy" values for r7-r8
  STMFD   r1!, {r2-r3}                 ; Save off "dummy" values for r5-r6
  STMFD   r1!, {r2}                    ; Save off "dummy" values for r4
  ; NOT saving value for r0-r3, as we are using these registers to pass information between worlds

  IMPORT  ns_image
  MOV     r2,  #Mode_SVC               ; Set initial mode for the Normal world as SVC
  LDR     r3, =ns_image                ; Set starting address as location on Normal world binary
  STMFD   r1!, {r2-r3}
  MOV     r2,  #0                      ; Set initial SP_svc for the Normal world
  MOV     r3,  #0                      ; Set initial LR_svc for the Normal world
  STMFD   r1!, {r2,r3}

  LDR     r0, =NS_STACK_SP
  STR     r1, [r0]                     ; Save a pointer to the top of the Normal world context


  ; Set up the pointer to the Secure world memory saved state
  ; This buffer is currently empty, as the Secure world is running
  LDR     r0, =S_STACK_LIMIT           ; Get address of memory for saving Secure state
  LDR     r1, =S_STACK_SP              ; Get address of the pointer to saved Secure state
  STR     r0, [r1]                     ; Buffer currently empty, so pointer points to the top of the stack (FD stack model)


  ; Return to the Secure world code
  ; --------------------------------
  BX      lr

  ENDP

; ------------------------------------------------------------
; Space reserved for stacks
; ------------------------------------------------------------

  AREA  Monitor_Data, DATA, ALIGN=8, READWRITE

  ; Reserving some memory to save Normal world state information
NS_STACK_BASE
  DCD     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
NS_STACK_LIMIT

  ; Reserving some memory to save Secure world state information
S_STACK_BASE
  DCD     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
S_STACK_LIMIT

  ; This will be used as a pointer to the top of the saved Normal world state
NS_STACK_SP
  DCD     0
  
  ; This will be used as a pointer to the top of the saved Secure world state
S_STACK_SP
  DCD     0

  END
