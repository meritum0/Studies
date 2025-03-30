section .text
global mdiv

mdiv:
	xchg rbx, r9			; Zapisujemy wartość rbx w r9.
	mov r8, rdx			; Kopiujemy dzielnik.
	xor ebx, ebx			; Zerujemy rbx.
	add bh, [rdi+rsi*8-1]
	jns dzielnik			; Skok, jeśli dzielna była dodatnia.
	mov bl, 0x3			; Jeśli bl = 1, to dzielna była ujemna.
					; Jeśli bl = 3, to dzielna i dzielnik mają różne znaki.
	
negacja:
	mov rcx, rsi			; Kopiujemy size.
	xor edx, edx			; Zerujemy rdx.
	stc				; Ustawiamy CarryFlag = 1
loop:
	not qword [rdi+rdx*8]		; Zmieniamy bity na przeciwne.
	adc qword [rdi+rdx*8], qword 0	; Dodajemy CarryFlag.
	inc rdx				; Zwiększamy indeks o 1.
	loop loop			; Skok, jeśli jeszcze nie doszliśmy do końca tablicy.
	
 	test bl, bl			; AND bl z bl niezmieniający wartości bl.
	je return			; Skok, jeśli bl = 0
	
dzielnik:
 	test r8, r8			; AND r8 z r8 niezmieniający wartości r8.
	jns dzielenie			; Jezeli dzielnik jest dodatni, nie musimy go zmieniać.
	xor bl, 0x2			; Ustawia czy dzielna i dzielnik mają ten sam znak.
	neg r8				; Negujemy dzielnik.

dzielenie:
	mov rcx, rsi			; Kopiujemy size.
	xor edx, edx			; Zerujemy rdx.
loop_dzielenie:
	mov rax, [rdi+rcx*8-8]		; Aktualny blok liczby wrzucamy do rax.
	div r8				; Dzielimy przez podany dzielnik.
	mov [rdi+rcx*8-8], rax		; Wynik zapisujemy we wziętym bloku, reszta w rdx.
	loop loop_dzielenie		; Skok, jeśli nie doszliśmy do końca tablicy.
	
	xchg rax, rdx			; Resztę z dzielenia zapisujemy w rax.

	sar bl, 1			; Zapisujemy informację, czy dzielna była ujemna, w CF.	
	jnc czy_rozne_znaki		; Skok, jeśli dzielna była na początku dodatnia.
	neg rax				; Reszta ma być ujemna dla ujemnej dzielnej.

czy_rozne_znaki:
	sar bl, 1 			; Zapisujemy w CF informację,
					; czy dzielna i dzielnik były różnych znaków 
	jc negacja			; Musimy negować wynnik, jeśli były różnych znaków.

	add cl, [rdi+rsi*8-1]		; Czy wynik ujemny, czyli OF ?
	jns return
	div ch				; Zgłaszamy Overflow - dzielimy przez 0.
return:
	xchg rbx, r9			; Przywracamy rbx.
	ret
