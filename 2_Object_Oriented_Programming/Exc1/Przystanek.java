package tramwaje;

public class Przystanek {
    private final String nazwa;
    private final int max;  // pojemność przystanku
    private ListaPasazerow oczekujacy = new ListaPasazerow(this);
    private ListaPasazerow dopiero_wysiedli = new ListaPasazerow(this);

    public Przystanek( String nazwa, int max ) {
        this.nazwa = nazwa;
        this.max = max;
    }

    /*********************** Metody public ************************/

    public void dodajOczekujacego( Pasazer pasazer ) { oczekujacy.push(pasazer); }

    public void dodajWysiadajacego( Pasazer pasazer, Godzina godzina ) {
        pasazer.nowaWysiadka(godzina);
        dopiero_wysiedli.push(pasazer);
    }

    // Przerzuca pasażerów z [dopiero_wysiedli] do [oczekujacy]
    public void poWymianie() {
        while ( dopiero_wysiedli.ileLudzi() > 0 )
            dodajOczekujacego( dopiero_wysiedli.wyjmijLosowego() );
    }

    public Pasazer wyjmijLosowego() {
        return oczekujacy.wyjmijLosowego();
    }

    public static Przystanek wyszukaj( String nazwa, Przystanek[] przystanki ) {
        for ( Przystanek p : przystanki )
            if ( p.czyTenPrzystanek(nazwa) )
                return p;

        return null;    // nie ma takiego przystanku
    }

    public boolean czyMozna() {
        return oczekujacy.ileLudzi() + dopiero_wysiedli.ileLudzi() < max;
    }

    public void clean() {
        oczekujacy = new ListaPasazerow(this);
        dopiero_wysiedli = new ListaPasazerow(this);
    }

    public String toString() { return nazwa; }

    /*********************** Metody private ************************/

    private boolean czyTenPrzystanek( String nazwa ) {
        return this.nazwa.equals(nazwa);
    }
}
