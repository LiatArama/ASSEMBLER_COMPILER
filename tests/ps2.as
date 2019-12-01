;file ps.as
        ;commant_legal
;LINE 

;Test of .entry
;OK
  .entry  LIST
  .entry  LOOP
 ;Check that when we reach second scan, this one fail (.define)
  .entry  sz
  .entry  LIST

;Test of .extern
;OK
  .extern   W
  .extern   L3
  
;Test of .define
;OK
  .define   sz = 2
  .define   l = 3
  .define   skk=41 
  .define   len = 4
  
;Test of Label
;OK
A12345678901234567890123456789:        add		#2 , r1
A123456789012345678901234567890:       add		#4 , r0

;Test of Command
;OK
MAIN: 	      mov   r3,LIST[sz]
LOOP: 	      jmp   W
	          prn   #-5
	          mov   STR[5], STR[2]
	          sub   r1, r4
			  cmp   K, #sz
	          bne   W		
L1:           inc   L3
			  bne   LOOP
END:          stop	
			  dec       K[0]
			  dec       K[1]
			  dec       k[1]
			  
		  
	          mov       #2        , L1
	          mov       #2        , STR[1]
	          mov       #2        , LIST[1]
	          mov       #2        , r5

	          mov       #sz       , L1
	          mov       #sz       , STR[1]
			  mov       #sz       , LIST[1]
			  mov       #sz       , r5
			  
	          mov       L1        , L1
	          mov       L1        , STR[1]
			  mov       L1        , LIST[1] 
			  mov       L1        , r5
			  
	          mov       STR[1]    , L1
	          mov       STR[1]    , STR[1]
			  mov       STR[1]    , LIST[1]
			  mov       STR[1]    , r5
			  
			  mov       LIST[1]   , L1
	          mov       LIST[1]   , STR[1]
			  mov       LIST[1]   , LIST[1]
			  mov       LIST[1]   , r5
			  
			  mov       r5        , L1
	          mov       r5        , STR[1]
			  mov       r5        , LIST[1]
			  mov       r5	      , r5  
			  
			  cmp       #2        , #2
			  cmp       #2        , #sz  
			  cmp       #2        , L1
			  cmp       #2        , STR[1]
			  cmp       #2        , LIST[1]
			  cmp       #2        , r5
			  
			  cmp       #sz       , #2
			  cmp       #sz       , #sz  
			  cmp       #sz       , L1
			  cmp       #sz       , STR[1]
			  cmp       #sz       , LIST[1]
			  cmp       #sz       , r5  
		
			  cmp       L1        , #2
			  cmp       L1        , #sz  
			  cmp       L1        , L1
			  cmp       L1        , STR[1]
			  cmp       L1        , LIST[1]
			  cmp       L1        , r5  
			  
			  cmp       STR[1]    , #2
			  cmp       STR[1]    , #sz  
			  cmp       STR[1]    , L1
			  cmp       STR[1]    , STR[1]
			  cmp       STR[1]    , LIST[1]
			  cmp       STR[1]    , r5  
			  
			  cmp       LIST[1]   , #2
			  cmp       LIST[1]   , #sz  
			  cmp       LIST[1]   , L1
			  cmp       LIST[1]   , STR[1]
			  cmp       LIST[1]   , LIST[1]
			  cmp       LIST[1]   , r5  

			  cmp       r5        , #2
			  cmp       r5        , #sz  
			  cmp       r5        , L1
			  cmp       r5        , STR[1]
			  cmp       r5        , LIST[1]
			  cmp       r5        , r5  
			  
			  add       #2        , L1  
			  add       #2        , STR[1] 
			  add       #2        , LIST[1]	
			  add       #2        , r5
			  
			  add       #sz       , L1
			  add       #sz       , STR[1]			  
			  add       #sz       , LIST[1]		
			  add       #sz       , r5
			  
			  add       L1	      , L1
			  add       L1	      ,	STR[1]		  
			  add       L1	      ,	LIST[1]			  
			  add       L1	      ,	r5		  
			  
			  add       STR[1]    , L1
			  add       STR[1]    , STR[1]
			  add       STR[1]    , LIST[1]
			  add       STR[1]    , r5		
			  
			  add       LIST[1]   , L1
			  add       LIST[1]   , STR[1]
			  add       LIST[1]   , LIST[1]
			  add       LIST[1]   , r5				  
			
   			  add       r5        , L1
   			  add       r5        , STR[1]
   			  add       r5        , LIST[1]
   			  add       r5        , r5		
			  
			  sub       #2        , L1  
			  sub       #2        , STR[1] 
			  sub       #2        , LIST[1]	
			  sub       #2        , r5
			  
			  sub       #sz       , L1
			  sub       #sz       , STR[1]			  
			  sub       #sz       , LIST[1]		
			  sub       #sz       , r5
			  
			  sub       L1	      , L1
			  sub       L1	      ,	STR[1]		  
			  sub       L1	      ,	LIST[1]			  
			  sub       L1	      ,	r5		  
			  
			  sub       STR[1]    , L1
			  sub       STR[1]    , STR[1]
			  sub       STR[1]    , LIST[1]
			  sub       STR[1]    , r5		
			  
			  sub       LIST[1]   , L1
			  sub       LIST[1]   , STR[1]
			  sub       LIST[1]   , LIST[1]
			  sub       LIST[1]   , r5				  
			
   			  sub       r5        , L1
   			  sub       r5        , STR[1]
   			  sub       r5        , LIST[1]
   			  sub       r5        , r5				  
			  
			  not       L1
			  not       STR[1]
			  not       LIST[1]   
			  not       r7    
			  
			  clr       L1
			  clr       STR[1]
			  clr       LIST[1]
			  clr       r7
			  
			  lea       L1        ,  L1
			  lea       L1        ,  STR[1]
			  lea       L1        ,  LIST[1]
			  lea       L1        ,  r7 
			  
			  lea       STR[1]    , L1
			  lea       STR[1]    , STR[1]
			  lea       STR[1]    , LIST[1]
			  lea       STR[1]    , r7 
			  
			  lea       LIST[1]   , L1		  
			  lea       LIST[1]   , STR[1]
			  lea       LIST[1]   , LIST[1]
			  lea       LIST[1]   , r7 		  
			  
			  inc       L1
			  inc       STR[1]
			  inc       LIST[1]
			  inc       r7 
			  
			  dec       L1
			  dec       STR[1]
			  dec       LIST[1]
			  dec       r7
			  
			  jmp       L1
			  jmp       r7
			  
			  bne       L1
			  bne       r7
			  
			  red       L1
			  red       STR[1]
			  red       LIST[1]
			  red       r7
			 
 			  prn       #4
			  prn       #sz
			  prn       L1
			  prn       STR[1]
			  prn       LIST[1]
			  prn       r7
			  
			  jsr       r5
			  jsr       L1
			 
			  rts
			  
			  stop
			
 			  mov       LIST[1]   , r8
		 	  mov       A         , r3
			  mov       #2        , r4
  
			  cmp       #2        , r5
			  cmp       #2        , #sz		

			  add       #2        , r4
			  
			  sub       r0        , r9
			  sub       r1        , r9
			  sub       r8        , r2

			  sub       Loooo     , r6 
			  sub       r9        , r2
			  sub       #len      , r2
			  
	          not       LOOP
 
			  clr       r8
			  
			  inc       r0
			  
			  dec       K[23]
					  
              bne       L
              bne       L12			  

              red       sz
			  red       LIS[2]

              prn       LIST[91]

;Test of directive
;OK
STR: 	     .string  "abcdef"		
LIST:	     .data  6, -9, len	
K:           .data  22
LENGTH:      .data 5,-2,55
rAV:         .string  "liat"
ALM: 	     .string  "a,b,c,d,e"
LSO:         .data  1
			 .data  1 , 2 , 3
             .string  "bb"arn""uri"	