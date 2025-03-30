package tramwaje;

public interface KolejkaZdarzen {
    boolean isEmpty();
    void wstaw( Event event );
    Event wyciagnij();
    void sortuj();
}
