package gielda;

import java.util.Random;

public class Spolka {
    private final String nazwa;
    private int laczna_ilosc_akcji;
    private int cena_ostatniej_transakcji;
    private int suma_tury;  // Suma pieniędzy wydanych na akcje spółki w aktualnej turze.
    private int liczba_zakupow_tury; // Liczba akcji spółki zakupionych w aktualnej turze.
    private int[] sumy_10_tur = new int[10];    // Informacje o suma_tury z ostatnich 10 tur.
    private int[] liczba_zakupow_10_tur = new int[10];  // Informacje o liczba_zakupow_tury z ostatnich 10 tur.
    private boolean sma10ByloNadSma5 = false;   // Czy w poprzedniej turze SMA10 było nad SMA5?

    public Spolka( String nazwa ) { this.nazwa = nazwa; }

    //*************************************** METODY PUBLIC *******************************************//

    public void ostatniaTransakcja( int cena ) { cena_ostatniej_transakcji = cena; }

    public String nazwa() { return nazwa; }

    public void dodajAkcje( int liczba ) { laczna_ilosc_akcji += liczba; }

    // Podaje losową cenę nieodbiegającą od ostatniej o więcej niż 10.
    public int losowaCena() {
        Random random = new Random();
        int odchylenie = random.nextInt() % 11;
        if ( cena_ostatniej_transakcji + odchylenie <= 0 )
            return cena_ostatniej_transakcji;
        return cena_ostatniej_transakcji + odchylenie;
    }

    // Zwraca losową liczbę między 1, a łączną liczbą akcji danej spółki na giełdzie.
    public int losowaLiczbaDoKupienia() {
        if ( laczna_ilosc_akcji == 0 )
            return 0;
        Random random = new Random();
        return mod( random.nextInt(), laczna_ilosc_akcji ) + 1;
    }

    // Wystąpiła nowa sprzedaż, więc aktualizujemy ostatnią cenę, liczbe zakupów i kwotę
    // wydaną na akcje tej spółki w turze
    public void uaktualnij( int liczba, int cena ) {
        cena_ostatniej_transakcji = cena;
        suma_tury += cena * liczba;
        liczba_zakupow_tury += liczba;
    }

    // Aktualizuje po końcu tury tabele potrzebne do obliczania SMA.
    public void koniecTury( int tura ) {
        if ( sredniaSma10() > sredniaSma5(tura) )
            sma10ByloNadSma5 = true;
        else
            sma10ByloNadSma5 = false;

        sumy_10_tur[tura % 10] = suma_tury;
        liczba_zakupow_10_tur[tura % 10] = liczba_zakupow_tury;
        suma_tury = 0;
        liczba_zakupow_tury = 0;
    }

    public boolean equals( Spolka other ) {
        return nazwa.equals( other.nazwa );
    }

    // Czy inwestor SMA powinien kupić akcje danej spółki?
    public boolean czyMomentKupna( int tura ) {
        return ( tura >= 10 && sma10ByloNadSma5 && sredniaSma10() < sredniaSma5(tura) );
    }

    // Czy inwestor SMA powinien sprzedać akcje danej spółki?
    public boolean czyMomentSprzedazy( int tura ) {
        return ( tura >= 10 && !sma10ByloNadSma5 && sredniaSma10() > sredniaSma5(tura) );
    }

    public String toString() { return nazwa; }

    //*************************************** METODY PRIVATE *******************************************//

    private float sredniaSma10() {
        int dzielnik = 0;
        int suma = 0;
        for ( int i : sumy_10_tur)
            suma += i;

        if ( suma == 0 )
            return 0;

        for ( int i : liczba_zakupow_10_tur )
            dzielnik += i;

        return (float)suma / dzielnik;
    }

    private float sredniaSma5( int tura ) {
        int dzielnik = 0;
        int suma = 0;
        int j = 0;
        for ( int i = mod(tura - 1, 10); j < 5; i = mod(i - 1, 10), j++ )
            suma += sumy_10_tur[i];

        if ( suma == 0 )
            return 0;

        j = 0;
        for ( int i = mod(tura - 1, 10); j < 5; i = mod(i - 1, 10), j++ )
            dzielnik += liczba_zakupow_10_tur[i];

        return (float)suma / dzielnik;
    }
    private int mod( int n, int dzielnik ) {
        if ( dzielnik == 0 )
            throw new ArithmeticException();
        if ( n < 0 )
            return ( n % dzielnik ) + dzielnik - 1;

        return n % dzielnik;
    }
}
