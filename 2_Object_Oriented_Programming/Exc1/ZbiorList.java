package tramwaje;

import java.util.Arrays;

public class ZbiorList {
    private ListaPasazerow[] listy_pasazerow = new ListaPasazerow[0];
    private int size;

    /*********************** Metody public ************************/

    public int ileLudzi() {
        int ret = 0;
         for ( int i = 0; i < size; i++ )
             ret += listy_pasazerow[i].ileLudzi();
         return ret;
    }

    public void dodaj( Pasazer pasazer ) {
        for ( int i = 0; i < size; i++ ) {
            if ( listy_pasazerow[i].przystanek() == pasazer.nastepny() ) {
                listy_pasazerow[i].push(pasazer);
                return;
            }
        }
        nowaLista(pasazer);
    }

    // Pozwala wysiąść losowemu pasażerowi, który chciał wysiąść na danym przystanku
    public Pasazer wypusc( Przystanek przystanek ) {
        for (int i = 0; i < size; i++ ) {
            if ( listy_pasazerow[i].ileLudzi() > 0 && listy_pasazerow[i].przystanek() == przystanek )
                return listy_pasazerow[i].wyjmijLosowego();
        }
        return null;
    }

    /*********************** Metody private ************************/

    private void nowaLista( Pasazer pasazer ) {
        if ( size >= listy_pasazerow.length )
            powieksz();
        listy_pasazerow[size] = new ListaPasazerow( pasazer.nastepny() );
        listy_pasazerow[size].push(pasazer);
        size++;
    }

    private void powieksz() {
        listy_pasazerow = Arrays.copyOf( listy_pasazerow,
                               listy_pasazerow.length * 2 + 2 );
    }
}
