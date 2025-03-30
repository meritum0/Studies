package cp2024.solution;

import cp2024.circuit.CircuitValue;

import java.util.concurrent.ExecutionException;
import java.util.concurrent.ForkJoinTask;

public class ParallelCircuitValue implements CircuitValue {
    private final ForkJoinTask<Boolean> value;
    private final ParallelCircuitSolver solver;
    private final RecursiveSolver task;
    private boolean alreadyComputed = false;
    private boolean computedValue = false;

    public ParallelCircuitValue(ForkJoinTask<Boolean> q, ParallelCircuitSolver p, RecursiveSolver t ) {
        value = q;
        solver = p;
        task = t;
    }
    @Override
    public synchronized boolean getValue() throws InterruptedException {
        try {
            if ( alreadyComputed )
                return computedValue;

            if ( solver.is_stopped() )
                throw new InterruptedException();

            computedValue = value.get();
            if ( !task.wentOK() )
                throw new InterruptedException();

            alreadyComputed = true;
            return computedValue;
        } catch ( InterruptedException | ExecutionException e ) {
            throw new InterruptedException();
        }
    }
}
