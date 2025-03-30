package tramwaje;

public class ListaPasazerow {

    private class Element {
        private final Pasazer pasazer;
        private Element next;

        public Element(Pasazer pasazer ) {
            this.pasazer = pasazer;
        }

        /*********************** Metody public ************************/

        public Pasazer pasazer() { return pasazer; }

        public Element next() { return next; }

        public void ustawNastepnik( Element element ) {
            this.next = element;
        }
    }


    private final Przystanek przystanek;
    private int size;
    private Element first;
    private Element last;

    public ListaPasazerow( Przystanek przystanek ) {
        this.przystanek = przystanek;
    }

    /*********************** Metody public ************************/

    public Przystanek przystanek() { return przystanek; }
    public int ileLudzi() { return size; }
    public void push( Pasazer pasazer ) {
        Element e = new Element(pasazer);
        if ( last == null ) {
            first = e;
        }
        else {
            last.ustawNastepnik(e);

        }
        last = e;
        size++;
    }

    public Pasazer wyjmijLosowego() {
        if ( size == 0 )
            return null;
        return pop( Losowanie.losuj(1, size) );
    }

    /*********************** Metody private ************************/

    private Pasazer pop( int n ) {
        if( last == null)
            return null;
        Element current = first;
        Element prev = current;
        for ( int i = 1; i < n; i++ ) {
            prev = current;
            current = current.next();
        }
        if ( current == first ) {
            first = first.next();
        }
        if ( current == last ) {
            last = ( prev == current ? null : prev );
        }
        prev.ustawNastepnik( current.next() );
        size--;
        return current.pasazer();
    }
}
