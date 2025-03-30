package tramwaje;

import java.util.Arrays;

public class Kolejka implements KolejkaZdarzen {
    private Event[] sterta = new Event[0];
    private int size;

    /*********************** Metody public ************************/

    public boolean isEmpty() { return size <= 0; }
    public void wstaw( Event event )
    {
        if( size >= sterta.length )
            powieksz();

        sterta[size] = event;
        size++;
    }

    public Event wyciagnij() {
        assert size > 0;
        return sterta[--size];
    }

    public void sortuj() {
        sterta = Arrays.copyOf( sterta, size );
        Arrays.sort(sterta);
        reverse();
    }

    /*********************** Metody private ************************/

    private void reverse() {
        Event[] copy = Arrays.copyOf( sterta, size );
        for ( int i = 0; i < size; i++ ) {
            sterta[i] = copy[size - 1 - i];
        }
    }

    private void powieksz() {
        sterta = Arrays.copyOf( sterta, sterta.length * 2 + 2 );
    }
}
