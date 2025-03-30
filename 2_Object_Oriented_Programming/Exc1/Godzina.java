package tramwaje;

public class Godzina {
    private final int godz;
    private final int min;

    public Godzina( int godz, int min ) {
        this.godz = godz;
        this.min = min;
    }

    /*********************** Metody public ************************/

    public static int roznicaCzasu( Godzina pierwsza, Godzina druga ) {
        if ( druga.czyWczesniej(pierwsza) ) {
            Godzina temp = druga;
            druga = pierwsza;
            pierwsza = temp;
        }

        int godziny = druga.godz - pierwsza.godz;
        int minuty = druga.min - pierwsza.min;

        if ( minuty < 0 ) {
            godziny--;
            minuty = 60 - pierwsza.min + druga.min;
        }

        return godziny * 60 + minuty;
    }

    public boolean czyWczesniej( Godzina g ) {
        return ( this.godz < g.godz || (this.godz == g.godz && this.min < g.min) );
    }

    public boolean equal( Godzina g ) {
        return this.godz == g.godz && this.min == g.min;
    }

    public Godzina dodaj( int minuty ) {
        int g = this.godz;
        int m = this.min + minuty;
        if ( m >= 60 ) {
            int temp = m / 60;
            g += temp;
            m = m % 60;
        }

        return new Godzina( g, m );
    }

    public String toString() {
        return min < 10 ? ( godz + ":0" + min ) : ( godz + ":" + min );
    }
}

