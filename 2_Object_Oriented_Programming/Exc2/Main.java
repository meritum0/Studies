package gielda;

public class Main {
    public static void main(String[] args) {
        if (args.length != 2)
            throw new ZlaLiczbaParametrow();

        SystemTransakcyjny system = Czytnik.czytaj(args[0]);
        if ( system == null )
            throw new BrakSystemuTransakcyjnego();

        int liczba_tur = Integer.parseInt(args[1]);    // Wyrzuci wyjątek, jeśli zły parametr
        system.ustawLiczbeTur(liczba_tur);
        system.start();
        system.wypiszRezultat();

    }
}
