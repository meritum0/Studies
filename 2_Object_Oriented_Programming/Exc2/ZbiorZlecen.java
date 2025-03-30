package gielda;

import java.util.ArrayList;
import java.util.Collections;
import java.util.LinkedList;
import java.util.List;

public class ZbiorZlecen {
    private List<Zlecenie> kupna = new LinkedList<>();
    private List<Zlecenie> sprzedaze = new LinkedList<>();

    //*************************************** METODY PUBLIC *******************************************//

    public void dodajKupno( Zlecenie z ) {
        kupna.add(z);
    }

    public void dodajSprzedaz( Zlecenie z ) {
        sprzedaze.add(z);
    }

    // Realizuje zlecenia.
    public void realizuj() {
        Collections.sort(kupna);
        Collections.sort(sprzedaze);

        for ( Zlecenie kupno : kupna )
            dopasujDoSprzedazy(kupno);
    }

    // Na koniec tury sprząta zlecenia, które już powinny zniknąć z bazy.
    public void wyczysc() {
        ArrayList<Zlecenie> do_usuniecia = new ArrayList<>();
        for ( Zlecenie z : kupna ) {
            if ( z.czyUsunac() )
                do_usuniecia.add(z);
        }

        for ( Zlecenie z : do_usuniecia )
            kupna.remove(z);

        do_usuniecia.clear();

        for ( Zlecenie z : sprzedaze ) {
            if ( z.czyUsunac() )
                do_usuniecia.add(z);
        }

        for ( Zlecenie z : do_usuniecia )
            sprzedaze.remove(z);
    }

    //*************************************** METODY PRIVATE *******************************************//

    // Dopasowuje do oferty kupna, ofertę sprzedaży...
    private void dopasujDoSprzedazy( Zlecenie kupno ) {
        ArrayList<Zlecenie> do_usuniecia = new ArrayList<>();
        for ( Zlecenie oferta : sprzedaze ) {
            if ( kupno.spolka().equals(oferta.spolka()) &&
                 kupno.limit() >= oferta.limit() ) {

                if ( !oferta.czyJeszczeMozliwa() ) {
                    do_usuniecia.add(oferta);
                    continue;
                }

                int cena = oferta.limit();
                if ( kupno.czyStarsze(oferta) > 0 )
                    cena = kupno.limit();

                if ( !kupno.czyStac(oferta, cena) )
                    continue;

                if ( wymienAkcje(kupno, oferta, cena, do_usuniecia ) )    // Czy całe zlecenie kupna zostało zrealizowane?
                    break;
            }
        }

        for ( Zlecenie z : do_usuniecia )
            sprzedaze.remove(z);
    }

    // ...a po dopasowaniu, inwestorzy wymieniają się gotówką i akcjami.
    private boolean wymienAkcje( Zlecenie kupno, Zlecenie oferta, int cena, ArrayList<Zlecenie> do_usuniecia ) {
        int ile = Math.min( kupno.liczbaSztuk(), oferta.liczbaSztuk() );


        Akcje wymienione = new Akcje( kupno.spolka(), ile );
        kupno.inwestor().dodajAkcje( wymienione, cena );
        oferta.inwestor().odejmijAkcje( wymienione, cena );

        kupno.uaktualnij(ile, cena);
        oferta.uaktualnij(ile);

        if ( oferta.liczbaSztuk() == 0 )
            do_usuniecia.add(oferta);

        if ( kupno.liczbaSztuk() == 0 )
            return true;

        return false;
    }
}
