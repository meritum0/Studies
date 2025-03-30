package gielda;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

public class GieldaTests {
    @Test
    public void checkSpolka() {
        Spolka s1 = new Spolka("A");
        Spolka s2 = new Spolka("A");
        Spolka s3 = new Spolka("B");

        assertTrue( s1.equals(s2) );
        assertFalse( s1.equals(s3) );

        s1.ostatniaTransakcja(20);
        int a = s1.losowaCena();
        assertTrue( a <= 30 && a >= 10 );

        for ( int i = 0; i < 10; i++ ) {
            int cena = 30;
            if ( i == 9 )
                cena = 100;
            if ( i == 8 )
                cena = 25;
            s1.uaktualnij(1, cena);
            s1.koniecTury(i);
        }

        assertTrue( s1.czyMomentKupna(10) );
        assertFalse( s1.czyMomentSprzedazy(10) );

        for ( int i = 0; i < 20; i++ ) {
            int cena = 30;
            if ( i == 19 )
                cena = 1;
            s1.uaktualnij(1, cena);
            s1.koniecTury(i);
        }

        assertFalse( s1.czyMomentKupna(10) );
        assertTrue( s1.czyMomentSprzedazy(10) );
    }

    @Test
    public void checkAkcje() {
        Spolka s1 = new Spolka("APL");
        Spolka s2 = new Spolka("MSFT");
        Akcje a1 = new Akcje( s1, 10 );
        Akcje a2 = new Akcje( s1, 5 );
        Akcje a3 = new Akcje( s2, 10 );

        assertTrue( a1.equals(a2) );
        assertFalse( a1.equals(a3));

        a1.dodajLiczbe(3);
        assertEquals( 13, a1.liczba() );

        a2.odejmijLiczbe(4);
        assertEquals( 1, a2.liczba());
    }

    @Test
    public void checkDoTury() {
        SystemTransakcyjny s = new SystemTransakcyjny();
        Akcje akcje = new Akcje( new Spolka("X"), 1 );

        DoTury d = new DoTury( true, "A", new Inwestor( new Losowa() ), new Spolka("x"), 1, 1, 0, s );

        assertTrue( d.czyUsunac() );
    }

    @Test
    public void checkPortfel() {
        Spolka s = new Spolka("X");
        Akcje a = new Akcje( s, 10 );
        Portfel p = new Portfel();
        p.dodajGotowke(100);

        Zlecenie z = new Natychmiastowe( true, "A", new Inwestor( new Losowa() ), s, 10, 1 );

        assertTrue( p.czyStac( z, 6) );
        assertFalse( p.czyPosiada(s) );

        p.dodajAkcje(a, 5);

        assertTrue( p.czyPosiada(s) );
        assertFalse( p.czyStac(z, 6) );
        assertTrue( p.czyJeszczeMozliwa(s, 10) );
        assertFalse( p.czyJeszczeMozliwa(s, 11) );
    }

    @Test
    public void checkZbiorAkcji() {
        ZbiorAkcji z = new ZbiorAkcji();
        Spolka s1 = new Spolka("X");
        Spolka s2 = new Spolka("Y");
        Akcje a = new Akcje(s1, 10 );
        z.dodaj(a);

        assertTrue( z.czyPosiada(s1) );
        assertFalse( z.czyPosiada(s2) );

        assertTrue( z.czyJeszczeMozliwa(s1, 10) );
        assertEquals( s1, z.losowaSpolka() );
        int n = z.losowaLiczbaDoSprzedazy(s1);
        assertTrue( n > 0 && n <= 10 );

        z.odejmij(a);
        assertFalse( z.czyPosiada(s1) );
    }

    @Test
    public void checkZlecenie() {
        Inwestor inwestor = new Inwestor( new Losowa() );
        Spolka spolka = new Spolka("X");
        Zlecenie z1 = new Natychmiastowe( true, "AAZZ", inwestor, spolka, 10, 10 );
        Zlecenie z2 = new Natychmiastowe( true, "ABZZ", inwestor, spolka, 10, 10 );
        Zlecenie z3 = new Natychmiastowe( true, "AAAAA", inwestor, spolka, 10, 10 );
        Zlecenie z4 = new Natychmiastowe( true, "BAAA", inwestor, spolka, 10, 10 );

        assertEquals( 1, z1.czyStarsze(z2) );
        assertEquals( 1, z2.czyStarsze(z3) );
        assertEquals( 1, z2.czyStarsze(z4) );
    }

    @Test
    public void checkSystemTransakcyjnyID() {
        SystemTransakcyjny system = new SystemTransakcyjny();
        String s = system.podajId();
        assertEquals( "A", s );
        s = system.podajId();
        assertEquals( "B", s);

        for ( int i = 0; i < 25; i++ )
            s = system.podajId();

        assertEquals( "AA", s );

        for ( int i = 0; i < 26; i++ )
            s = system.podajId();

        assertEquals( "BA", s );

        system.ustawLiczbeTur(2);
        int x = system.losowaTura();
        assertTrue( x >= 0 && x < 2 );
    }

    @Test
    public void checkSystemTransakcyjnyRandom() {
        SystemTransakcyjny sys = new SystemTransakcyjny();
        Spolka s1 = new Spolka("X");
        s1.ostatniaTransakcja(200);
        Spolka s2 = new Spolka("Y");
        s1.ostatniaTransakcja(300);
        Spolka s3 = new Spolka("Z");
        s1.ostatniaTransakcja(500);
        sys.dodajSpolke(s1);
        sys.dodajSpolke(s2);
        sys.dodajSpolke(s3);

        sys.nowyRandom();
        sys.nowyRandom();
        sys.nowyRandom();
        sys.nowyRandom();

        sys.ustawLiczbeTur(100);

        Akcje a1 = new Akcje( s1, 100 );
        Akcje a2 = new Akcje( s2, 50 );
        Akcje a3 = new Akcje( s3, 20 );

        sys.dodajStartoweAkcje(a1);
        sys.dodajStartoweAkcje(a2);
        sys.dodajStartoweAkcje(a3);

        sys.ustawGotowke(2000);

        sys.start();

        int suma_gotowki = sys.sumaGotowki();
        assertEquals( 8000, suma_gotowki );

        int suma_x = sys.sumaAkcji(s1);
        assertEquals( 400, suma_x );
        int suma_y = sys.sumaAkcji(s2);
        assertEquals( 200, suma_y );
        int suma_z = sys.sumaAkcji(s3);
        assertEquals( 80, suma_z );
    }

    @Test
    public void checkSystemTransakcyjnySMA() {
        SystemTransakcyjny sys = new SystemTransakcyjny();
        Spolka s1 = new Spolka("X");
        s1.ostatniaTransakcja(200);
        Spolka s2 = new Spolka("Y");
        s1.ostatniaTransakcja(300);
        Spolka s3 = new Spolka("Z");
        s1.ostatniaTransakcja(500);
        sys.dodajSpolke(s1);
        sys.dodajSpolke(s2);
        sys.dodajSpolke(s3);

        sys.nowySMA();
        sys.nowySMA();
        sys.nowySMA();
        sys.nowySMA();

        sys.ustawLiczbeTur(100);

        Akcje a1 = new Akcje( s1, 100 );
        Akcje a2 = new Akcje( s2, 50 );
        Akcje a3 = new Akcje( s3, 20 );

        sys.dodajStartoweAkcje(a1);
        sys.dodajStartoweAkcje(a2);
        sys.dodajStartoweAkcje(a3);

        sys.ustawGotowke(2000);

        sys.start();

        int suma_gotowki = sys.sumaGotowki();
        assertEquals( 8000, suma_gotowki );

        int suma_x = sys.sumaAkcji(s1);
        assertEquals( 400, suma_x );
        int suma_y = sys.sumaAkcji(s2);
        assertEquals( 200, suma_y );
        int suma_z = sys.sumaAkcji(s3);
        assertEquals( 80, suma_z );
    }

    @Test
    public void checkSystemTransakcyjnyMix() {
        SystemTransakcyjny sys = new SystemTransakcyjny();
        Spolka s1 = new Spolka("X");
        s1.ostatniaTransakcja(200);
        Spolka s2 = new Spolka("Y");
        s1.ostatniaTransakcja(300);
        Spolka s3 = new Spolka("Z");
        s1.ostatniaTransakcja(500);
        sys.dodajSpolke(s1);
        sys.dodajSpolke(s2);
        sys.dodajSpolke(s3);

        sys.nowyRandom();
        sys.nowyRandom();
        sys.nowyRandom();
        sys.nowyRandom();
        sys.nowySMA();
        sys.nowySMA();
        sys.nowySMA();

        sys.ustawLiczbeTur(100);

        Akcje a1 = new Akcje( s1, 100 );
        Akcje a2 = new Akcje( s2, 50 );
        Akcje a3 = new Akcje( s3, 20 );

        sys.dodajStartoweAkcje(a1);
        sys.dodajStartoweAkcje(a2);
        sys.dodajStartoweAkcje(a3);

        sys.ustawGotowke(2000);

        sys.start();

        int suma_gotowki = sys.sumaGotowki();
        assertEquals( 14000, suma_gotowki );

        int suma_x = sys.sumaAkcji(s1);
        assertEquals( 700, suma_x );
        int suma_y = sys.sumaAkcji(s2);
        assertEquals( 350, suma_y );
        int suma_z = sys.sumaAkcji(s3);
        assertEquals( 140, suma_z );
    }

    @Test
    public void checkSystemTransakcyjnyZeroCenySpolki() {
        SystemTransakcyjny sys = new SystemTransakcyjny();
        Spolka s1 = new Spolka("X");
        s1.ostatniaTransakcja(0);
        Spolka s2 = new Spolka("Y");
        s1.ostatniaTransakcja(0);
        Spolka s3 = new Spolka("Z");
        s1.ostatniaTransakcja(0);
        sys.dodajSpolke(s1);
        sys.dodajSpolke(s2);
        sys.dodajSpolke(s3);

        sys.nowyRandom();
        sys.nowyRandom();
        sys.nowyRandom();
        sys.nowyRandom();

        sys.ustawLiczbeTur(100);

        Akcje a1 = new Akcje( s1, 100 );
        Akcje a2 = new Akcje( s2, 50 );
        Akcje a3 = new Akcje( s3, 20 );

        sys.dodajStartoweAkcje(a1);
        sys.dodajStartoweAkcje(a2);
        sys.dodajStartoweAkcje(a3);

        sys.ustawGotowke(2000);

        sys.start();

        int suma_gotowki = sys.sumaGotowki();
        assertEquals( 8000, suma_gotowki );

        int suma_x = sys.sumaAkcji(s1);
        assertEquals( 400, suma_x );
        int suma_y = sys.sumaAkcji(s2);
        assertEquals( 200, suma_y );
        int suma_z = sys.sumaAkcji(s3);
        assertEquals( 80, suma_z );
    }

    @Test
    public void checkSystemTransakcyjnyZeroAkcjiInwestorow() {
        SystemTransakcyjny sys = new SystemTransakcyjny();
        Spolka s1 = new Spolka("X");
        s1.ostatniaTransakcja(200);
        Spolka s2 = new Spolka("Y");
        s1.ostatniaTransakcja(300);
        Spolka s3 = new Spolka("Z");
        s1.ostatniaTransakcja(500);
        sys.dodajSpolke(s1);
        sys.dodajSpolke(s2);
        sys.dodajSpolke(s3);

        sys.nowyRandom();
        sys.nowyRandom();
        sys.nowyRandom();
        sys.nowyRandom();

        sys.ustawLiczbeTur(100);

        Akcje a1 = new Akcje( s1, 0 );
        Akcje a2 = new Akcje( s2, 0 );
        Akcje a3 = new Akcje( s3, 0 );

        sys.dodajStartoweAkcje(a1);
        sys.dodajStartoweAkcje(a2);
        sys.dodajStartoweAkcje(a3);

        sys.ustawGotowke(2000);

        sys.start();

        int suma_gotowki = sys.sumaGotowki();
        assertEquals( 8000, suma_gotowki );

        int suma_x = sys.sumaAkcji(s1);
        assertEquals( 0, suma_x );
        int suma_y = sys.sumaAkcji(s2);
        assertEquals( 0, suma_y );
        int suma_z = sys.sumaAkcji(s3);
        assertEquals( 0, suma_z );
    }

    @Test
    public void checkSystemTransakcyjnyZeroGotowki() {
        SystemTransakcyjny sys = new SystemTransakcyjny();
        Spolka s1 = new Spolka("X");
        s1.ostatniaTransakcja(200);
        Spolka s2 = new Spolka("Y");
        s1.ostatniaTransakcja(300);
        Spolka s3 = new Spolka("Z");
        s1.ostatniaTransakcja(500);
        sys.dodajSpolke(s1);
        sys.dodajSpolke(s2);
        sys.dodajSpolke(s3);

        sys.nowyRandom();
        sys.nowyRandom();
        sys.nowyRandom();
        sys.nowyRandom();

        sys.ustawLiczbeTur(100);

        Akcje a1 = new Akcje( s1, 100 );
        Akcje a2 = new Akcje( s2, 50 );
        Akcje a3 = new Akcje( s3, 20 );

        sys.dodajStartoweAkcje(a1);
        sys.dodajStartoweAkcje(a2);
        sys.dodajStartoweAkcje(a3);

        sys.ustawGotowke(0);

        sys.start();

        int suma_gotowki = sys.sumaGotowki();
        assertEquals( 0, suma_gotowki );

        int suma_x = sys.sumaAkcji(s1);
        assertEquals( 400, suma_x );
        int suma_y = sys.sumaAkcji(s2);
        assertEquals( 200, suma_y );
        int suma_z = sys.sumaAkcji(s3);
        assertEquals( 80, suma_z );
    }

    @Test
    public void checkSystemTransakcyjnyKazdyMa1Gotowki() {
        SystemTransakcyjny sys = new SystemTransakcyjny();
        Spolka s1 = new Spolka("X");
        s1.ostatniaTransakcja(200);
        Spolka s2 = new Spolka("Y");
        s1.ostatniaTransakcja(300);
        Spolka s3 = new Spolka("Z");
        s1.ostatniaTransakcja(500);
        sys.dodajSpolke(s1);
        sys.dodajSpolke(s2);
        sys.dodajSpolke(s3);

        sys.nowyRandom();
        sys.nowyRandom();
        sys.nowyRandom();
        sys.nowyRandom();

        sys.ustawLiczbeTur(100);

        Akcje a1 = new Akcje( s1, 100 );
        Akcje a2 = new Akcje( s2, 50 );
        Akcje a3 = new Akcje( s3, 20 );

        sys.dodajStartoweAkcje(a1);
        sys.dodajStartoweAkcje(a2);
        sys.dodajStartoweAkcje(a3);

        sys.ustawGotowke(1);

        sys.start();

        int suma_gotowki = sys.sumaGotowki();
        assertEquals( 4, suma_gotowki );

        int suma_x = sys.sumaAkcji(s1);
        assertEquals( 400, suma_x );
        int suma_y = sys.sumaAkcji(s2);
        assertEquals( 200, suma_y );
        int suma_z = sys.sumaAkcji(s3);
        assertEquals( 80, suma_z );
    }
}
