package tramwaje;

public class Tramwaj extends Pojazd {
    private final int max;      // pojemnośc tramwaju
    private int startowy_idx;   // indeks na trasie startowego przystanku pasażera
    private boolean kierunek;   // true - tramwaj jedzie z pierwszej petli do drugiej, false - wpp.
    private ZbiorList pasazerowie = new ZbiorList();

    public Tramwaj( int nr_boczny, int max, Linia linia ) {
        super( nr_boczny, linia );
        this.max = max;
    }

    /*********************** Metody public ************************/

    public void ustawKierunek( boolean kierunek ) {
        this.kierunek = kierunek;
    }
    public int nr_boczny() { return getNr_boczny(); }
    public Linia linia() { return getLinia(); }
    public void ustawStartowy( int idx ) { startowy_idx = idx; }

    public void wysiadanie( Przystanek przystanek, Czas czas ) {
        while ( przystanek.czyMozna() ) {
            Pasazer p = pasazerowie.wypusc(przystanek);
            if ( p == null )
                return;
            przystanek.dodajWysiadajacego(p, czas.godzina() );
            System.out.println( czas.dzien() + ", " +  czas.godzina().toString() + ": Pasażer " + p.id() +
                    " wysiadł na przystanku " + przystanek.toString() );
        }
    }

    public void wsiadanie( Przystanek przystanek, int idx, boolean direction, Czas czas ) {
        while ( czyMozna() ) {
            Pasazer p = przystanek.wyjmijLosowego();
            if ( p == null )
                return;
            p.losujPrzystanek( getLinia(), idx, direction);
            dodajCzlowieka( p, czas );
            System.out.println( czas.dzien() + ", " +  czas.godzina().toString() + ": Pasażer " + p.id() +
                    " wsiadł do tramwaju linii " + getLinia().nr() + " (nr boczny " + getNr_boczny() +
                    ") i jedzie na przystanek " + p.nastepny().toString() );
        }
    }

    // Wrzuca trasę tramwaju do podanej kolejki zdarzeń
    public void trasaDoKolejki( Trasa trasa, KolejkaZdarzen kolejka, Czas start, Godzina LIMIT ) {
        int idx;
        if ( kierunek )
            idx = 0;
        else
            idx = trasa.ilePrzystankow() - 1;

        boolean poczatkowy_kierunek = kierunek;
        Godzina teraz = start.godzina();
        Przystanek current = trasa.przystanek(startowy_idx);
        while ( idx != startowy_idx || teraz.czyWczesniej(LIMIT) ) {
            kolejka.wstaw( new Przyjazd( this, current, new Czas(start.dzien(), teraz)) );
            if ( !trasa.czyKoniec(idx, kierunek) ) {
                kolejka.wstaw( new Odjazd(this, current, idx, kierunek, new Czas(start.dzien(), teraz)) );
                teraz = teraz.dodaj( trasa.zaIleNastepny(idx, kierunek) );
                current = trasa.nastepnyPrzystanek( idx, kierunek );
                if ( kierunek )
                    idx++;
                else
                    idx--;
            }
            else {
                teraz = teraz.dodaj( trasa.zaIleNastepny(idx, kierunek) );
                zmienKierunek();
            }
        }
        if ( kierunek != poczatkowy_kierunek )
            kolejka.wstaw( new Przyjazd(this, current, new Czas(start.dzien(), teraz)) );
    }

    public void clean() {
        pasazerowie = new ZbiorList();
    }

    /*********************** Metody private ************************/

    private void zmienKierunek() { kierunek = !kierunek; }

    // Czy jest miejsce w tramwaju
    private boolean czyMozna() {
        return pasazerowie.ileLudzi() < max;
    }

    private void dodajCzlowieka( Pasazer pasazer, Czas czas ) {
        if ( pasazer == null || czas == null )
            return;
        pasazer.nowyPrzejazd( czas.godzina() );
        pasazerowie.dodaj(pasazer);
    }
}
