<?php
$input="public InverseQuantizeMMX
InverseQuantizeMMX PROC C uses ESI, coeff:PTR WORD, intra:DWORD, QP:DWORD, dc_scaler:DWORD
		MOV		EDX, QP
		MOV		ECX, 8
		MOVD	mm5, EDX
		MOV		ESI, coeff

		PUNPCKLDQ	mm5, mm5
		PCMPEQW		mm4, mm4
		PACKSSDW	mm5, mm5
		MOV			EAX, intra
		MOVQ		mm6, mm5
		OR			EAX, EAX
		PSLLW		mm5, 1
		JZ			JUMP2
		
		MOV		AX, [ESI]
		IMUL	AX, WORD PTR dc_scaler
		CMP		AX, 2047
		JLE		JUMP1
		MOV		AX, 2047
JUMP1:
		TEST	AX, AX
		JGE		JUMP2
		XOR		AX, AX
JUMP2:
		TEST	EDX, 1 
		JNE		JUMP4
		PADDW	mm6, mm4
JUMP4:
				
LOOP1:
		; load
		MOVQ	mm0, [ESI]
		MOVQ	mm1, [ESI+8]

		; get absolute coeff
		MOVQ	mm2, mm0
		MOVQ	mm3, mm1
		PSRAW	mm0, 15
		PSRAW	mm1, 15
		PXOR	mm2, mm0
		PXOR	mm3, mm1
		PSUBSW	mm2, mm0
		PSUBSW	mm3, mm1

		; zero
		MOVQ	mm4, mm2
		MOVQ	mm7, mm3
		PCMPGTW	mm4, mptr Q_ZERO
		PCMPGTW	mm7, mptr Q_ZERO

		; multiply
		PMULLW	mm2, mm5
		PMULLW	mm3, mm5
		PADDW	mm2, mm6
		PADDW	mm3, mm6

		; zero
		PAND	mm2, mm4
		PAND	mm3, mm7

		; sign
		PXOR	mm2, mm0
		PXOR	mm3, mm1
		PSUBSW	mm2, mm0
		PSUBSW	mm3, mm1
		
		; clip
		PADDW	mm2, mptr packed_min
		PADDW	mm3, mptr packed_min
		PADDUSW	mm2, mptr packed_add
		PADDUSW	mm3, mptr packed_add
		PSUBUSW	mm2, mptr packed_sub
		PSUBUSW	mm3, mptr packed_sub
		PADDW	mm2, mptr packed_low
		PADDW	mm3, mptr packed_low

		; store
		MOVQ	[ESI], mm2
		MOVQ	[ESI+8], mm3

		DEC		ECX
		LEA		ESI, [ESI+16]
		JNZ		LOOP1

		MOV		EDX, intra
		OR		EDX, EDX
		JZ		JUMP_E
		MOV		ESI, coeff
		MOV		[ESI], AX
JUMP_E:
		EMMS
		RET                                                           
InverseQuantizeMMX ENDP
";

$i=0;
$from[$i]="eax";	$to[$i]="RAX";	$i++;
$from[$i]="ebx";	$to[$i]="RBX";	$i++;
$from[$i]="ecx";	$to[$i]="RCX";	$i++;
$from[$i]="edx";	$to[$i]="RDX";	$i++;
$from[$i]="edi";	$to[$i]="RDI";	$i++;
$from[$i]="esi";	$to[$i]="RSI";	$i++;
//now simply convert to fascall safe versions
$from[$i]="RCX";	$to[$i]="R10";	$i++;
$from[$i]="RDX";	$to[$i]="R11";	$i++;
$from[$i]="RSI";	$to[$i]="R12";	$i++;
$from[$i]="RDI";	$to[$i]="R13";	$i++;
$from[$i]="RBX";	$to[$i]="R14";	$i++;

$output = $input;
$output = str_replace("\r","",$output);
$output = str_replace("\n","<br>",$output);
$output = str_replace(" ","&nbsp ",$output);
$output = str_replace("\t","&nbsp&nbsp ",$output);
for($token=0;$token<$i;$token++)
	$output = str_ireplace($from[$token],$to[$token],$output);
	
echo $output;
?>