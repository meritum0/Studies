package tramwaje;

public abstract class Event implements Comparable<Event> {
    private final Czas czas;

    protected Event( Czas czas ) {
        this.czas = czas;
    }

    /*********************** Metody public ************************/

    public abstract void wykonaj();

    public int compareTo( Event e ) {
        if ( czas.equal(e.czas) )
            return 0;
        if ( czas.czyWczesniej(e.czas) )
            return -1;
        return 1;
    }

    /*********************** Metody protected ************************/

    protected Czas czas() {
        return czas;
    }
}
