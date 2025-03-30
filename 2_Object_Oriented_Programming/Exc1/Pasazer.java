package tramwaje;

public class Pasazer {
    private final int id;
    private final Przystanek przystanek_startowy;
    private Godzina godzina_wyjscia;    // godzina wyjścia rano z domu na przystanek
    private Przystanek nastepny;
    private int liczba_przejazdow;
    private int liczba_czekan;  // ile razy pasażer czekał na przystanku na tramwaj
    private Godzina ostatnia_wysiadka;
    private int laczny_czas_czekania; // w minutach
    private boolean czy_zrezygnowal;  // czy ten pasażer zrezygnował z podróży danego dnia
    private boolean czy_w_tramwaju;

    public Pasazer( int id, Przystanek przystanek ) {
        this.id = id;
        this.przystanek_startowy = przystanek;
    }

    /*********************** Metody public ************************/

    public int id() { return id; }
    public Przystanek nastepny() { return nastepny; }
    public int liczbaCzekan() { return liczba_czekan; }
    public int liczbaPrzejazdow() { return liczba_przejazdow; }

    // Zwraca czas czekania pasażera na pprzystankach w ciągu dnia
    public int czasCzekania( Godzina KONIEC ) {
        if ( czy_zrezygnowal )
            return 0;

        // Interpretacja jest taka, że pasażerowie, którym się nie udało
        // wysiąść na ostatnim przystanku ostatniego przejazdu idą do domu
        if ( !czy_w_tramwaju )
            laczny_czas_czekania += Godzina.roznicaCzasu( ostatnia_wysiadka, KONIEC );
        return laczny_czas_czekania;
    }

    public void nowyPrzejazd( Godzina godzina ) {
        liczba_przejazdow++;
        czy_w_tramwaju = true;
        laczny_czas_czekania += Godzina.roznicaCzasu( ostatnia_wysiadka, godzina );
    }

    public void nowaWysiadka( Godzina godzina ) {
        liczba_czekan++;
        czy_w_tramwaju = false;
        ostatnia_wysiadka = godzina;
    }

    public void zrezygnowal() { czy_zrezygnowal = true; }

    // Przypisuje losową godzinę między 6 a 12 (włącznie)
    public void losujGodzine() {
        Godzina los = new Godzina( Losowanie.losuj( 6, 11 ), Losowanie.losuj( 0, 59) );
        los = los.dodaj( Losowanie.losuj(0, 1) );
        godzina_wyjscia = los;
        ostatnia_wysiadka = godzina_wyjscia;
    }

    public void losujPrzystanek( Linia linia, int idx, boolean kierunek ) {
        nastepny = linia.losujPrzystanek( idx, kierunek );
    }

    public void wyjdzZDomu( KolejkaZdarzen kolejka, int dzien ) {
        kolejka.wstaw( new WyjscieZDomu(this, przystanek_startowy, new Czas(dzien,  godzina_wyjscia)) );
    }

    public void clean() {
        liczba_przejazdow = 0;
        liczba_czekan = 0;
        laczny_czas_czekania = 0;
        czy_zrezygnowal = false;
        czy_w_tramwaju = false;
    }
}
