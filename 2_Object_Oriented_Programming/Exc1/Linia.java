package tramwaje;

import java.util.Arrays;

public class Linia {
    private final int nr;
    private Tramwaj[] tramwaje;
    private int size;
    private final Trasa trasa;
    public Linia( int nr, Trasa trasa ) {
        this.nr = nr;
        tramwaje = new Tramwaj[0];
        this.size = 0;
        this.trasa = trasa;
    }

    /*********************** Metody public ************************/

    // Wypisuje liczbę tramwajów linii, długość trasy i przystanki na trasie z czasami dojazdów
    public void wypisz() {
        System.out.println(
                "Liczba tramwajów linii: " + size +
                " Długość trasy: " + trasa.ilePrzystankow() + "\n");

        trasa.wypisz();
    }

    public int nr() { return nr; }

    public void dodajTramwaj( Tramwaj tramwaj ) {
        if ( size >= tramwaje.length )
            powieksz();
        tramwaje[size] = tramwaj;
        size++;
    }

    // Wrzuca trasy wszystkich tramwajów linii do podanej kolejki zdarzeń
    public void trasyDoKolejki( KolejkaZdarzen kolejka, int dzien, Godzina LIMIT ) {
        int odstepy = obliczOdstepy();
        Godzina current = new Godzina( 6, 0 );
        for ( int i = 0; i < Math.floorDiv( size, 2 ); i++ ) {
            tramwaje[i].ustawStartowy(0);
            tramwaje[i].ustawKierunek(true);
            tramwaje[i].trasaDoKolejki( trasa, kolejka, new Czas(dzien, current), LIMIT );

            tramwaje[size - 1 - i].ustawStartowy(trasa.ilePrzystankow() - 1);
            tramwaje[size - 1 - i].ustawKierunek(false);
            tramwaje[size - 1 - i].trasaDoKolejki( trasa, kolejka, new Czas(dzien, current), LIMIT );

            current = current.dodaj(odstepy);
        }
        if ( size % 2 == 1 ) {
            tramwaje[size / 2].ustawStartowy(0);
            tramwaje[size / 2].ustawKierunek(true);
            tramwaje[size / 2].trasaDoKolejki( trasa, kolejka, new Czas(dzien, current), LIMIT );
        }
    }

    public Przystanek losujPrzystanek( int idx, boolean kierunek ) {
        return trasa.losujPrzystanek( idx, kierunek );
    }
    public void clean() {
        for ( int i = 0; i < size; i++ )
            tramwaje[i].clean();
        trasa.clean();
    }

    /*********************** Metody private ************************/

    // Oblicza odstępy puszczania tramwajów rano w trasę
    private int obliczOdstepy() {
        if ( size == 0 ) return -1;
        return trasa.czasTrasy() / size;
    }

    private void powieksz() {
        tramwaje = Arrays.copyOf( tramwaje, tramwaje.length * 2 + 2 );
    }
}
