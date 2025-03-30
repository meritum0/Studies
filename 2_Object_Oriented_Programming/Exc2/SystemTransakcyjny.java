package gielda;

import java.util.Random;

public class SystemTransakcyjny {
    private int liczba_tur;
    private int aktualna_tura;
    private ZbiorInwestorow inwestorzy = new ZbiorInwestorow();
    private ZbiorZlecen zlecenia = new ZbiorZlecen();
    private ZbiorSpolek spolki_na_gieldzie = new ZbiorSpolek();
    private String nastepny_id = "A";

    //*************************************** METODY PUBLIC *******************************************//

    public int tura() { return aktualna_tura; }

    public void ustawLiczbeTur( int liczba_tur ) {
        this.liczba_tur = liczba_tur;
    }

    // Podaje unikalne dla symulacji id zlecenia.
    public String podajId() {
        String nastepny = "";
        String ret = nastepny_id;

        int i = nastepny_id.length() - 1;
        for ( ; i >= 0; i-- ) {
            if ( nastepny_id.charAt(i) != 'Z' )
                break;
        }

        if ( i < 0 ) {
            if ( nastepny_id.length() > 5 )
                throw new Przepelnienie();
            else
                for ( int j = 0; j <= nastepny_id.length(); j++ )
                    nastepny += 'A';
        }
        else {
            for ( int j = 0; j < i; j++ )
                nastepny += nastepny_id.charAt(j);
            nastepny += (char)(nastepny_id.charAt(i++) + 1);
            for ( ; i < nastepny_id.length(); i++ ) {
                nastepny += 'A';
            }
        }

        nastepny_id = nastepny;
        return ret;
    }

    // Znajduje spółkę, której akcje powinien kupić inwestor SMA.
    public Spolka znajdzDoKupienia() {
        return spolki_na_gieldzie.znajdzDoKupienia(aktualna_tura);
    }

    // Znajduje spółkę, której akcje powinien sprzedać inwestor SMA.
    public Spolka znajdzDoSprzedazy( Portfel portfel ) {
        return spolki_na_gieldzie.znajdzDoSprzedazy( portfel, aktualna_tura );
    }

    public boolean czyPosiada( Spolka spolka ) { return spolki_na_gieldzie.czyPosiada(spolka); }

    public void dodajKupno( Zlecenie z ) {
        zlecenia.dodajKupno(z);
    }

    public void dodajSprzedaz( Zlecenie z ) {
        zlecenia.dodajSprzedaz(z);
    }

    // Jeśli spółka już istnieje w bazie systemu, to ją zwraca.
    public Spolka podajKonkretnaSpolke( String nazwa ) {
        return spolki_na_gieldzie.podajKonkretnaSpolke(nazwa);
    }

    public Spolka losowaSpolka() {
        return spolki_na_gieldzie.losowaSpolka();
    }

    public int losowaTura() {
        Random random = new Random();
        return  mod( random.nextInt(), liczba_tur - aktualna_tura ) + aktualna_tura;
    }

    public void dodajSpolke( Spolka spolka ) {
        spolki_na_gieldzie.dodaj(spolka);
    }

    public void ustawGotowke ( int liczba ){
        inwestorzy.ustawGotowke(liczba);
    }

    // Dodaje inwestorom startowe akcje.
    public void dodajStartoweAkcje ( Akcje akcje ) {
        inwestorzy.dodajStartoweAkcje(akcje);
    }

    public void nowyRandom () {
        inwestorzy.nowyRandom();
    }

    public void nowySMA () {
        inwestorzy.nowySMA();
    }

    // Startuje symulację.
    public void start () {
        aktualna_tura = 0;
        for ( ; aktualna_tura < liczba_tur; aktualna_tura++ ) {
            inwestorzy.odpytaj(this);
            zlecenia.realizuj();
            zlecenia.wyczysc();
            spolki_na_gieldzie.koniecTury(aktualna_tura);
        }

        aktualna_tura = 0;
        nastepny_id = "A";
    }

    public void wypiszRezultat() { inwestorzy.wypiszRezultat(); }

    // Metoda potrzebna do testów.
    public int sumaGotowki() { return inwestorzy.sumaGotowki(); }

    // Metoda potrzebna do testów
    public int sumaAkcji( Spolka spolka ) { return inwestorzy.sumaAkcji(spolka); }

    //*************************************** METODY PRIVATE *******************************************//

    private int mod( int n , int dzielnik ) {
        if ( dzielnik == 0 )
            throw new ArithmeticException();
        if ( n < 0 )
            return ( n % dzielnik ) + dzielnik - 1;

        return n % dzielnik;
    }
}

