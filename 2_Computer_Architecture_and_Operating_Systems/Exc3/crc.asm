BUF_LENGTH equ 0x1000			; Będziemy alokować 4096 bajtów.
MAX_LENGTH equ 0x40			; Maksymalny stopień wielomianu crc-  u nas 64.
SYS_OPEN equ 2
SYS_CLOSE equ 3
SYS_READ equ 0
SYS_WRITE equ 1
SYS_LSEEK equ 8
SYS_EXIT equ 0x3C
SEEK_CUR equ 1				; W lseek() przesuwamy się względem aktualnej pozycji
ZERO equ 0x30
ONE equ 0x31
ERROR equ 1
SUCCESS equ 0
NEG_VAL equ -1
BIG_VAL equ 0x0000FFFFFFFFFFFF		; Dowolna liczba większa niż moduł dowolnej 32-bitowej liczby.

section .bss
buffer:	resb BUF_LENGTH
wynik:	resb 0x41			; Wynik będzie w max. 64 bajtach + 1 bajt (\n).
	
section .text
global _start
					; Przeznaczenia wybranych rejestrów w programie:
					; r8 - deskryptor,
					; r9 - wielomian crc,
					; rcx - długość danych we fragmencie,
					; rbx - lokalny indeks fragmentu,
					; r10 - liczba danych wczytanych do bufora,
					; r11 - aktualny indeks odczytu z bufora,
					; r12 - rejestr, który xoruje z wielomianem,
					; r13 - stopień wielomianu crc.
	
_start:
	cmp qword [rsp], 3		; Czy zgadza się liczba parametrów?
	jne error
	mov rdi, [rsp + 16]		; Wskaźnik na pierwszy znak nazwy pliku do rdi.
	mov rcx, [rsp + 24]		; Wskaźnik na pierwszy znak wielomianu crc do rcx.
	push rbx
	push r12
	push r13
	push r14

	push rcx
	mov rax, SYS_OPEN
	xor edx, edx		
	mov esi, edx			; O_RDONLY: chcemy tylko czytać z pliku.
	syscall
	pop rcx
	test rax, rax
	js error			; Błąd syscalla.

	mov r8, rax			; Deskryptor do r8.
	
obrobka_crc:
	xor r9, r9
	xor r13, r13

przechodzenie:	
	cmp byte [rcx + r13], 0 	; Czy już koniec stringa?
	je shiftowanie

	shl r9, 1
	cmp byte [rcx + r13], ONE 	; Na danym miejscu znajduje się '1'.
	je jeden
	cmp byte [rcx + r13], ZERO 	; Na danym miejscu znajduje się '0'.
	je zero
	mov rax, NEG_VAL		; Sygnalizujemy błąd.
	jmp zamkniecie
	
jeden:	
	or r9, 1			; Dodajemy 1 na początku.
	
zero:
	inc r13
	cmp r13, MAX_LENGTH
	mov rax, NEG_VAL		; Sygnalizuje błąd w przypadku skoku.
	jg zamkniecie			; Wielomian crc jest stopnia większego niż 64.
	jmp przechodzenie

shiftowanie:				; Wyrównujemy wielomian crc do lewej.
	mov cl, MAX_LENGTH
	sub byte cl, byte r13b
	shl r9, cl
	
obliczanie:				; Przygotowujemy rejestry do oblicznia crc.
	xor r10, r10
	xor r11, r11
	xor r12, r12
	xor ebx, ebx
	mov ecx, ebx			; Żeby na pewno nie zaszło ebx = cx+2.

loop_czytaj:
	mov eax, ecx
	add eax, dword 2
	cmp ebx, eax			; Czy czytamy przesunięcie?
	jne nie_przesuniecie

przesuniecie:
	xor r14, r14
	xor ecx, ecx
	call czytaj_nastepny
	mov cl, al			; Czytany bajt do cl.
	call czytaj_nastepny
	mov ch, al			; Czytany bajt do ch.
	call czytaj_nastepny
	mov r14b, al
	shl r14d, 16			; Ustawiamy bajt na odpowiedniej pozycji.
	or ecx, r14d			; Wstawiamy kolejny bajt na odpowiednie miejsce w ecx.
	
	call czytaj_nastepny
	shl eax, 24			; Ustawiamy bajt na odpowiedniej pozycji.
	or ecx, eax			; Teraz już całe 4-bajtowe przesunięcie jest w ecx.
	cmp ecx, 0
	jl ujemne_przesuniecie
	
dodatnie_przesuniecie:
	mov eax, r11d			; Indeks odczytu z bufora.
	add eax, ecx
	sub eax, r10d			; Liczba danych wczytanych do bufora.
	jle bez_lseek	
	jmp wywolanie_lseek
	
ujemne_przesuniecie:
	mov eax, ebx			; Lokalny indeks fragmentu do rax.
	neg eax
	cmp eax, ecx
	je dokonczenie			; Koniec pliku - przesunięcie wskazuje na swój fragment.
	mov eax, r11d			; Indeks odczytu z buforu do eax.
	neg eax
	cmp ecx, eax			; Czy kolejny bajty do czytania znajdują się jeszcze w buforze?
	jns bez_lseek

lseek_ujemny:
	add ecx, r11d
	sub ecx, r10d
	mov eax, ecx
	cdqe
	mov r11, BIG_VAL		; W r11 jest liczba wieksza niż moduł wartości przesunięcia.
	xor r10, r10		    	; Oznaczamy, że będzie trzeba na nowo wypełnić bufor.

wywolanie_lseek:	
	push r11
	push rcx
	mov rsi, rax			; W rsi informacja, o ile bajtów się przesuwamy w pliku.
	mov rax, SYS_LSEEK
	mov rdi, r8			; Deskryptor do rdi.
	mov rdx, SEEK_CUR 
	syscall	
	pop rcx			
	pop r11
	test rax, rax
	js zamkniecie			; Błąd lseek().

bez_lseek:	
	xor ebx, ebx			; Ustawiamy, że jesteśmy na początku fragmentu.
	add r11d, ecx
	xor ecx, ecx			; Będziemy czytać nową długość.
	jmp loop_czytaj
	
	
nie_przesuniecie:
	test ebx, ebx			; Czy czytamy bajty opisujące długość danych?
	jnz normalne_dane
					; Wczytujemy długość danych do cx.
	call czytaj_nastepny
	mov cl, al
	call czytaj_nastepny
	mov ch, al
	jmp loop_czytaj

normalne_dane:	
	call czytaj_nastepny
	mov rsi, 8			; Licznik pozostałych pętli.
	
xorowanie:
	xor ah, ah
	shl r12, 1
	adc ah, 0			; Zapisujemy CF z shiftowanego r12 w ah.
	shl al, 1
	adc r12, 0			; Wstawiamy CF(przed chwilą MSB al) w LSB r12.
	test ah, ah
	jz bez_xor
	xor r12, r9			; Wykonujemy xor tylko jeśli w na początku w MSB r12 było '1'.
	
bez_xor:
	dec rsi
	jnz xorowanie
	jmp loop_czytaj


dokonczenie:
	mov bl, r13b			; Długość crc do bl.
	mov ecx, MAX_LENGTH		; Licznik pozostałych pętli.
	
petla_koniec:	
	shl r12, 1
	jnc bez
	xor r12, r9			; Wykonujemy xor tylko jeśli w na początku w MSB r12 było '1'.
bez:	
	loop petla_koniec

	mov ecx, 1
	mov eax, 1
	
wrzucanie:				; Wrzucamy po kolei wszystkie bity reszty do [wynik].
	ror rax, 1
	mov rdx, rax
	and rdx, r12			; Wyłuskuje konkretny bit z r12.
	rol rdx, cl			; Rotate, żeby wyłuskany bit był na LSB.
	
	add dl, ZERO			; Zamiana na znak w kodzie ASCII
	mov byte [wynik + ecx-1], dl	; Wrzucamy ten znak na odpowiednie miejsce.
	inc cl
	dec bl
	jnz wrzucanie

	mov byte [wynik + ecx-1], 0xa 	; Na koniec wrzucamy znak \n.
	
wypisanie_wyniku:
	inc r13				; Bo chcemy wypisać też \n.
	mov eax, SYS_WRITE	
	mov rdi, dword 1		; Wypisujemy na STDOUT
	mov rsi, wynik			; Wypisujemy znaki z tablicy [wynik]
	mov rdx, r13			; Ile bajtów wypisujemy?
	syscall
	cmp rax, 0
	jg zamkniecie
	mov rax, NEG_VAL		; Sygnalizujemy błąd sys_write

zamkniecie:
	xchg rdx, rax
	mov eax, SYS_CLOSE
	mov rbx, r8			; Deskryptor do rbx.
	syscall
	
	test rdx, rdx
	js error			; Czy wcześniej wystąpił błąd?
	test rax, rax
	jz koniec			; Udało się pomyślnie zamknąć plik.

error:
	pop r14
	pop r13
	pop r12
	pop rbx
	mov rax, SYS_EXIT
	mov rdi, ERROR		
	syscall
	
koniec:
	pop r14
	pop r13
	pop r12
	pop rbx
	mov rax, SYS_EXIT
	mov rdi, SUCCESS	
	syscall
	

czytaj_nastepny:			; Ten blok zwraca nastepny bajt z bufora, a jeśli
					; skończył się bufor, to wczytuje do niego nowe dane z pliku.
	
	cmp r11, r10			; Czy skończył się bufor?
	jl czytaj		
	
	push rcx
	mov rax, SYS_READ	
	mov rdi, r8			; Deskryptor do rdi.
	mov rsi, buffer			; Czytamy do [buffer]
	mov rdx, BUF_LENGTH		; Ile próbujemy przeczytać?
	syscall
	pop rcx
	cmp rax, 0			; Czy przeczytaliśmy 0 bajtów?
	jg ok
	
	mov rax, NEG_VAL		; Sygnalizujemy błąd.
	jmp zamkniecie
ok:	
	mov r10, rax			; Liczba przeczytanych bajtów do r10.
	xor r11, r11			; Zerujemy indeks odczytu z bufora.

czytaj:	
	mov al, byte [buffer + r11]
	inc r11				; Indeks odczytu z  bufora.
	inc rbx				; Lokalny indeks fragmentu.
	ret
