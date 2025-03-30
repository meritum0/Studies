package tramwaje;

public class Czas {
    private int dzien;
    private Godzina godzina;

    public Czas( int dzien, Godzina godzina ) {
        this.dzien = dzien;
        this.godzina = godzina;
    }

    /*********************** Metody public ************************/

    public int dzien() { return dzien; }

    public Godzina godzina() { return godzina; }

    public boolean equal( Czas c ) {
        return dzien == c.dzien && godzina.equal(c.godzina);
    }

    public boolean czyWczesniej( Czas c ) {
        if ( dzien < c.dzien )
            return true;
        if ( dzien > c.dzien )
            return false;
        return godzina.czyWczesniej(c.godzina);
    }
}
