package tramwaje;

public class Symulacja {
    private final int ile_dni;
    private final Godzina LIMIT = new Godzina( 23,1);
    private final Godzina KONIEC = new Godzina( 24, 0 );
    private int pojemnosc_przystanku;
    private int pojemnosc_tramwaju;
    private final Przystanek[] przystanki;
    private final Pasazer[] pasazerowie;
    private final Linia[] linie;
    private long liczba_przejazdow;
    private long liczba_czekan;     // ile razy pasażer czekał na przystanku na tramwaj
    private long czas_czekania;
    private int[] liczby_przejazdow;    // liczba przejazdów w różnych dniach
    private int[] czasy_czekania;       // czasy czekania w różnych dniach


    public Symulacja( int ile_dni, int liczba_pasazerow, int pojemnosc_przystanku,
                      int pojemnosc_tramwaju, Przystanek[] przystanki, Linia[] linie ) {
        this.ile_dni = ile_dni;
        this.pasazerowie = new Pasazer[liczba_pasazerow];
        this.pojemnosc_przystanku = pojemnosc_przystanku;
        this.pojemnosc_tramwaju = pojemnosc_tramwaju;
        this.przystanki = przystanki;
        this.linie = linie;
        liczby_przejazdow = new int[ile_dni];
        czasy_czekania = new int[ile_dni];
    }

    /*********************** Metody public ************************/

    public void wypiszParametry() {
        System.out.println(
                "Liczba dni symulacji: " + ile_dni +
                "\nPojemność przystanku: " + pojemnosc_przystanku +
                "\nLiczba przystanków: " + przystanki.length );

        for ( Przystanek p : przystanki )
            System.out.println( p.toString() );

        System.out.println(
                "Liczba pasażerów: " + pasazerowie.length +
                "\nPojemność tramwaju: " + pojemnosc_tramwaju +
                "\nLiczba linii tramwajowych: " + linie.length );

        for ( Linia l : linie )
            l.wypisz();
    }

    public void startujSymulacje() {
        wypiszParametry();
        stworzPasazerow();
        for ( int i = 0; i < ile_dni; i++ )
            symulacjaDnia(i);
        statystykiSymulacji();
    }

    public void clean() {
        for ( Linia l : linie )
            l.clean();
        for ( Pasazer p : pasazerowie )
            p.clean();
    }

    /*********************** Metody private ************************/

    private Przystanek losujPrzystanek() {
        int idx = Losowanie.losuj( 0, przystanki.length - 1 );
        return przystanki[idx];
    }

    private void stworzPasazerow() {
        for ( int i = 0; i < pasazerowie.length; i++ ) {
            pasazerowie[i] = new Pasazer( i, losujPrzystanek() );
        }
    }

    private void obliczStatystykiDnia( int dzien ) {
        int suma_przejazdow = 0, suma_czekania = 0;
        for ( Pasazer p : pasazerowie ) {
            suma_przejazdow += p.liczbaPrzejazdow();
            suma_czekania += p.czasCzekania(KONIEC);
            liczba_czekan += p.liczbaCzekan();
        }
        liczby_przejazdow[dzien] = suma_przejazdow;
        czasy_czekania[dzien] = suma_czekania;

        liczba_przejazdow += suma_przejazdow;
        czas_czekania += suma_czekania;
    }

    private void statystykiSymulacji() {
        float srednia;
        if ( liczba_czekan ==  0 )
            srednia = 0;
        else
            srednia = (float) czas_czekania / liczba_czekan;

        for ( int i = 0; i < ile_dni; i++ ) {
            System.out.println(
                    "Dzień " + i +
                    "\nLiczba przejazdów: " + liczby_przejazdow[i] +
                    "\nCzas czekania na przystankach: " + czasy_czekania[i] + "\n\n" );
        }

        System.out.println( "Statystyki symulacji: \n" +
                            "Łączna liczba przejazdów pasażerów: " + liczba_przejazdow +
                            "\nŚredni czas czekania na przystanku: " + srednia + " min" );
    }

    private void symulacjaDnia( int dzien ) {
        KolejkaZdarzen kolejka = new Kolejka();
        Czas czas = new Czas( dzien, new Godzina(24, 0) );
        kolejka.wstaw( new KoniecDnia(czas) );

        for ( Pasazer p : pasazerowie ) {
            p.losujGodzine();
            p.wyjdzZDomu( kolejka, dzien );
        }

        for ( Linia l : linie )
            l.trasyDoKolejki( kolejka, dzien, LIMIT );

        kolejka.sortuj();

        while ( !kolejka.isEmpty() )
            kolejka.wyciagnij().wykonaj();

        obliczStatystykiDnia(dzien);
        clean();
    }
}