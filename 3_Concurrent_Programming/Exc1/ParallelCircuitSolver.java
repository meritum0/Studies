package cp2024.solution;

import cp2024.circuit.*;

import java.util.concurrent.*;

public class ParallelCircuitSolver implements CircuitSolver {
    private final ForkJoinPool pool = new ForkJoinPool();
    private Boolean stopComputations = false;

    @Override
    public synchronized CircuitValue solve(Circuit c) {
        if ( stopComputations )
            return new PBrokenCircuitValue();

        RecursiveSolver solver = new RecursiveSolver( c.getRoot(), null, this );
        return new ParallelCircuitValue( pool.submit(solver), this, solver );
    }

    @Override
    public synchronized void stop() {
        stopComputations = true;
        pool.shutdownNow();
    }

    public synchronized boolean is_stopped() {
        return stopComputations;
    }
}
