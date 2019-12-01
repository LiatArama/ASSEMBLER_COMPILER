;file ps.as
        ;commant_legal
;LINE 

;This line is too longggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggg

Ti
Label:  add	#7 , r3     ;BadCommant
AB:add #2 , r3
BA: ;
      AS:  stop
ASa:   stop
AAS:stop
AAAS :stop
   aAAA: stop
   
   
   
;Test of .entry
;OK
CD: .entry LIST
  .entry  LIST
  .entry  LOOP
 ;Check that when we reach second scan, this one fail (.define)
  .entry  sz
;BAD
  entry
  . entry END
  .entry
  .entry  r1
  .entry  2A
  .entry  432
  .entry  Li Li
  .entry  a,b
  .entry  LIST
  .entry  add
  .entry  LISA   LIST
  .entry  LISB   L1
  .entry  LISC , L1
  .entry  L1 , END
  .entry  LIST , L1 , STR
  .entry  LIST   L1   STR
  .entry  ,LIST
  .entry  LIST,


;Test of .extern
;OK
DC: .extern   W
  .extern   W
  .extern   L3
;BAD
  extern
  . extern  M
  .extern
  .extern   W , M
  .extern   23
  .extern   r4
  .extern   mov
  .extern   W,W
  .extern   W    , LIST
  .extern   L1   , END
  .extern   LIST , L1 , STR
  .extern   LIST   L1   STR
.extern     LIST   L1   STR
.extern     ,W
.extern     W,
  
;Test of .define
;OK
  .define   sz = 2
  .define   l = 3
  .define   skk=41 
  .define   len = 4
	
;BAD
  defing g = 45
  . define  sm  = 33
  .define   sz  = 43
  .define   23  = 24
  .define   ab  = 4 , ba =9
  .define   M   = 1   K = 1
  .define   r2  = 5
  .define   sub = 7
  .define   sub = sub 
  .define   so  = si  
  .define   ,sl = 43
  .define   sy, = 43
  .define   sl  = ,43
  .define   sy  = 43,
  
  
;Test of Label
;OK
A12345678901234567890123456789:        add		#2 , r1
A123456789012345678901234567890:       add		#4 , r0

;BAD
ThisLabelIsTooLonggggggggggggggg:	   mov      STR[5], STR[2]
12345678901234567890123456789012:      add		#7 , r3
3a:	                                   sub      r1, r4
r0:	                                   cmp      K, #sz
_a:	                                   bne      W
mov:								   mov      r3,LIST[sz]
lds :                                  jmp   W     
M123456789012345678901234567890 :      mov   r3,LIST[sz]


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

;BAD
			  mov       #4        , #1			  
			  mov       # 4       , r3
			  mov       r3        , #sz
			  mov     
			  mov       L1
			  mov       L1        ,	
			  mov       L1        , r4 ,
			  mov       L1          r4 
			  mov      ,L1        , r4 
			  mov      ,L1
			  mov       LIST[1]   , r5	, r7

			  mov       r4        , #2	
			  
			  cmp
			  cmp       L1
			  cmp       L1        ,
			  cmp       L1        , L1 ,
			  cmp       L1          L1 
			  cmp       L1          L1 ,		  
			  cmp      ,L1        , r4				  
			  cmp      ,L1      
			  cmp       L1        , L1 , L1		  
			  cmp       #2        , r5
			  cmp       #2        , #sz		
			  cmp       #1        , #2 , #3
			  cmp       r2
			  
			  add       LIST[1]   , #sz
			  add       LIST[1]   , # 2
			  add       #2        , #2 
			  add       # 2        , #sz 
			  add       						  
			  add       r2
			  add      ,r2
			  add       r2        , r4,
			  add       r2        , r4 , r5			  
			  add       r4        , #2
			  add       #2        , r4
			  
			  sub       r0        , r9
			  sub       r1        , r9
			  sub       r8        , r2
			  sub    
			  sub ;
			  sub       #2        , #sz
			  sub       r5        , #-5
			  sub       r8
			  sub       L1
			  sub       r5        , r4 , r1
			  sub       L1        ,
			  sub       L1        ,    , 
			  sub       Loooo     , r6 
			  sub       r4        , r5 , r6
			  sub       r7          r3
			  sub       r9        , r2
			  sub       #len      , r2
			  
	          not       
	          not       #sz 	  
	          not       #-3 
			  not       # -3 
			  not       5
	          not       r5        , r4 
	          not       LOOP
 
			  clr
			  clr       #sz
			  clr       #-3 
			  clr       L1        , r4
			  clr       r8
			  clr       r2        , r3
			  clr       MAIN:
			 
			  lea			  
			  lea       #1        , #len
			  lea       #sz       , #-5
			  lea       #length
			  lea       r5
			  lea       L1
			  lea       LOOP        r5
              lea       #6        , r5
			  lea       r5        , #6 
			  lea       LIST[1]
			  lea       LIST[1]   , #4
			  lea       L1        , L1 , L1
			  lea       ,           LIST[4]
              lea       L1        , L1 ,
			  
			  
			  inc
			  inc       #2
			  inc       #12az
			  inc       #len
			  inc       r4        , ,
			  inc       r2        , r3 , r3
			  inc       r2        , r3 , 
			  inc      ,r2        , r3 
			  inc       r8        , #4
			  inc       L1        , #-4
			  inc       r0
			  
			  dec   			  
			  dec       r4        , r4
			  dec       #2 
              dec       # 2 			  
			  dec       #len
			  dec       #12a
			  dec       K[23]
			  dec       K[K]
			  dec       K[se]
			  dec       r4        , #4    
              dec       MAIN,
			  dec       r4        ,
			  dec      ,r4		  

			  jmp   
			  jmp       LOOP ,
              jmp       LOOP , MAIN  
              jmp      ,r2	
              jmp                 , r2	
			  jmp       #3
			  jmp       #sz
			  jmp       ,#4
			  jmp       LIST[6]
			  jmp       STR[1]
			  jmp       K[sz]
						  
              bne       
              bne       L
              bne	    LOOP[6]
              bne	    #sz
              bne	    #-7       
			  bne	    LIST[3]       
			  bne	    STR[1]
			  bne                 , r4
			  bne       r3        , r4
			  bne       r5        ,     
              bne       L12			  
LOTOV:        bne       MAIN        LOOP
GAMLO:        bne       MAIN      , LOOP

              red       
              red       #-7
              red       sz
              red       #sz
              red       #77
              red       r2        , r2
              red     , r2
			  red       r7 ,
MAIN:         red       r1
			  red       LIS[2]

              prn
              prn       #-1       , #56
              prn                 , r4
              prn       r4        , 
              prn       LIST[91]
BALI:         prn       r1         , r2

              jsr
			  jsr       #45
			  jsr       #len
			  jsr       LIST[2]
			  jsr       K[0]
			  jsr       STR[3]
			  jsr       r5        , r3
			  jsr       L1        , LOOP
			  jsr       L3        , r3
			  jsr                 , r4
			  jsr       r4        ,

			  
		      rts                 ,
			  rts                 ,  ,
			  rts       #4        ,  ,
			  rts       LOOP
			  rts       #sz
			  rts       l2
			  rts       K[1]
			  rts       LIST[2]
			  rts       r4        , r4
			  
			  stop                ,
			  stop      #4        ,  ,
			  stop      LOOP
			  stop      #sz
			  stop      sz
			  stop      1
			  stop      l2
		  	  stop      r4 , r4
			  
			  liat      #2        , r5
			  go		LOOP

			  
;Test of directive
;OK

STR: 	     .string  "abcdef"		
LIST:	     .data  6, -9, len	
K:           .data  22
LENGTH:      .data 5,-2,55
AbGw:        .STRING  "lilili"
rAV:         .string  "liat"
ALM: 	     .string  "a,b,c,d,e"
LSO:         .data  1
			 .data  1 , 2 , 3
             .string  "bb"arn""uri"	
  
;BAD

STR:         .string   "barnuri"
STm:         . string  "barnuri"
M1:           string   1,2,3,4	
AB   :       .string   "55"
aBcD:        .string   'lolol'
A:           .string   '5""4'
			 .string   "sc

		     . data  544
LAS:         .data  2 , 31 , 244 , lon
			  .data 2,,3
		     .data  loae
iL:          ,data  45
12L          .data  7
f342          data  8
f11          .data  8,
f111         .data  ,8
f1111        .data  ,8,